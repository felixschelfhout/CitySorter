#ifndef API_H
#define API_H

#include <stddef.h>

/**
 * @file api.h
 * @brief HTTP connector abstraction for CountriesNow API
 *
 * This module handles all HTTP communication with the CountriesNow API,
 * implementing rate limiting, retry logic with exponential backoff,
 * and comprehensive error handling.
 */

/**
 * @brief HTTP response structure
 *
 * Contains the response body, HTTP status code, and error information
 * from an API request. Use API_RESPONSE_INIT to initialize.
 */
typedef struct {
    char *body;          /**< Response body (JSON string), freed by api_response_cleanup() */
    long http_code;      /**< HTTP status code (200, 404, 429, etc.) */
    char *error_message; /**< Error message if request fails, freed by api_response_cleanup() */
} ApiResponse;

/**
 * @brief Default API response initializer
 *
 * Usage:
 * @code
 * ApiResponse response = API_RESPONSE_INIT;
 * @endcode
 */
#define API_RESPONSE_INIT                                                                          \
    { NULL, 0, NULL }

/**
 * @brief Configuration for API requests
 *
 * Use API_CONFIG_DEFAULT initializer for default values.
 */
typedef struct {
    long timeout_seconds;    /**< Request timeout in seconds (default: 10) */
    int max_retries;         /**< Maximum number of retry attempts (default: 3) */
    int retry_delay_ms;      /**< Initial retry delay in milliseconds (default: 1000) */
    int exponential_backoff; /**< Use exponential backoff for retries (default: 1) */
    int follow_redirects;    /**< Follow HTTP redirects (default: 1) */
    long max_redirect_count; /**< Maximum number of redirects (default: 5) */
} ApiConfig;

/**
 * @brief Default API configuration initializer
 *
 * Usage:
 * @code
 * ApiConfig config = API_CONFIG_DEFAULT;
 * config.timeout_seconds = 15; // Customize as needed
 * @endcode
 */
#define API_CONFIG_DEFAULT                                                                         \
    { 10, 3, 1000, 1, 1, 5 }

/**
 * @brief Initialize API connector
 *
 * Must be called before making any API requests. Initializes libcurl
 * global state. Thread-safe after initialization.
 *
 * @return 0 on success, -1 on error
 */
int api_connector_init(void);

/**
 * @brief Clean up API connector
 *
 * Should be called when done with all API requests. Cleans up libcurl
 * global state.
 */
void api_connector_cleanup(void);

/**
 * @brief Clean up an API response and free internal allocations
 *
 * Frees the response body and error message strings. The ApiResponse
 * structure itself is not freed (it may be stack-allocated).
 * Safe to call multiple times or on zero-initialized structures.
 *
 * @param response Pointer to ApiResponse to clean up (can be NULL)
 */
void api_response_cleanup(ApiResponse *response);

/**
 * @brief Fetch cities for a given country from CountriesNow API
 *
 * Makes a POST request to the CountriesNow API to retrieve all cities
 * in the specified country. The response parameter must be initialized
 * (either with api_response_init() or zero-initialized) before calling.
 *
 * Implements:
 * - Timeout handling
 * - Automatic retries with exponential backoff
 * - Rate limit handling (429 status codes)
 * - Network error recovery
 *
 * Example usage:
 * @code
 * api_connector_init();
 *
 * ApiResponse response = API_RESPONSE_INIT;
 * ApiConfig config = API_CONFIG_DEFAULT;
 *
 * int result = api_fetch_cities("San Marino", &config, &response);
 * if (result == 0 && response.http_code == 200) {
 *     // Parse response.body with cities_parse_json()
 * }
 *
 * api_response_cleanup(&response);
 * api_connector_cleanup();
 * @endcode
 *
 * @param country_name Name of the country to fetch cities for
 * @param config Configuration for the request (NULL for defaults)
 * @param response Pointer to initialized ApiResponse to fill with results
 * @return 0 on success, -1 on error (check response.error_message)
 */
int api_fetch_cities(const char *country_name, const ApiConfig *config, ApiResponse *response);

/**
 * @brief Check if an API response indicates success
 *
 * @param response The API response to check
 * @return 1 if response is successful (200 status), 0 otherwise
 */
int api_response_is_success(const ApiResponse *response);

/**
 * @brief Check if an API response indicates a rate limit error
 *
 * @param response The API response to check
 * @return 1 if response is rate limited (429 status), 0 otherwise
 */
int api_response_is_rate_limited(const ApiResponse *response);

#endif /* API_H */
