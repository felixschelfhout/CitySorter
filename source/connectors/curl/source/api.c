/* NOLINTBEGIN(bugprone-reserved-identifier) */
#define _POSIX_C_SOURCE 199309L
/* NOLINTEND(bugprone-reserved-identifier) */

#include "api.h"
#include <ctype.h>
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define API_BASE_URL "https://countriesnow.space/api/v0.1/countries/cities"
#define API_POST_TEMPLATE "{\"country\":\"%s\"}"
#define API_POST_BUFFER_SIZE 256
#define API_POST_COUNTRY_NAME_MAX 128
#define API_RSP_ERROR_MESSAGE_MAX 512

/**
 * @brief Internal structure for accumulating response data
 */
typedef struct {
    char *data;
    size_t size;
} ResponseBuffer;

/**
 * @brief Callback function for curl to write response data
 *
 * Called by libcurl when data is received. Accumulates data in a buffer.
 */
static size_t write_callback(const void *contents, size_t size, size_t nmemb, void *userp) {
    size_t real_size = size * nmemb;
    ResponseBuffer *buffer = (ResponseBuffer *)userp;

    char *ptr = realloc(buffer->data, buffer->size + real_size + 1);
    if (!ptr) {
        return 0; /* Out of memory */
    }

    buffer->data = ptr;
    memcpy(&(buffer->data[buffer->size]), contents, real_size);
    buffer->size += real_size;
    buffer->data[buffer->size] = '\0';

    return real_size;
}

/**
 * @brief Sleep for specified milliseconds
 *
 * Uses nanosleep for precise timing on POSIX systems.
 */
static void sleep_ms(int milliseconds) {
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;
    ts.tv_nsec = (milliseconds % 1000) * 1000000L;
    nanosleep(&ts, NULL);
}

/**
 * @brief Validate a character in a country name
 */
static int is_valid_country_char(unsigned char c) {
    return isalnum(c) || c == ' ' || c == '-' || c == '\'';
}

/**
 * @brief Set error message in response
 */
static void set_response_error(ApiResponse *response, const char *format, const char *arg) {
    if (!response) {
        return;
    }

    response->error_message = (char *)malloc(API_RSP_ERROR_MESSAGE_MAX);
    if (response->error_message) {
        snprintf(response->error_message, API_RSP_ERROR_MESSAGE_MAX, format, arg ? arg : "");
    }
}

/**
 * @brief Initialize response buffer
 */
static int init_response_buffer(ResponseBuffer *buffer) {
    buffer->data = malloc(1);
    if (!buffer->data) {
        return -1;
    }
    buffer->data[0] = '\0';
    buffer->size = 0;
    return 0;
}

/**
 * @brief Create JSON request body for country query
 */
static char *create_request_json(const char *country_name) {
    char *json = (char *)malloc(API_POST_BUFFER_SIZE);
    if (!json) {
        return NULL;
    }

    int result = snprintf(json, API_POST_BUFFER_SIZE, API_POST_TEMPLATE, country_name);
    if (result < 0 || result >= API_POST_BUFFER_SIZE) {
        free(json);
        return NULL;
    }

    return json;
}

/**
 * @brief Configure curl handle with common options
 */
static int configure_curl_handle(CURL *curl, const char *json_data, ResponseBuffer *buffer,
                                 const ApiConfig *config) {
    struct curl_slist *headers = NULL;

    /* Set URL */
    if (curl_easy_setopt(curl, CURLOPT_URL, API_BASE_URL) != CURLE_OK) {
        return -1;
    }

    /* Set POST data */
    if (curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data) != CURLE_OK) {
        return -1;
    }

    /* Set headers */
    headers = curl_slist_append(headers, "Content-Type: application/json");
    if (!headers) {
        return -1;
    }
    if (curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers) != CURLE_OK) {
        curl_slist_free_all(headers);
        return -1;
    }

    /* Set callback for response data */
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)buffer);

    /* Set timeout */
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, config->timeout_seconds);

    /* Set redirect options */
    if (config->follow_redirects) {
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_MAXREDIRS, config->max_redirect_count);
    }

    /* Enable error buffer for detailed error messages */
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, NULL);

    return 0;
}

/**
 * @brief Perform HTTP request with retry logic
 */
static CURLcode perform_request_with_retries(CURL *curl, const ApiConfig *config,
                                             ApiResponse *response) {
    CURLcode res = CURLE_FAILED_INIT;
    int attempt = 0;
    int delay_ms = config->retry_delay_ms;

    while (attempt <= config->max_retries) {
        res = curl_easy_perform(curl);

        /* Get HTTP status code */
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response->http_code);

        /* Success - return immediately */
        if (res == CURLE_OK && response->http_code == 200) {
            return res;
        }

        /* Rate limited - wait longer */
        if (response->http_code == 429) {
            attempt++;
            if (attempt <= config->max_retries) {
                sleep_ms(delay_ms);
                if (config->exponential_backoff) {
                    delay_ms *= 2;
                }
                continue;
            }
            set_response_error(response, "%s", "Rate limit exceeded after retries");
            return res;
        }

        /* Network/connection error - retry */
        if (res != CURLE_OK) {
            attempt++;
            if (attempt <= config->max_retries) {
                sleep_ms(delay_ms);
                if (config->exponential_backoff) {
                    delay_ms *= 2;
                }
                continue;
            }
            set_response_error(response, "Network error: %s", curl_easy_strerror(res));
            return res;
        }

        /* Other HTTP error - don't retry */
        return res;
    }

    return res;
}

/* Public API Implementation */

int api_connector_init(void) {
    CURLcode res = curl_global_init(CURL_GLOBAL_DEFAULT);
    return (res == CURLE_OK) ? 0 : -1;
}

void api_connector_cleanup(void) {
    curl_global_cleanup();
}

ApiConfig api_config_default(void) {
    ApiConfig config;
    config.timeout_seconds = 10;
    config.max_retries = 3;
    config.retry_delay_ms = 1000;
    config.exponential_backoff = 1;
    config.follow_redirects = 1;
    config.max_redirect_count = 5;
    return config;
}

void api_response_cleanup(ApiResponse *response) {
    if (!response) {
        return;
    }

    free(response->body);
    free(response->error_message);
    response->body = NULL;
    response->error_message = NULL;
    response->http_code = 0;
}

int api_validate_country_name(const char *country_name) {
    if (!country_name || country_name[0] == '\0') {
        return 0;
    }

    size_t length = strlen(country_name);
    if (length > API_POST_COUNTRY_NAME_MAX) {
        return 0;
    }

    for (size_t i = 0; i < length; i++) {
        if (!is_valid_country_char((unsigned char)country_name[i])) {
            return 0;
        }
    }

    return 1;
}

int api_fetch_cities(const char *country_name, const ApiConfig *config, ApiResponse *response) {
    CURL *curl = NULL;
    char *json_data = NULL;
    ResponseBuffer buffer = {NULL, 0};
    struct curl_slist *headers = NULL;
    ApiConfig default_config;

    /* Validate response pointer */
    if (!response) {
        return -1;
    }

    /* Validate input */
    if (!api_validate_country_name(country_name)) {
        set_response_error(response, "%s", "Invalid country name");
        return -1;
    }

    /* Use default config if none provided */
    if (!config) {
        default_config = api_config_default();
        config = &default_config;
    }

    /* Initialize response buffer */
    if (init_response_buffer(&buffer) != 0) {
        set_response_error(response, "%s", "Memory allocation failed");
        return -1;
    }

    /* Create JSON request body */
    json_data = create_request_json(country_name);
    if (!json_data) {
        set_response_error(response, "%s", "Failed to create request");
        free(buffer.data);
        return -1;
    }

    /* Initialize curl */
    curl = curl_easy_init();
    if (!curl) {
        set_response_error(response, "%s", "Failed to initialize curl");
        free(json_data);
        free(buffer.data);
        return -1;
    }

    /* Set URL */
    curl_easy_setopt(curl, CURLOPT_URL, API_BASE_URL);

    /* Set POST data */
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data);

    /* Set headers */
    headers = curl_slist_append(headers, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    /* Set callback for response data */
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&buffer);

    /* Set timeout */
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, config->timeout_seconds);

    /* Set redirect options */
    if (config->follow_redirects) {
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_MAXREDIRS, config->max_redirect_count);
    }

    /* Perform request with retries */
    CURLcode res = perform_request_with_retries(curl, config, response);

    /* Transfer response body to response object */
    if (buffer.data && buffer.size > 0) {
        response->body = buffer.data;
    } else {
        free(buffer.data);
        if (res == CURLE_OK && !response->error_message) {
            set_response_error(response, "%s", "Empty response from server");
        }
    }

    /* Cleanup */
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    free(json_data);

    return 0;
}

int api_response_is_success(const ApiResponse *response) {
    return response && response->http_code == 200;
}

int api_response_is_rate_limited(const ApiResponse *response) {
    return response && response->http_code == 429;
}
