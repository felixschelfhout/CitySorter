#include "version.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define INPUT_MAX 256
#define CITY_MAX 64
#define COUNTRY_MAX 64

/**
 * print_version - Display the program version
 * @program_name: Name of the program to display in the version string
 */
void print_version(const char *program_name) {
    printf("%s %d.%d.%d\n", program_name, CITYSORTER_VERSION_MAJOR, CITYSORTER_VERSION_MINOR,
           CITYSORTER_VERSION_PATCH);
}

/**
 * print_help - Display usage and available commands
 * @program_name: Name of the program to display in usage information
 */
void print_help(const char *program_name) {
    printf("Usage: %s [OPTIONS]\n", program_name);
    printf("Discover all cities of the countries of your dreams - neatly sorted in a binary search "
           "tree\n\n");
    printf("Options:\n");
    printf("  --version    Display version information\n");
    printf("  --help       Display this help message\n");
    printf("\nInteractive commands:\n");
    printf("  print             Display the BST\n");
    printf("  add [city]         Add a city\n");
    printf("  remove [city]      Remove a city\n");
    printf("  travel-to [country] Fetch cities from a country\n");
    printf("  stop              Exit the program\n");
}

/**
 * flush_stdin_line - Clear remaining characters from stdin buffer
 *
 * Consumes characters until newline or EOF is encountered, used to
 * handle input longer than buffer size.
 */
static void flush_stdin_line(void) {
    int ch = 0;
    while (ch != '\n' && ch != EOF) {
        ch = getchar();
    }
}

/**
 * ltrim - Remove leading whitespace from string
 * @text: String to trim
 *
 * Return: Pointer to first non-whitespace character in string
 */
static char *ltrim(char *text) {
    while (*text != '\0' && isspace((unsigned char)*text)) {
        text++;
    }
    return text;
}

/**
 * rtrim - Remove trailing whitespace from string
 * @text: String to trim (modified in-place)
 *
 * Replaces trailing whitespace characters with null terminator.
 */
static void rtrim(char *text) {
    size_t len = strlen(text);
    while (len > 0 && isspace((unsigned char)text[len - 1])) {
        text[len - 1] = '\0';
        len--;
    }
}

/**
 * is_valid_city - Validate city name format
 * @city: City name to validate
 *
 * Checks if city name contains only alphanumeric characters, spaces,
 * hyphens, or apostrophes, and does not exceed CITY_MAX length.
 *
 * Return: 1 if valid, 0 otherwise
 */
static int is_valid_city(const char *city) {
    size_t length = strlen(city);

    if (length == 0 || length > CITY_MAX) {
        return 0;
    }

    for (size_t i = 0; i < length; ++i) {
        unsigned char ch = (unsigned char)city[i];
        if (!(isalnum(ch) || ch == ' ' || ch == '-' || ch == '\'')) {
            return 0;
        }
    }

    return 1;
}

/**
 * is_valid_country - Validate country name format
 * @country: Country name to validate
 *
 * Checks if country name contains only alphabetic characters, spaces,
 * or hyphens, and does not exceed COUNTRY_MAX length.
 *
 * Return: 1 if valid, 0 otherwise
 */
static int is_valid_country(const char *country) {
    size_t length = strlen(country);

    if (length == 0 || length > COUNTRY_MAX) {
        return 0;
    }

    for (size_t i = 0; i < length; ++i) {
        unsigned char ch = (unsigned char)country[i];
        if (!(isalpha(ch) || ch == ' ' || ch == '-')) {
            return 0;
        }
    }

    return 1;
}

/**
 * handle_stub_print - Stub handler for print command
 */
static void handle_stub_print(void) {
    printf("Not implemented: print BST\n");
}

/**
 * handle_stub_add - Stub handler for add command
 * @city: City name to add (validated by caller)
 */
static void handle_stub_add(const char *city) {
    (void)city;
    printf("Not implemented: add city\n");
}

/**
 * handle_stub_remove - Stub handler for remove command
 * @city: City name to remove (validated by caller)
 */
static void handle_stub_remove(const char *city) {
    (void)city;
    printf("Not implemented: remove city\n");
}

/**
 * handle_stub_travel_to - Stub handler for travel-to command
 * @country: Country name to fetch cities from (validated by caller)
 */
static void handle_stub_travel_to(const char *country) {
    (void)country;
    printf("Not implemented: travel to country\n");
}

/**
 * split_command - Parse input into command and argument
 * @input: Raw user input string (modified in-place)
 * @command: Pointer to store extracted command
 * @arg: Pointer to store extracted argument (NULL if none)
 *
 * Splits input on first whitespace boundary. Trims and null-terminates
 * both command and argument portions of the input string.
 */
static void split_command(char *input, char **command, char **arg) {
    char *cursor = input;

    while (*cursor != '\0' && !isspace((unsigned char)*cursor)) {
        cursor++;
    }

    *command = input;
    *arg = NULL;

    if (*cursor != '\0') {
        *cursor = '\0';
        *arg = ltrim(cursor + 1);
        rtrim(*arg);
    }
}

/**
 * validate_no_arg - Validate that a command has no arguments
 * @command: Command name for error messages
 * @arg: Argument to validate (should be NULL or empty)
 *
 * Return: 1 if no argument present, 0 if argument provided (prints error)
 */
static int validate_no_arg(const char *command, const char *arg) {
    if (arg != NULL && *arg != '\0') {
        printf("Error: '%s' takes no arguments.\n", command);
        return 0;
    }
    return 1;
}

/**
 * validate_city_arg - Validate a city name argument
 * @command: Command name for error messages
 * @arg: City name to validate
 *
 * Return: 1 if valid, 0 otherwise (prints error message)
 */
static int validate_city_arg(const char *command, const char *arg) {
    if (arg == NULL || *arg == '\0') {
        printf("Error: '%s' requires a city name.\n", command);
        return 0;
    }
    if (!is_valid_city(arg)) {
        printf("Error: Invalid city name. Use letters, numbers, spaces, '-' or '\''.\n");
        return 0;
    }
    return 1;
}

/**
 * validate_country_arg - Validate a country name argument
 * @command: Command name for error messages
 * @arg: Country name to validate
 *
 * Return: 1 if valid, 0 otherwise (prints error message)
 */
static int validate_country_arg(const char *command, const char *arg) {
    if (arg == NULL || *arg == '\0') {
        printf("Error: '%s' requires a country name.\n", command);
        return 0;
    }
    if (!is_valid_country(arg)) {
        printf("Error: Invalid country name. Use letters, spaces, or '-'.\n");
        return 0;
    }
    return 1;
}

/**
 * handle_command - Process and execute a user command
 * @command: Command name to execute
 * @arg: Argument for the command (may be NULL)
 *
 * Dispatches command to appropriate handler after validation.
 * Handlers are currently stubbed and print "not implemented" messages.
 *
 * Return: 1 to continue CLI loop, 0 to exit (stop command)
 */
static int handle_command(const char *command, const char *arg) {
    if (strcmp(command, "print") == 0) {
        if (!validate_no_arg("print", arg)) {
            return 1;
        }
        handle_stub_print();
        return 1;
    }

    if (strcmp(command, "add") == 0) {
        if (!validate_city_arg("add", arg)) {
            return 1;
        }
        handle_stub_add(arg);
        return 1;
    }

    if (strcmp(command, "remove") == 0) {
        if (!validate_city_arg("remove", arg)) {
            return 1;
        }
        handle_stub_remove(arg);
        return 1;
    }

    if (strcmp(command, "travel-to") == 0) {
        if (!validate_country_arg("travel-to", arg)) {
            return 1;
        }
        handle_stub_travel_to(arg);
        return 1;
    }

    if (strcmp(command, "stop") == 0) {
        if (!validate_no_arg("stop", arg)) {
            return 1;
        }
        return 0;
    }

    if (strcmp(command, "help") == 0) {
        if (!validate_no_arg("help", arg)) {
            return 1;
        }
        print_help("citysorter");
        return 1;
    }

    if (strcmp(command, "version") == 0) {
        if (!validate_no_arg("version", arg)) {
            return 1;
        }
        print_version("citysorter");
        return 1;
    }

    printf("Error: Unknown command '%s'. Type 'help' for a list of commands.\n", command);
    return 1;
}

/**
 * run_cli_loop - Main interactive command loop
 *
 * Reads user input, parses commands, validates arguments, and invokes
 * handlers. Continues until EOF or stop command is received.
 */
static void run_cli_loop(void) {
    char buffer[INPUT_MAX];

    while (1) {
        printf("citysorter> ");

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("\n");
            break;
        }

        if (strchr(buffer, '\n') == NULL) {
            printf("Error: Input too long (max %d characters).\n", INPUT_MAX - 1);
            flush_stdin_line();
            continue;
        }

        rtrim(buffer);
        char *input = ltrim(buffer);

        if (*input == '\0') {
            continue;
        }

        char *command = NULL;
        char *arg = NULL;
        split_command(input, &command, &arg);
        if (!handle_command(command, arg)) {
            break;
        }
    }
}

/**
 * main - CLI entry point
 * @argc: Argument count
 * @argv: Argument vector
 *
 * Handles command-line options (--version, --help) or enters interactive
 * CLI loop. Prints version information on startup.
 *
 * Return: 0 on success, 1 on error
 */
int main(int argc, char *argv[]) {
    if (argc > 1) {
        if (strcmp(argv[1], "--version") == 0) {
            print_version(argv[0]);
            return 0;
        }
        if (strcmp(argv[1], "--help") == 0) {
            print_help(argv[0]);
            return 0;
        }
        fprintf(stderr, "Error: Unknown option '%s'\n", argv[1]);
        fprintf(stderr, "Try '%s --help' for more information.\n", argv[0]);
        return 1;
    }

    printf("%s %d.%d.%d\n", argv[0], CITYSORTER_VERSION_MAJOR, CITYSORTER_VERSION_MINOR,
           CITYSORTER_VERSION_PATCH);
    run_cli_loop();

    return 0;
}
