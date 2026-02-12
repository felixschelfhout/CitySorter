# Tests

Comprehensive test suite for the CitySorter application.

## Directory Structure

```
tests/
├── unit/         # Unit tests for individual components
├── integration/  # Integration tests for component interaction
└── e2e/          # End-to-end tests for complete workflows
```

## Unit Tests

Test individual components in isolation.
- Use mocking to simulate external dependencies
- Focus on single function/module behavior
- Fast execution
- No external dependencies (network, files, etc.)

## Integration Tests

Test interaction between multiple components.
- Use real API calls with test environment or mocked responses
- Verify data flow between modules
- Test error propagation
- Validate integration points

**Future:**
- Test CLI ↔ Core integration
- Test Connectors ↔ Models integration
- Test Core ↔ Models integration

## End-to-End Tests

Simulate complete user workflows.
- Test full application behavior
- Simulate user interactions with CLI
- Verify complete use cases
- Test error scenarios from user perspective

**Future:**
- Test complete workflow: fetch cities → add to BST → display
- Test error scenarios: invalid input, network failures
- Test edge cases: empty results, large datasets

## Running Tests

```bash
# Build all tests
cmake -B build
cmake --build build

# Run unit tests
./build/tests/unit/test_bst

# Run all tests via CTest
cd build
ctest --verbose

# Run specific test suite
ctest -R "unit"
ctest -R "integration"
ctest -R "e2e"
```

## Test Coverage

Use code coverage tools to ensure comprehensive testing:
```bash
# Enable coverage in CMake
cmake -B build -DCMAKE_BUILD_TYPE=Debug -DENABLE_COVERAGE=ON
cmake --build build
ctest
# Generate coverage report (requires gcov/lcov)
```
