#ifndef CITIES_H
#define CITIES_H

#include <stddef.h>

/**
 * @brief Structure to hold a list of cities
 * Represents the parsed response from the CountriesNow API
 */
typedef struct {
    char **cities;       /**< Array of city names */
    size_t count;        /**< Number of cities in the array */
    char *error_message; /**< Error message if parsing fails */
} CitiesList;

/**
 * @brief Initialize an empty cities list
 * @return Pointer to a new CitiesList structure, or NULL on error
 */
CitiesList *cities_create(void);

/**
 * @brief Free a cities list and all allocated memory
 * @param cities Pointer to the CitiesList to free
 */
void cities_free(CitiesList *cities);

/**
 * @brief Parse JSON response from CountriesNow API
 * Expected JSON format:
 * {
 *   "error": false,
 *   "msg": "cities in [country] retrieved",
 *   "data": ["City1", "City2", ...]
 * }
 *
 * @param json_string The JSON response from the API
 * @param countries_name The country name (optional, for validation)
 * @return CitiesList with parsed cities, or NULL if parsing fails
 *         The CitiesList contains an error_message on failure
 */
CitiesList *cities_parse_json(const char *json_string, const char *country_name);

/**
 * @brief Validate a city name
 * Rules:
 * - Must not be NULL
 * - Must not be empty string
 * - Must not exceed 256 characters
 * - Should contain only alphanumeric characters, spaces, hyphens, and apostrophes
 *
 * @param city_name The city name to validate
 * @return 1 if valid, 0 if invalid
 */
int cities_validate_city_name(const char *city_name);

/**
 * @brief Add a city to the cities list
 * @param cities Pointer to the CitiesList
 * @param city_name The city name to add
 * @return 0 on success, -1 on error
 */
int cities_add(CitiesList *cities, const char *city_name);

/**
 * @brief Get city by index
 * @param cities Pointer to the CitiesList
 * @param index The index of the city
 * @return The city name, or NULL if index is out of bounds
 */
const char *cities_get(CitiesList *cities, size_t index);

#endif /* CITIES_H */
