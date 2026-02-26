# Connectors Module

External API Integration Layer - Handles communication with external services.

## Responsibilities

- Connect to CountriesNow API
- Fetch city and country data
- Handle HTTP requests and responses
- Implement rate limiting
- Implement retry logic with exponential backoff
- Parse API responses
- Handle network errors gracefully

## API Endpoints

### CountriesNow API
Base URL: `https://countriesnow.space/api/v0.1/`

- `POST /countries/cities` - Get all cities for a country

## Usage Example

```c
#include "api.h"

int main(void) {
    // Initialize the connector
    if (api_connector_init() != 0) {
        fprintf(stderr, "Failed to initialize API connector\n");
        return 1;
    }

    // Configure the request (or use defaults)
    ApiConfig config = API_CONFIG_DEFAULT;
    config.timeout_seconds = 15;

    // Create and initialize response structure
    ApiResponse response = API_RESPONSE_INIT;

    // Fetch cities - symmetric API design
    int result = api_fetch_cities("San Marino", &config, &response);

    if (api_response_is_success(&response)) {
        printf("Success! Response body: %s\n", response.body);
        // Parse response.body with cities_parse_json()
    } else {
        fprintf(stderr, "Error: %s (HTTP %ld)\n", 
                response.error_message ? response.error_message : "Unknown error",
                response.http_code);
    }

    // Clean up response internals (frees body and error_message)
    api_response_cleanup(&response);

    // Clean up connector
    api_connector_cleanup();

    return 0;
}
```

## Implementation Requirements

- Use libcurl for HTTP operations
- Implement connection timeout handling
- Support request cancellation
- Log API requests and responses (debug mode)
- Cache responses when appropriate
- Handle rate limiting (429 status codes)

## Error Handling

- Network connectivity issues
- Timeout errors
- Invalid API responses
- Rate limit exceeded
- API service unavailable

## Design Philosophy

### Symmetric API
The connector uses a symmetric design where the caller manages the response structure lifetime:
- User provides `ApiResponse` (stack or heap allocated)
- `api_fetch_cities()` fills it in
- `api_response_cleanup()` frees internal allocations
- No hidden memory management surprises

### Clean Public API
Testing-specific functions are separated into `api_internal.h`:
- `api_config_default()` - For tests only
- `api_validate_country_name()` - Internal validation

The public API (`api.h`) only exposes what users need:
- Initialization/cleanup
- Main fetch function
- Helper functions for response checking
- Configuration struct with macro initializer

## File Structure

```
source/connectors/
├── include/              # Abstraction layer (public API)
│   ├── api.h            # Main public API
│   └── api_internal.h   # Internal/testing functions
├── curl/                # curl-specific implementation
│   ├── source/
│   │   └── api.c       # curl implementation
│   ├── test/
│   │   └── test_api.c  # Unit tests
│   └── CMakeLists.txt  # Build configuration
└── README.md
```

The abstraction (`api.h`) is implementation-agnostic, while the curl-specific
implementation resides in the `curl/` subdirectory. This allows for future
implementations (e.g., using different HTTP libraries) without changing the
public API.
