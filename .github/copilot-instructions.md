# Work environment
- Use git worktrees to run agents in background.
- Use CMake to auto-identify a toolchain for the host system, and to build the project.

# Architecture
- User interface: CLI with input sanitization
- Connectors: Connects to external APIs, fetches data, and processes it. Handles API rate limits and retries.
- data models: parse JSON data into structures. Includes validation and error handling. 
- Core logic: Implements the main functionality of the application, such as sorting cities. Uses data models to process and manipulate data fetched by connectors.

# Testing
- Unit tests: Test individual components in isolation. Use mocking to simulate external dependencies.
- Integration tests: Test the interaction between components. Use real API calls with a test environment or
mocked responses to ensure the system works as a whole.
- End-to-end tests: Simulate user interactions with the CLI. Use a testing framework to automate the process and verify the expected outcomes.

# Commit messages
- Use the following format for commit messages:
```feat|fix([module]): Short description

Detailed description (if necessary)```
```
- Examples:
  - `feat(api): Add support for new endpoint`
  - `fix(cli): Handle edge case for empty input`
- Use the `feat` prefix for new features and the `fix` prefix for bug fixes.