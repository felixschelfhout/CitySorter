# Models Module

Data Models Layer - Data structures and JSON parsing.

## Responsibilities

- Define data structures for cities and countries
- Parse JSON responses from APIs
- Validate data integrity
- Handle data serialization/deserialization
- Provide data accessors and utilities

## Data Structures

### City
Represents a city entity with relevant attributes.

### Country
Represents a country with associated cities.

### API Response Models
Structures that match API response formats.

## JSON Processing

- Use cJSON library for parsing
- Validate JSON schema
- Handle missing or malformed data
- Provide error messages for invalid data

## Validation Rules

- City names: Non-empty strings, valid characters
- Country codes: ISO standard formats
- Data completeness checks
- Type validation
