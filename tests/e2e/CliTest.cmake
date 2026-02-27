# CTest test configuration for the citysorter CLI application
# This file defines all tests for user-facing CLI interaction testing

enable_testing()

# Command-line option tests
add_test(NAME cli_version_flag COMMAND citysorter --version)
set_tests_properties(cli_version_flag PROPERTIES PASS_REGULAR_EXPRESSION "citysorter ${PROJECT_VERSION}")

add_test(NAME cli_help_flag COMMAND citysorter --help)
set_tests_properties(cli_help_flag PROPERTIES PASS_REGULAR_EXPRESSION "Usage:.*OPTIONS")

add_test(NAME cli_unknown_flag COMMAND citysorter --unknown)
set_tests_properties(cli_unknown_flag PROPERTIES WILL_FAIL TRUE)

# Interactive mode tests - help and version commands
add_test(NAME interactive_help_command COMMAND sh -c "echo 'help' | ${PROJECT_BINARY_DIR}/citysorter")
set_tests_properties(interactive_help_command PROPERTIES PASS_REGULAR_EXPRESSION "Usage:")

add_test(NAME interactive_version_command COMMAND sh -c "echo 'version' | ${PROJECT_BINARY_DIR}/citysorter")
set_tests_properties(interactive_version_command PROPERTIES PASS_REGULAR_EXPRESSION "citysorter")

# Interactive mode tests - add command
add_test(NAME interactive_add_no_argument COMMAND sh -c "echo 'add' | ${PROJECT_BINARY_DIR}/citysorter")
set_tests_properties(interactive_add_no_argument PROPERTIES PASS_REGULAR_EXPRESSION "requires a city name")

add_test(NAME interactive_add_valid_city COMMAND sh -c "echo 'add Paris' | ${PROJECT_BINARY_DIR}/citysorter")
set_tests_properties(interactive_add_valid_city PROPERTIES PASS_REGULAR_EXPRESSION "Added.*Paris" FAIL_REGULAR_EXPRESSION "Error:")

add_test(NAME interactive_add_multi_word_city COMMAND sh -c "echo 'add New York' | ${PROJECT_BINARY_DIR}/citysorter")
set_tests_properties(interactive_add_multi_word_city PROPERTIES PASS_REGULAR_EXPRESSION "Added.*New York" FAIL_REGULAR_EXPRESSION "Error:")

add_test(NAME interactive_add_city_with_hyphen COMMAND sh -c "echo 'add Saint-Jean' | ${PROJECT_BINARY_DIR}/citysorter")
set_tests_properties(interactive_add_city_with_hyphen PROPERTIES PASS_REGULAR_EXPRESSION "Added.*Saint-Jean" FAIL_REGULAR_EXPRESSION "Error:")

add_test(NAME interactive_add_city_with_apostrophe COMMAND sh -c "echo \"add O'Connor\" | ${PROJECT_BINARY_DIR}/citysorter")
set_tests_properties(interactive_add_city_with_apostrophe PROPERTIES PASS_REGULAR_EXPRESSION "Added.*O'Connor" FAIL_REGULAR_EXPRESSION "Error:")

add_test(NAME interactive_add_invalid_city_special_chars COMMAND sh -c "echo 'add City@Name' | ${PROJECT_BINARY_DIR}/citysorter")
set_tests_properties(interactive_add_invalid_city_special_chars PROPERTIES PASS_REGULAR_EXPRESSION "Invalid city name")

# Interactive mode tests - remove command
add_test(NAME interactive_remove_no_argument COMMAND sh -c "echo 'remove' | ${PROJECT_BINARY_DIR}/citysorter")
set_tests_properties(interactive_remove_no_argument PROPERTIES PASS_REGULAR_EXPRESSION "requires a city name")

add_test(NAME interactive_remove_valid_city COMMAND sh -c "echo 'remove Domagnano' | ${PROJECT_BINARY_DIR}/citysorter")
set_tests_properties(interactive_remove_valid_city PROPERTIES PASS_REGULAR_EXPRESSION "Removed Domagnano" FAIL_REGULAR_EXPRESSION "Error:")

# Interactive mode tests - travel-to command
add_test(NAME interactive_travel_to_no_argument COMMAND sh -c "echo 'travel-to' | ${PROJECT_BINARY_DIR}/citysorter")
set_tests_properties(interactive_travel_to_no_argument PROPERTIES PASS_REGULAR_EXPRESSION "requires a country name")

add_test(NAME interactive_travel_to_valid_country COMMAND sh -c "echo 'travel-to France' | ${PROJECT_BINARY_DIR}/citysorter")
set_tests_properties(interactive_travel_to_valid_country PROPERTIES PASS_REGULAR_EXPRESSION "Loaded.*cities.*France" FAIL_REGULAR_EXPRESSION "Error:")

add_test(NAME interactive_travel_to_multi_word_country COMMAND sh -c "echo 'travel-to United States' | ${PROJECT_BINARY_DIR}/citysorter")
set_tests_properties(interactive_travel_to_multi_word_country PROPERTIES PASS_REGULAR_EXPRESSION "Loaded.*cities.*United States" FAIL_REGULAR_EXPRESSION "Error:")

add_test(NAME interactive_travel_to_invalid_country_numbers COMMAND sh -c "echo 'travel-to France123' | ${PROJECT_BINARY_DIR}/citysorter")
set_tests_properties(interactive_travel_to_invalid_country_numbers PROPERTIES PASS_REGULAR_EXPRESSION "Invalid country name")

add_test(NAME interactive_travel_to_invalid_country_special_chars COMMAND sh -c "echo 'travel-to Country@' | ${PROJECT_BINARY_DIR}/citysorter")
set_tests_properties(interactive_travel_to_invalid_country_special_chars PROPERTIES PASS_REGULAR_EXPRESSION "Invalid country name")

# Interactive mode tests - print command
add_test(NAME interactive_print_command COMMAND sh -c "echo 'print' | ${PROJECT_BINARY_DIR}/citysorter")
set_tests_properties(interactive_print_command PROPERTIES PASS_REGULAR_EXPRESSION "San Marino" FAIL_REGULAR_EXPRESSION "Error:")

add_test(NAME interactive_print_with_argument COMMAND sh -c "echo 'print tree' | ${PROJECT_BINARY_DIR}/citysorter")
set_tests_properties(interactive_print_with_argument PROPERTIES PASS_REGULAR_EXPRESSION "takes no arguments")

# Interactive mode tests - stop command
add_test(NAME interactive_stop_command COMMAND sh -c "echo 'stop' | ${PROJECT_BINARY_DIR}/citysorter")

# Interactive mode tests - error handling
add_test(NAME interactive_unknown_command COMMAND sh -c "echo 'unknown' | ${PROJECT_BINARY_DIR}/citysorter")
set_tests_properties(interactive_unknown_command PROPERTIES PASS_REGULAR_EXPRESSION "Unknown command")

add_test(NAME interactive_empty_input COMMAND sh -c "echo '' | ${PROJECT_BINARY_DIR}/citysorter")

# Interactive mode tests - normal startup
add_test(NAME interactive_startup COMMAND sh -c "echo 'stop' | ${PROJECT_BINARY_DIR}/citysorter")
set_tests_properties(interactive_startup PROPERTIES PASS_REGULAR_EXPRESSION "citysorter")

# End-to-end workflow tests
add_test(NAME e2e_startup_loads_san_marino COMMAND sh -c "echo 'stop' | ${PROJECT_BINARY_DIR}/citysorter")
set_tests_properties(e2e_startup_loads_san_marino PROPERTIES PASS_REGULAR_EXPRESSION "Loaded.*cities.*San Marino" FAIL_REGULAR_EXPRESSION "Error:")

add_test(NAME e2e_add_and_print_workflow COMMAND sh -c "printf 'add TestCity\\nprint\\nstop\\n' | ${PROJECT_BINARY_DIR}/citysorter")
set_tests_properties(e2e_add_and_print_workflow PROPERTIES PASS_REGULAR_EXPRESSION "Added.*TestCity" FAIL_REGULAR_EXPRESSION "Error:")

add_test(NAME e2e_travel_changes_country COMMAND sh -c "printf 'travel-to Italy\\nstop\\n' | ${PROJECT_BINARY_DIR}/citysorter")
set_tests_properties(e2e_travel_changes_country PROPERTIES PASS_REGULAR_EXPRESSION "Loaded.*Italy" FAIL_REGULAR_EXPRESSION "Error:")

add_test(NAME e2e_add_remove_same_city COMMAND sh -c "printf 'add TestCity\\nremove TestCity\\nstop\\n' | ${PROJECT_BINARY_DIR}/citysorter")
set_tests_properties(e2e_add_remove_same_city PROPERTIES PASS_REGULAR_EXPRESSION "Added.*TestCity" PASS_REGULAR_EXPRESSION "Removed.*TestCity" FAIL_REGULAR_EXPRESSION "Error:")

add_test(NAME e2e_multiple_commands_sequence COMMAND sh -c "printf 'add Paris\\nadd London\\nprint\\nstop\\n' | ${PROJECT_BINARY_DIR}/citysorter")
set_tests_properties(e2e_multiple_commands_sequence PROPERTIES PASS_REGULAR_EXPRESSION "Paris" PASS_REGULAR_EXPRESSION "London" FAIL_REGULAR_EXPRESSION "Error:")

# Signal handling tests
add_test(NAME signal_graceful_shutdown_sigint COMMAND sh -c "sleep 1 && pkill -INT citysorter || true")
set_tests_properties(signal_graceful_shutdown_sigint PROPERTIES WILL_FAIL FALSE)

add_test(NAME signal_graceful_shutdown_sigterm COMMAND sh -c "sleep 1 && pkill -TERM citysorter || true")
set_tests_properties(signal_graceful_shutdown_sigterm PROPERTIES WILL_FAIL FALSE)
