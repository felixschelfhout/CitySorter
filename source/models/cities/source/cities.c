#include "cities.h"
#include <cjson/cJSON.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Check if a character is valid in a city name
 * Valid characters: alphanumeric, space, hyphen, apostrophe
 */
static int is_valid_city_char(unsigned char c) {
    return isalnum(c) || c == ' ' || c == '-' || c == '\'';
}

/**
 * @brief Set an error message in the CitiesList structure
 */
static void set_error(CitiesList *cities, size_t size, const char *format, const char *arg) {
    cities->error_message = (char *)malloc(size);
    if (cities->error_message) {
        snprintf(cities->error_message, size, format, arg);
    }
}

/**
 * @brief Validate the error field in JSON
 * Returns 1 if error field is valid and false, 0 otherwise
 */
static int validate_error_field(cJSON *root, cJSON *error, CitiesList *cities) {
    if (!error || !cJSON_IsBool(error)) {
        set_error(cities, 50, "%s", "Missing or invalid 'error' field");
        return 0;
    }
    if (error->type == cJSON_True) {
        cJSON *msg = cJSON_GetObjectItemCaseSensitive(root, "msg");
        if (msg && cJSON_IsString(msg)) {
            set_error(cities, 300, "API error: %s", msg->valuestring);
        } else {
            set_error(cities, 300, "%s", "API error: Unknown");
        }
        return 0;
    }
    return 1;
}

/**
 * @brief Validate the msg field in JSON (optional)
 * Returns 1 if valid or missing, 0 otherwise
 */
static int validate_msg_field(cJSON *msg, CitiesList *cities) {
    if (msg && !cJSON_IsString(msg)) {
        set_error(cities, 50, "%s", "Invalid 'msg' field type");
        return 0;
    }
    return 1;
}

/**
 * @brief Validate the data array field in JSON
 * Returns pointer to data array if valid, NULL otherwise
 */
static cJSON *validate_data_field(cJSON *root, CitiesList *cities) {
    cJSON *data = cJSON_GetObjectItemCaseSensitive(root, "data");
    if (!data || !cJSON_IsArray(data)) {
        set_error(cities, 50, "%s", "Missing or invalid 'data' field");
        return NULL;
    }
    return data;
}

/**
 * @brief Validate all cities in the data array
 * Returns 1 if all valid, 0 otherwise
 */
static int validate_all_cities(cJSON *data, CitiesList *cities) {
    cJSON *item = data->child;
    while (item) {
        if (!cJSON_IsString(item)) {
            set_error(cities, 100, "%s", "Data array contains non-string element");
            return 0;
        }

        const char *city_name = item->valuestring;
        if (!cities_validate_city_name(city_name)) {
            set_error(cities, 300, "Invalid city name: %s", city_name);
            return 0;
        }

        item = item->next;
    }
    return 1;
}

/**
 * @brief Add all cities from the data array
 * Returns 1 on success, 0 on failure
 */
static int add_all_cities(const cJSON *data, CitiesList *cities) {
    const cJSON *item = data->child;
    while (item) {
        const char *city_name = item->valuestring;
        if (cities_add(cities, city_name) != 0) {
            set_error(cities, 50, "%s", "Failed to add city to list");
            return 0;
        }
        item = item->next;
    }
    return 1;
}

CitiesList *cities_create(void) {
    CitiesList *cities = (CitiesList *)malloc(sizeof(CitiesList));
    if (!cities) {
        return NULL;
    }
    cities->cities = NULL;
    cities->count = 0;
    cities->error_message = NULL;
    return cities;
}

void cities_free(CitiesList *cities) {
    if (!cities) {
        return;
    }
    if (cities->cities) {
        for (size_t i = 0; i < cities->count; i++) {
            free(cities->cities[i]);
        }
        free((void *)cities->cities);
    }
    free(cities->error_message);
    free(cities);
}

int cities_validate_city_name(const char *city_name) {
    if (!city_name) {
        return 0;
    }

    size_t len = strlen(city_name);
    if (len == 0 || len > 255) {
        return 0;
    }

    for (size_t i = 0; i < len; i++) {
        if (!is_valid_city_char((unsigned char)city_name[i])) {
            return 0;
        }
    }

    return 1;
}

int cities_add(CitiesList *cities, const char *city_name) {
    if (!cities || !city_name) {
        return -1;
    }

    if (!cities_validate_city_name(city_name)) {
        return -1;
    }

    char **new_cities =
        (char **)realloc((void *)cities->cities, (cities->count + 1) * sizeof(char *));
    if (!new_cities) {
        return -1;
    }

    cities->cities = new_cities;
    cities->cities[cities->count] = (char *)malloc(strlen(city_name) + 1);
    if (!cities->cities[cities->count]) {
        return -1;
    }

    memcpy(cities->cities[cities->count], city_name, strlen(city_name) + 1);
    cities->count++;
    return 0;
}

const char *cities_get(CitiesList *cities, size_t index) {
    if (!cities || index >= cities->count) {
        return NULL;
    }
    return cities->cities[index];
}

CitiesList *cities_parse_json(const char *json_string, const char *country_name) {
    CitiesList *cities = cities_create();
    if (!cities) {
        return NULL;
    }

    if (!json_string) {
        set_error(cities, 50, "%s", "JSON string is NULL");
        return cities;
    }

    // Parse the JSON
    cJSON *root = cJSON_Parse(json_string);
    if (!root) {
        set_error(cities, 100, "Failed to parse JSON: %s",
                  cJSON_GetErrorPtr() ? cJSON_GetErrorPtr() : "Unknown error");
        return cities;
    }

    // Validate error field
    cJSON *error = cJSON_GetObjectItemCaseSensitive(root, "error");
    if (!validate_error_field(root, error, cities)) {
        cJSON_Delete(root);
        return cities;
    }

    // Validate optional msg field
    cJSON *msg = cJSON_GetObjectItemCaseSensitive(root, "msg");
    if (!validate_msg_field(msg, cities)) {
        cJSON_Delete(root);
        return cities;
    }

    // Validate data field
    cJSON *data = validate_data_field(root, cities);
    if (!data) {
        cJSON_Delete(root);
        return cities;
    }

    // Validate all cities
    if (!validate_all_cities(data, cities)) {
        cJSON_Delete(root);
        return cities;
    }

    // Add all cities (we know they are all valid)
    if (!add_all_cities(data, cities)) {
        cJSON_Delete(root);
        return cities;
    }

    cJSON_Delete(root);
    return cities;
}
