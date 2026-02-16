# CLI Module

User Interface Layer - Command-Line Interface implementation.

## Responsibilities

- Accept and sanitize user input
- Parse commands and arguments
- Display output and error messages
- Implement interactive shell loop
- Handle user session management

## Current Files

- `main.c` - Main entry point and CLI loop

## Expected Commands

- `print` - Display the BST structure
- `add [city]` - Add a city to the BST
- `remove [city]` - Remove a city from the BST
- `stop` - Exit the program

## Input Sanitization

All user input should be validated and sanitized before processing to prevent:
- Buffer overflows
- Injection attacks
- Invalid characters
- Excessive input lengths
