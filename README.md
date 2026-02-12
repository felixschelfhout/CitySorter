# CitySorter

An interactive command-line that looks up all cities of your favourite countries andand organizes it using a Binary Search Tree (BST) data structure. Depends on public API: [CountriesNow API](https://countriesnow.space/)

## Usage
  - `print` - Display the BST in a rotated format (right → root → left)
  - `add [city]` - Add a new city to the BST
  - `remove [city]` - Remove a city from the BST
  - `stop` - Exit the program

## Requirements
- CMake (version 3.10 or higher)
- GCC compiler
- curl dynamic library for development (for HTTP requests)
- cJSON dynamic library for development (for JSON parsing)
- POSIX-compliant system (Linux/Unix/macOS)

## Installation

```bash
sudo apt-get update
sudo apt-get install build-essential cmake libcurl4-openssl-dev libcjson-dev
```

## Building

```bash
# Create build directory
mkdir build
cd build

# Configure and build
cmake ..
make citysorter
```

# Run the program
./citysorter
```

## Testing

### Building and Running Tests

```bash
# From the project root directory
cd build
make test

# Or run through CTest for integration with CI/CD
cd build
ctest --verbose
```

## Project Structure

The project follows a modular architecture with clear separation of concerns:

```
CitySorter/
├── include/              # Public header files
│   ├── bst.h            # BST interface
│   └── README.md        # Header documentation
├── src/                 # Source code
│   ├── cli/            # User Interface Layer
│   │   └── main.c      # CLI implementation
│   ├── connectors/     # API Integration Layer
│   │   └── README.md   # Connector documentation
│   ├── models/         # Data Models Layer
│   │   └── README.md   # Model documentation
│   └── core/           # Core Business Logic
│       └── bst.c       # BST implementation
├── tests/              # Test suite
│   ├── unit/          # Unit tests
│   │   └── test_bst.c # BST unit tests (28 tests)
│   ├── integration/   # Integration tests
│   └── e2e/           # End-to-end tests
├── build/             # Build artifacts (generated)
├── CMakeLists.txt     # CMake build configuration
└── README.md          # This file
```

### Architecture Layers

1. **CLI Layer** (`src/cli/`): Handles user interaction and input sanitization
2. **Connectors Layer** (`src/connectors/`): Manages external API communication with rate limiting and retry logic
3. **Models Layer** (`src/models/`): Defines data structures and handles JSON parsing/validation
4. **Core Layer** (`src/core/`): Implements core algorithms and business logic (BST operations)

Each layer has its own README with detailed documentation.

## Implementation Details

### Libraries Used

- **libcurl**: For making HTTP requests to the CountriesNow API
- **cJSON**: For parsing JSON responses and extracting city data
- **Standard C Library**: For memory management, I/O, and string operations

### Data Structure

The program uses a Binary Search Tree (BST) to store city names. Each node contains:
- City name (string)
- Left child pointer
- Right child pointer

Cities are inserted and searched using lexicographic (alphabetical) comparison.

### State Machine

The command processor implements a simple state machine that:
1. Waits for user input
2. Parses the command
3. Executes the corresponding operation
4. Returns to waiting state

## Known Issues & Future Improvements

_This section will be updated as the project develops._

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Author

Felix

## Acknowledgments

- [CountriesNow API](https://countriesnow.space/) for providing the city data
