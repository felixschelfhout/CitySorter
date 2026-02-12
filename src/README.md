# Source Code

This directory contains all the source code for the CitySorter application, organized by architectural layer.

## Directory Structure

```
src/
├── cli/         # User Interface Layer
├── connectors/  # External API Integration Layer
├── models/      # Data Models and Structures
└── core/        # Core Business Logic
```

### cli/
Contains the command-line interface implementation with input sanitization.
- Handles user input/output
- Command parsing and validation
- Interactive shell implementation

### connectors/
Handles external API communication.
- API client implementations
- HTTP request/response handling
- Rate limiting and retry logic
- Error handling for network operations

### models/
Data structures and models for the application.
- JSON parsing and validation
- Data structure definitions
- Type conversions and serialization

### core/
Core application logic and algorithms.
- Binary Search Tree implementation
- City sorting and management
- Business logic operations
