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

- `GET /countries/cities` - Get all cities for a country

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
