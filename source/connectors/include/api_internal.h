#ifndef API_INTERNAL_H
#define API_INTERNAL_H

#include "api.h"

/**
 * @file api_internal.h
 * @brief Internal API functions - used for unit testing
 *
 * These functions are not part of the public API and should only be used
 * for testing purposes.
 */

/**
 * @brief Get default API configuration
 *
 * Returns a default configuration with sensible values:
 * - 10 second timeout
 * - 3 retry attempts
 * - 1000ms initial retry delay
 * - Exponential backoff enabled
 * - Follow redirects enabled (max 5)
 *
 * @return Default configuration structure
 */
ApiConfig api_config_default(void);

/**
 * @brief Validate country name format
 *
 * Checks if the country name is valid for use in API requests:
 * - Not NULL
 * - Not empty
 * - Does not exceed 128 characters
 * - Contains only alphanumeric characters, spaces, hyphens, and apostrophes
 *
 * @param country_name Country name to validate
 * @return 1 if valid, 0 if invalid
 */
int api_validate_country_name(const char *country_name);

#endif /* API_INTERNAL_H */
