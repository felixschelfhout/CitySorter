#include "cities.h"
#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Test: Create cities list
START_TEST(test_create_cities_list) {
    CitiesList *cities = cities_create();
    ck_assert_ptr_nonnull(cities);
    ck_assert_uint_eq(cities->count, 0);
    ck_assert_ptr_null(cities->cities);
    ck_assert_ptr_null(cities->error_message);
    cities_free(cities);
}
END_TEST

// Test: Free cities list
START_TEST(test_free_cities_list) {
    CitiesList *cities = cities_create();
    cities_add(cities, "Paris");
    cities_add(cities, "London");
    ck_assert_uint_eq(cities->count, 2);
    cities_free(cities);
    // Should not crash
}
END_TEST

// Test: Free NULL cities list
START_TEST(test_free_null_cities) {
    cities_free(NULL);
    // Should not crash
}
END_TEST

// Test: Validate valid city names
START_TEST(test_validate_valid_city_names) {
    ck_assert_int_eq(cities_validate_city_name("Paris"), 1);
    ck_assert_int_eq(cities_validate_city_name("New York"), 1);
    ck_assert_int_eq(cities_validate_city_name("São Paulo"), 0); // Special char
    ck_assert_int_eq(cities_validate_city_name("Saint-Tropez"), 1);
    ck_assert_int_eq(cities_validate_city_name("O'Fallon"), 1);
}
END_TEST

// Test: Validate invalid city names
START_TEST(test_validate_invalid_city_names) {
    ck_assert_int_eq(cities_validate_city_name(NULL), 0);
    ck_assert_int_eq(cities_validate_city_name(""), 0);
    ck_assert_int_eq(cities_validate_city_name("City@Name"), 0);  // Invalid char
    ck_assert_int_eq(cities_validate_city_name("City#Name"), 0);  // Invalid char
    ck_assert_int_eq(cities_validate_city_name("City\nName"), 0); // Newline
    ck_assert_int_eq(cities_validate_city_name("City\tName"), 0); // Tab
}
END_TEST

// Test: Validate city name length limits
START_TEST(test_validate_city_name_length) {
    char long_name[300];
    memset(long_name, 'A', 256);
    long_name[256] = '\0';
    ck_assert_int_eq(cities_validate_city_name(long_name), 0); // Too long (> 256)

    memset(long_name, 'A', 255);
    long_name[255] = '\0';
    ck_assert_int_eq(cities_validate_city_name(long_name), 1); // Exactly 256 chars (index 0-255)
}
END_TEST

// Test: Add city to list
START_TEST(test_add_city) {
    CitiesList *cities = cities_create();
    int result = cities_add(cities, "Berlin");
    ck_assert_int_eq(result, 0);
    ck_assert_uint_eq(cities->count, 1);
    ck_assert_str_eq(cities->cities[0], "Berlin");
    cities_free(cities);
}
END_TEST

// Test: Add multiple cities
START_TEST(test_add_multiple_cities) {
    CitiesList *cities = cities_create();
    cities_add(cities, "Rome");
    cities_add(cities, "Venice");
    cities_add(cities, "Milan");
    ck_assert_uint_eq(cities->count, 3);
    ck_assert_str_eq(cities->cities[0], "Rome");
    ck_assert_str_eq(cities->cities[1], "Venice");
    ck_assert_str_eq(cities->cities[2], "Milan");
    cities_free(cities);
}
END_TEST

// Test: Add invalid city
START_TEST(test_add_invalid_city) {
    CitiesList *cities = cities_create();
    int result = cities_add(cities, "Invalid@City");
    ck_assert_int_eq(result, -1);
    ck_assert_uint_eq(cities->count, 0);
    cities_free(cities);
}
END_TEST

// Test: Add NULL city
START_TEST(test_add_null_city) {
    CitiesList *cities = cities_create();
    int result = cities_add(cities, NULL);
    ck_assert_int_eq(result, -1);
    ck_assert_uint_eq(cities->count, 0);
    cities_free(cities);
}
END_TEST

// Test: Get city by index
START_TEST(test_get_city) {
    CitiesList *cities = cities_create();
    cities_add(cities, "Athens");
    cities_add(cities, "Barcelona");
    cities_add(cities, "Dublin");

    const char *city = cities_get(cities, 0);
    ck_assert_ptr_nonnull(city);
    ck_assert_str_eq(city, "Athens");

    city = cities_get(cities, 1);
    ck_assert_str_eq(city, "Barcelona");

    city = cities_get(cities, 2);
    ck_assert_str_eq(city, "Dublin");

    cities_free(cities);
}
END_TEST

// Test: Get city with out of bounds index
START_TEST(test_get_city_out_of_bounds) {
    CitiesList *cities = cities_create();
    cities_add(cities, "Oslo");

    const char *city = cities_get(cities, 10);
    ck_assert_ptr_null(city);

    cities_free(cities);
}
END_TEST

// Test: Get city from NULL list
START_TEST(test_get_city_null_list) {
    const char *city = cities_get(NULL, 0);
    ck_assert_ptr_null(city);
}
END_TEST

// Test: Parse valid JSON response
START_TEST(test_parse_valid_json) {
    const char *json = "{"
                       "\"error\": false, "
                       "\"msg\": \"cities in portugal retrieved\", "
                       "\"data\": [\"Lisbon\", \"Porto\", \"Braga\"]"
                       "}";

    CitiesList *cities = cities_parse_json(json, "portugal");
    ck_assert_ptr_nonnull(cities);
    ck_assert_uint_eq(cities->count, 3);
    ck_assert_ptr_null(cities->error_message);
    ck_assert_str_eq(cities->cities[0], "Lisbon");
    ck_assert_str_eq(cities->cities[1], "Porto");
    ck_assert_str_eq(cities->cities[2], "Braga");
    cities_free(cities);
}
END_TEST

// Test: Parse empty JSON data array
START_TEST(test_parse_empty_json_data_array) {
    const char *json = "{"
                       "\"error\": false, "
                       "\"msg\": \"cities in country retrieved\", "
                       "\"data\": []"
                       "}";

    CitiesList *cities = cities_parse_json(json, "country");
    ck_assert_ptr_nonnull(cities);
    ck_assert_uint_eq(cities->count, 0);
    ck_assert_ptr_null(cities->error_message);
    cities_free(cities);
}
END_TEST

// Test: Parse JSON with single city
START_TEST(test_parse_json_single_city) {
    const char *json = "{"
                       "\"error\": false, "
                       "\"msg\": \"cities in andorra retrieved\", "
                       "\"data\": [\"Andorra la Vella\"]"
                       "}";

    CitiesList *cities = cities_parse_json(json, "andorra");
    ck_assert_ptr_nonnull(cities);
    ck_assert_uint_eq(cities->count, 1);
    ck_assert_ptr_null(cities->error_message);
    ck_assert_str_eq(cities->cities[0], "Andorra la Vella");
    cities_free(cities);
}
END_TEST

// Test: Parse JSON with cities containing hyphens and apostrophes
START_TEST(test_parse_json_special_names) {
    const char *json = "{"
                       "\"error\": false, "
                       "\"msg\": \"cities in country retrieved\", "
                       "\"data\": [\"Saint-Tropez\", \"O'Fallon\", \"New York\"]"
                       "}";

    CitiesList *cities = cities_parse_json(json, "country");
    ck_assert_ptr_nonnull(cities);
    ck_assert_uint_eq(cities->count, 3);
    ck_assert_ptr_null(cities->error_message);
    ck_assert_str_eq(cities->cities[0], "Saint-Tropez");
    ck_assert_str_eq(cities->cities[1], "O'Fallon");
    ck_assert_str_eq(cities->cities[2], "New York");
    cities_free(cities);
}
END_TEST

// Test: Parse NULL JSON string
START_TEST(test_parse_null_json) {
    CitiesList *cities = cities_parse_json(NULL, "country");
    ck_assert_ptr_nonnull(cities);
    ck_assert_uint_eq(cities->count, 0);
    ck_assert_ptr_nonnull(cities->error_message);
    cities_free(cities);
}
END_TEST

// Test: Parse invalid JSON
START_TEST(test_parse_invalid_json) {
    const char *json = "{ invalid json }";
    CitiesList *cities = cities_parse_json(json, "country");
    ck_assert_ptr_nonnull(cities);
    ck_assert_uint_eq(cities->count, 0);
    ck_assert_ptr_nonnull(cities->error_message);
    cities_free(cities);
}
END_TEST

// Test: Parse JSON missing error field
START_TEST(test_parse_json_missing_error_field) {
    const char *json = "{"
                       "\"msg\": \"cities retrieved\", "
                       "\"data\": [\"City1\"]"
                       "}";

    CitiesList *cities = cities_parse_json(json, "country");
    ck_assert_ptr_nonnull(cities);
    ck_assert_uint_eq(cities->count, 0);
    ck_assert_ptr_nonnull(cities->error_message);
    cities_free(cities);
}
END_TEST

// Test: Parse JSON with error field as string (invalid type)
START_TEST(test_parse_json_error_field_wrong_type) {
    const char *json = "{"
                       "\"error\": \"false\", "
                       "\"msg\": \"cities retrieved\", "
                       "\"data\": [\"City1\"]"
                       "}";

    CitiesList *cities = cities_parse_json(json, "country");
    ck_assert_ptr_nonnull(cities);
    ck_assert_uint_eq(cities->count, 0);
    ck_assert_ptr_nonnull(cities->error_message);
    cities_free(cities);
}
END_TEST

// Test: Parse JSON with error field as true
START_TEST(test_parse_json_error_true) {
    const char *json = "{"
                       "\"error\": true, "
                       "\"msg\": \"API error message\", "
                       "\"data\": []"
                       "}";

    CitiesList *cities = cities_parse_json(json, "country");
    ck_assert_ptr_nonnull(cities);
    ck_assert_uint_eq(cities->count, 0);
    ck_assert_ptr_nonnull(cities->error_message);
    cities_free(cities);
}
END_TEST

// Test: Parse JSON missing data field
START_TEST(test_parse_json_missing_data_field) {
    const char *json = "{"
                       "\"error\": false, "
                       "\"msg\": \"cities retrieved\""
                       "}";

    CitiesList *cities = cities_parse_json(json, "country");
    ck_assert_ptr_nonnull(cities);
    ck_assert_uint_eq(cities->count, 0);
    ck_assert_ptr_nonnull(cities->error_message);
    cities_free(cities);
}
END_TEST

// Test: Parse JSON with data field as object instead of array
START_TEST(test_parse_json_data_not_array) {
    const char *json = "{"
                       "\"error\": false, "
                       "\"msg\": \"cities retrieved\", "
                       "\"data\": {\"city\": \"Paris\"}"
                       "}";

    CitiesList *cities = cities_parse_json(json, "country");
    ck_assert_ptr_nonnull(cities);
    ck_assert_uint_eq(cities->count, 0);
    ck_assert_ptr_nonnull(cities->error_message);
    cities_free(cities);
}
END_TEST

// Test: Parse JSON with non-string city in array
START_TEST(test_parse_json_non_string_city) {
    const char *json = "{"
                       "\"error\": false, "
                       "\"msg\": \"cities retrieved\", "
                       "\"data\": [\"Paris\", 123, \"London\"]"
                       "}";

    CitiesList *cities = cities_parse_json(json, "country");
    ck_assert_ptr_nonnull(cities);
    ck_assert_uint_eq(cities->count, 0);
    ck_assert_ptr_nonnull(cities->error_message);
    cities_free(cities);
}
END_TEST

// Test: Parse JSON with invalid city name in array
START_TEST(test_parse_json_invalid_city_name) {
    const char *json = "{"
                       "\"error\": false, "
                       "\"msg\": \"cities retrieved\", "
                       "\"data\": [\"Paris\", \"Invalid@City\", \"London\"]"
                       "}";

    CitiesList *cities = cities_parse_json(json, "country");
    ck_assert_ptr_nonnull(cities);
    ck_assert_uint_eq(cities->count, 0);
    ck_assert_ptr_nonnull(cities->error_message);
    cities_free(cities);
}
END_TEST

// Test: Parse JSON with optional msg field as non-string
START_TEST(test_parse_json_msg_wrong_type) {
    const char *json = "{"
                       "\"error\": false, "
                       "\"msg\": 123, "
                       "\"data\": [\"City1\"]"
                       "}";

    CitiesList *cities = cities_parse_json(json, "country");
    ck_assert_ptr_nonnull(cities);
    ck_assert_uint_eq(cities->count, 0);
    ck_assert_ptr_nonnull(cities->error_message);
    cities_free(cities);
}
END_TEST

// Test: Parse real-world-like response
START_TEST(test_parse_real_world_response) {
    const char *json = "{"
                       "\"error\": false, "
                       "\"msg\": \"cities in San Marino retrieved\", "
                       "\"data\": ["
                       "\"Acquaviva\", "
                       "\"Borgo Maggiore\", "
                       "\"Chiesi\", "
                       "\"Citta di San Marino\", "
                       "\"Domagnano\", "
                       "\"Faetano\", "
                       "\"Fiorentino\", "
                       "\"Montegiardino\", "
                       "\"San Marino\", "
                       "\"Serravalle\""
                       "]"
                       "}";

    CitiesList *cities = cities_parse_json(json, "San Marino");
    ck_assert_ptr_nonnull(cities);
    ck_assert_uint_eq(cities->count, 10);
    ck_assert_ptr_null(cities->error_message);
    ck_assert_str_eq(cities->cities[0], "Acquaviva");
    ck_assert_str_eq(cities->cities[4], "Domagnano");
    ck_assert_str_eq(cities->cities[9], "Serravalle");
    cities_free(cities);
}
END_TEST

// Suite creation for Check framework
Suite *cities_suite(void) {
    Suite *s = suite_create("Cities");

    TCase *tc_core = tcase_create("Core");
    tcase_add_test(tc_core, test_create_cities_list);
    tcase_add_test(tc_core, test_free_cities_list);
    tcase_add_test(tc_core, test_free_null_cities);
    suite_add_tcase(s, tc_core);

    TCase *tc_validation = tcase_create("Validation");
    tcase_add_test(tc_validation, test_validate_valid_city_names);
    tcase_add_test(tc_validation, test_validate_invalid_city_names);
    tcase_add_test(tc_validation, test_validate_city_name_length);
    suite_add_tcase(s, tc_validation);

    TCase *tc_add = tcase_create("Add");
    tcase_add_test(tc_add, test_add_city);
    tcase_add_test(tc_add, test_add_multiple_cities);
    tcase_add_test(tc_add, test_add_invalid_city);
    tcase_add_test(tc_add, test_add_null_city);
    suite_add_tcase(s, tc_add);

    TCase *tc_get = tcase_create("Get");
    tcase_add_test(tc_get, test_get_city);
    tcase_add_test(tc_get, test_get_city_out_of_bounds);
    tcase_add_test(tc_get, test_get_city_null_list);
    suite_add_tcase(s, tc_get);

    TCase *tc_parse = tcase_create("Parse");
    tcase_add_test(tc_parse, test_parse_valid_json);
    tcase_add_test(tc_parse, test_parse_empty_json_data_array);
    tcase_add_test(tc_parse, test_parse_json_single_city);
    tcase_add_test(tc_parse, test_parse_json_special_names);
    tcase_add_test(tc_parse, test_parse_null_json);
    tcase_add_test(tc_parse, test_parse_invalid_json);
    tcase_add_test(tc_parse, test_parse_json_missing_error_field);
    tcase_add_test(tc_parse, test_parse_json_error_field_wrong_type);
    tcase_add_test(tc_parse, test_parse_json_error_true);
    tcase_add_test(tc_parse, test_parse_json_missing_data_field);
    tcase_add_test(tc_parse, test_parse_json_data_not_array);
    tcase_add_test(tc_parse, test_parse_json_non_string_city);
    tcase_add_test(tc_parse, test_parse_json_invalid_city_name);
    tcase_add_test(tc_parse, test_parse_json_msg_wrong_type);
    tcase_add_test(tc_parse, test_parse_real_world_response);
    suite_add_tcase(s, tc_parse);

    return s;
}

// Main test runner
int main(void) {
    Suite *s = cities_suite();
    SRunner *sr = srunner_create(s);

    // Run with normal forking
    srunner_run_all(sr, CK_NORMAL);
    int number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
