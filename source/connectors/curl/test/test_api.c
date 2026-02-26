#include "api.h"
#include "api_internal.h"
#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Test: Initialize and cleanup connector */
START_TEST(test_connector_init_cleanup) {
    int result = api_connector_init();
    ck_assert_int_eq(result, 0);
    api_connector_cleanup();
    /* Should not crash */
}
END_TEST

/* Test: Get default configuration */
START_TEST(test_config_default) {
    ApiConfig config = api_config_default();
    ck_assert_int_eq(config.timeout_seconds, 10);
    ck_assert_int_eq(config.max_retries, 3);
    ck_assert_int_eq(config.retry_delay_ms, 1000);
    ck_assert_int_eq(config.exponential_backoff, 1);
    ck_assert_int_eq(config.follow_redirects, 1);
    ck_assert_int_eq(config.max_redirect_count, 5);
}
END_TEST

/* Test: Init response with macro */
START_TEST(test_response_init_macro) {
    ApiResponse response = API_RESPONSE_INIT;
    ck_assert_ptr_null(response.body);
    ck_assert_int_eq(response.http_code, 0);
    ck_assert_ptr_null(response.error_message);
    api_response_cleanup(&response);
}
END_TEST

/* Test: Cleanup NULL response */
START_TEST(test_response_cleanup_null) {
    api_response_cleanup(NULL);
    /* Should not crash */
}
END_TEST

/* Test: Zero-initialized response */
START_TEST(test_response_zero_init) {
    ApiResponse response = {0};
    ck_assert_ptr_null(response.body);
    ck_assert_int_eq(response.http_code, 0);
    ck_assert_ptr_null(response.error_message);
    api_response_cleanup(&response); /* Should be safe */
}
END_TEST

/* Test: Validate valid country names */
START_TEST(test_validate_valid_country_names) {
    ck_assert_int_eq(api_validate_country_name("San Marino"), 1);
    ck_assert_int_eq(api_validate_country_name("United States"), 1);
    ck_assert_int_eq(api_validate_country_name("Nigeria"), 1);
    ck_assert_int_eq(api_validate_country_name("Côte d'Ivoire"), 0); /* Special char */
    ck_assert_int_eq(api_validate_country_name("Saint-Martin"), 1);
    ck_assert_int_eq(api_validate_country_name("O'Fallon"), 1);
}
END_TEST

/* Test: Validate invalid country names */
START_TEST(test_validate_invalid_country_names) {
    ck_assert_int_eq(api_validate_country_name(NULL), 0);
    ck_assert_int_eq(api_validate_country_name(""), 0);
    ck_assert_int_eq(api_validate_country_name("Country@Name"), 0);  /* Invalid char */
    ck_assert_int_eq(api_validate_country_name("Country#Name"), 0);  /* Invalid char */
    ck_assert_int_eq(api_validate_country_name("Country\nName"), 0); /* Newline */
    ck_assert_int_eq(api_validate_country_name("Country\tName"), 0); /* Tab */
}
END_TEST

/* Test: Validate country name length limits */
START_TEST(test_validate_country_name_length) {
    char long_name[200];
    memset(long_name, 'A', 129);
    long_name[129] = '\0';
    ck_assert_int_eq(api_validate_country_name(long_name), 0); /* Too long (> 128) */

    memset(long_name, 'A', 128);
    long_name[128] = '\0';
    ck_assert_int_eq(api_validate_country_name(long_name), 1); /* Exactly 128 chars */
}
END_TEST

/* Test: Fetch cities with invalid country name */
START_TEST(test_fetch_invalid_country) {
    api_connector_init();
    ApiResponse response = {0};
    int result = api_fetch_cities("Invalid@Country", NULL, &response);
    ck_assert_int_eq(result, -1);
    ck_assert_ptr_null(response.body);
    ck_assert_ptr_nonnull(response.error_message);
    ck_assert_str_eq(response.error_message, "Invalid country name");
    api_response_cleanup(&response);
    api_connector_cleanup();
}
END_TEST

/* Test: Fetch cities with NULL country name */
START_TEST(test_fetch_null_country) {
    api_connector_init();
    ApiResponse response = {0};
    int result = api_fetch_cities(NULL, NULL, &response);
    ck_assert_int_eq(result, -1);
    ck_assert_ptr_null(response.body);
    ck_assert_ptr_nonnull(response.error_message);
    api_response_cleanup(&response);
    api_connector_cleanup();
}
END_TEST

/* Test: Fetch cities with empty country name */
START_TEST(test_fetch_empty_country) {
    api_connector_init();
    ApiResponse response = {0};
    int result = api_fetch_cities("", NULL, &response);
    ck_assert_int_eq(result, -1);
    ck_assert_ptr_null(response.body);
    ck_assert_ptr_nonnull(response.error_message);
    api_response_cleanup(&response);
    api_connector_cleanup();
}
END_TEST

/* Test: Response success check */
START_TEST(test_response_is_success) {
    ApiResponse response = {0};
    response.http_code = 200;
    ck_assert_int_eq(api_response_is_success(&response), 1);

    response.http_code = 404;
    ck_assert_int_eq(api_response_is_success(&response), 0);

    response.http_code = 429;
    ck_assert_int_eq(api_response_is_success(&response), 0);
}
END_TEST

/* Test: Response rate limit check */
START_TEST(test_response_is_rate_limited) {
    ApiResponse response = {0};
    response.http_code = 429;
    ck_assert_int_eq(api_response_is_rate_limited(&response), 1);

    response.http_code = 200;
    ck_assert_int_eq(api_response_is_rate_limited(&response), 0);

    response.http_code = 404;
    ck_assert_int_eq(api_response_is_rate_limited(&response), 0);
}
END_TEST

#ifdef ENABLE_INTEGRATION_TESTS
/* Test: Fetch cities with valid country (integration test) */
/* NOLINTBEGIN(readability-function-cognitive-complexity) */
START_TEST(test_fetch_valid_country) {
    api_connector_init();

    ApiConfig config = api_config_default();
    config.timeout_seconds = 2;
    config.max_retries = 2;

    ApiResponse response = {0};
    api_fetch_cities("San Marino", &config, &response);

    /* The API might be down or rate limited, so we check for reasonable responses */
    const int is_success = (response.http_code == 200);
    const int is_rate_limited = (response.http_code == 429);
    const int is_network_error = (response.http_code == 0);

    if (is_success) {
        ck_assert_ptr_nonnull(response.body);
        ck_assert_int_gt(strlen(response.body), 0);
        ck_assert_ptr_nonnull(strstr(response.body, "data"));
    }

    if (is_rate_limited || is_network_error) {
        ck_assert_ptr_nonnull(response.error_message);
    }

    api_response_cleanup(&response);
    api_connector_cleanup();
}
/* NOLINTEND(readability-function-cognitive-complexity) */
END_TEST

/* Test: Fetch cities with custom config */
START_TEST(test_fetch_with_custom_config) {
    api_connector_init();

    ApiConfig config;
    config.timeout_seconds = 2;
    config.max_retries = 1;
    config.retry_delay_ms = 500;
    config.exponential_backoff = 0;
    config.follow_redirects = 1;
    config.max_redirect_count = 3;

    ApiResponse response = {0};
    api_fetch_cities("Nigeria", &config, &response);

    /* Should get some response, even if it's an error */
    ck_assert(response.http_code >= 0);

    api_response_cleanup(&response);
    api_connector_cleanup();
}
END_TEST

/* Test: Fetch cities for non-existent country */
START_TEST(test_fetch_nonexistent_country) {
    api_connector_init();

    ApiResponse response = {0};
    ApiConfig config = api_config_default();
    config.timeout_seconds = 2;
    api_fetch_cities("Nonexistentcountry", &config, &response);

    /* API should return some response, possibly with error:true in JSON */
    if (response.http_code == 200) {
        ck_assert_ptr_nonnull(response.body);
    }

    api_response_cleanup(&response);
    api_connector_cleanup();
}
END_TEST
#endif

/* Test Suite Setup */
Suite *api_connector_suite(void) {
    Suite *s;
    TCase *tc_config;
    TCase *tc_response;
    TCase *tc_validation;
    TCase *tc_fetch;

    s = suite_create("ApiConnector");

    /* Configuration tests */
    tc_config = tcase_create("Configuration");
    tcase_add_test(tc_config, test_connector_init_cleanup);
    tcase_add_test(tc_config, test_config_default);
    suite_add_tcase(s, tc_config);

    /* Response management tests */
    tc_response = tcase_create("Response");
    tcase_add_test(tc_response, test_response_init_macro);
    tcase_add_test(tc_response, test_response_cleanup_null);
    tcase_add_test(tc_response, test_response_zero_init);
    tcase_add_test(tc_response, test_response_is_success);
    tcase_add_test(tc_response, test_response_is_rate_limited);
    suite_add_tcase(s, tc_response);

    /* Validation tests */
    tc_validation = tcase_create("Validation");
    tcase_add_test(tc_validation, test_validate_valid_country_names);
    tcase_add_test(tc_validation, test_validate_invalid_country_names);
    tcase_add_test(tc_validation, test_validate_country_name_length);
    suite_add_tcase(s, tc_validation);

    /* Fetch tests */
    tc_fetch = tcase_create("Fetch");
    tcase_add_test(tc_fetch, test_fetch_invalid_country);
    tcase_add_test(tc_fetch, test_fetch_null_country);
    tcase_add_test(tc_fetch, test_fetch_empty_country);
#ifdef ENABLE_INTEGRATION_TESTS
    tcase_add_test(tc_fetch, test_fetch_valid_country);
    tcase_add_test(tc_fetch, test_fetch_with_custom_config);
    tcase_add_test(tc_fetch, test_fetch_nonexistent_country);
    /* Set longer timeout for integration tests that make real API calls */
    tcase_set_timeout(tc_fetch, 10);
#endif
    suite_add_tcase(s, tc_fetch);

    return s;
}

int main(void) {
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = api_connector_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_VERBOSE);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
