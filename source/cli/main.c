/* NOLINTBEGIN(bugprone-reserved-identifier) */
#define _POSIX_C_SOURCE 200809L
/* NOLINTEND(bugprone-reserved-identifier) */

#include "api.h"
#include "bst.h"
#include "cities.h"
#include "version.h"
#include <ctype.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_MAX 256
#define CITY_MAX 64
#define COUNTRY_MAX 64
#define DEFAULT_COUNTRY "San Marino"

typedef struct {
    BSTTree *tree;
    CitiesList *store;
    ApiConfig api_config;
} AppState;

static volatile sig_atomic_t g_should_exit = 0;

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
    printf("  print              Display the BST\n");
    printf("  add [city]          Add a city\n");
    printf("  remove [city]       Remove a city\n");
    printf("  travel-to [country] Fetch cities from a country\n");
    printf("  stop               Exit the program\n");
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

static int compare_strings(const void *a, const void *b) {
    return strcmp((const char *)a, (const char *)b);
}

static void print_city_line(const void *data) {
    printf("%s\n", (const char *)data);
}

static void handle_signal(int signal_number) {
    (void)signal_number;
    g_should_exit = 1;
}

static void install_signal_handlers(void) {
    struct sigaction action;
    memset(&action, 0, sizeof(action));
    action.sa_handler = handle_signal;
    sigemptyset(&action.sa_mask);
    sigaction(SIGINT, &action, NULL);
    sigaction(SIGTERM, &action, NULL);
}

static void app_state_cleanup(AppState *state) {
    if (!state) {
        return;
    }

    if (state->tree) {
        bst_delete(state->tree);
        state->tree = NULL;
    }

    if (state->store) {
        cities_free(state->store);
        state->store = NULL;
    }
}

static int app_state_init(AppState *state) {
    if (!state) {
        return -1;
    }

    state->tree = bst_create(compare_strings, print_city_line);
    state->store = cities_create();
    state->api_config = (ApiConfig)API_CONFIG_DEFAULT;

    if (!state->tree || !state->store) {
        app_state_cleanup(state);
        return -1;
    }

    return 0;
}

static int build_tree_from_list(BSTTree **out_tree, CitiesList **out_store, CitiesList *parsed) {
    if (!out_tree || !out_store || !parsed) {
        return -1;
    }

    BSTTree *tree = bst_create(compare_strings, print_city_line);
    CitiesList *store = cities_create();

    if (!tree || !store) {
        if (tree) {
            bst_delete(tree);
        }
        if (store) {
            cities_free(store);
        }
        return -1;
    }

    for (size_t i = 0; i < parsed->count; i++) {
        const char *city_name = cities_get(parsed, i);
        if (!city_name) {
            continue;
        }

        if (bst_search(tree, city_name)) {
            continue;
        }

        if (cities_add(store, city_name) != 0) {
            bst_delete(tree);
            cities_free(store);
            return -1;
        }

        const char *stored_city = cities_get(store, store->count - 1);
        if (!stored_city || bst_insert(tree, stored_city) != 0) {
            bst_delete(tree);
            cities_free(store);
            return -1;
        }
    }

    *out_tree = tree;
    *out_store = store;
    return 0;
}

static int load_cities_for_country(AppState *state, const char *country) {
    ApiResponse response = API_RESPONSE_INIT;

    if (!state || !country) {
        return -1;
    }

    if (api_fetch_cities(country, &state->api_config, &response) != 0) {
        printf("Error: %s\n",
               response.error_message ? response.error_message : "API request failed");
        api_response_cleanup(&response);
        return -1;
    }

    if (!api_response_is_success(&response)) {
        printf("Error: API request failed (HTTP %ld)\n", response.http_code);
        if (response.error_message) {
            printf("Details: %s\n", response.error_message);
        }
        api_response_cleanup(&response);
        return -1;
    }

    CitiesList *parsed = cities_parse_json(response.body, country);
    if (!parsed) {
        printf("Error: Failed to parse API response.\n");
        api_response_cleanup(&response);
        return -1;
    }

    if (parsed->error_message) {
        printf("Error: %s\n", parsed->error_message);
        cities_free(parsed);
        api_response_cleanup(&response);
        return -1;
    }

    BSTTree *new_tree = NULL;
    CitiesList *new_store = NULL;

    if (build_tree_from_list(&new_tree, &new_store, parsed) != 0) {
        printf("Error: Failed to build city tree.\n");
        cities_free(parsed);
        api_response_cleanup(&response);
        return -1;
    }

    cities_free(parsed);
    api_response_cleanup(&response);

    app_state_cleanup(state);
    state->tree = new_tree;
    state->store = new_store;

    printf("Loaded %zu cities for %s.\n", state->store->count, country);
    return 0;
}

static void handle_print(AppState *state) {
    if (!state || !state->tree) {
        printf("(empty)\n");
        return;
    }

    bst_dump_tree(state->tree);
    bst_retrieve_data_high_to_low(state->tree);
}

static void handle_add(AppState *state, const char *city) {
    if (!state) {
        return;
    }

    if (!state->tree || !state->store) {
        if (app_state_init(state) != 0) {
            printf("Error: Failed to initialize city store.\n");
            return;
        }
    }

    if (bst_search(state->tree, city)) {
        printf("Error: City already exists.\n");
        return;
    }

    if (cities_add(state->store, city) != 0) {
        printf("Error: Failed to add city.\n");
        return;
    }

    const char *stored_city = cities_get(state->store, state->store->count - 1);
    if (!stored_city || bst_insert(state->tree, stored_city) != 0) {
        printf("Error: Failed to insert city into tree.\n");
        return;
    }

    printf("Added %s.\n", city);
}

static void handle_remove(AppState *state, const char *city) {
    if (!state || !state->tree || !state->store) {
        printf("Error: City store is empty.\n");
        return;
    }

    const void *found = bst_search(state->tree, city);
    if (!found) {
        printf("Error: City not found.\n");
        return;
    }

    if (!bst_remove(state->tree, found)) {
        printf("Error: Failed to remove city from tree.\n");
        return;
    }

    if (cities_remove(state->store, (const char *)found) != 0) {
        printf("Warning: City removed from tree but not from store.\n");
    } else {
        printf("Removed %s.\n", city);
    }
}

static void handle_travel_to(AppState *state, const char *country) {
    if (load_cities_for_country(state, country) != 0) {
        printf("Travel failed. Keeping current cities.\n");
    }
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
 * @state: Application state
 * @command: Command name to execute
 * @arg: Argument for the command (may be NULL)
 *
 * Dispatches command to appropriate handler after validation.
 *
 * Return: 1 to continue CLI loop, 0 to exit (stop command)
 */
static int handle_command(AppState *state, const char *command, const char *arg) {
    if (strcmp(command, "print") == 0) {
        if (!validate_no_arg("print", arg)) {
            return 1;
        }
        handle_print(state);
        return 1;
    }

    if (strcmp(command, "add") == 0) {
        if (!validate_city_arg("add", arg)) {
            return 1;
        }
        handle_add(state, arg);
        return 1;
    }

    if (strcmp(command, "remove") == 0) {
        if (!validate_city_arg("remove", arg)) {
            return 1;
        }
        handle_remove(state, arg);
        return 1;
    }

    if (strcmp(command, "travel-to") == 0) {
        if (!validate_country_arg("travel-to", arg)) {
            return 1;
        }
        handle_travel_to(state, arg);
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
 * handlers. Continues until EOF, signal, or stop command is received.
 */
static void run_cli_loop(AppState *state) {
    char buffer[INPUT_MAX];

    while (!g_should_exit) {
        printf("citysorter> ");

        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            if (g_should_exit) {
                printf("\n");
                break;
            }
            if (feof(stdin)) {
                printf("\n");
                break;
            }
            if (errno == EINTR) {
                clearerr(stdin);
                continue;
            }
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
        if (!handle_command(state, command, arg)) {
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

    if (api_connector_init() != 0) {
        fprintf(stderr, "Error: Failed to initialize API connector.\n");
        return 1;
    }

    AppState state = {0};
    if (app_state_init(&state) != 0) {
        fprintf(stderr, "Error: Failed to initialize application state.\n");
        api_connector_cleanup();
        return 1;
    }

    install_signal_handlers();

    printf("Welcome to CitySorter %d.%d.%d! Type 'help' for a list of commands.\n",
           CITYSORTER_VERSION_MAJOR, CITYSORTER_VERSION_MINOR, CITYSORTER_VERSION_PATCH);

    if (load_cities_for_country(&state, DEFAULT_COUNTRY) != 0) {
        printf("Warning: Starting with an empty city tree.\n");
    }

    run_cli_loop(&state);

    app_state_cleanup(&state);
    api_connector_cleanup();

    return 0;
}
