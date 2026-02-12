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
make

# Run the program
./citysorter
```

## Project Structure

```
TODO
```

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
