#include "bst.h"
#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * String comparison function for BST
 * returns 0, if a and b are equal
 * returns negative value if a is less than b
 * returns positive value if a is greater than b
 */
static int compare_strings(const void *a, const void *b) {
    return strcmp((const char *)a, (const char *)b);
}

/**
 * String print function for BST
 */
static void print_string(const void *data) {
    printf("%s\n", (const char *)data);
}

// Test: Create a tree
START_TEST(test_create_tree)
{
    BSTTree *tree = bst_create(compare_strings);
    ck_assert_ptr_nonnull(tree);
    ck_assert_uint_eq(bst_count_nodes(tree), 0);
    bst_delete(tree, NULL);
}
END_TEST

// Test: Create tree with NULL compare
START_TEST(test_create_tree_null)
{
    BSTTree *tree = bst_create(NULL);
    ck_assert_ptr_null(tree);
}
END_TEST

// Test: Insert single city
START_TEST(test_insert_single)
{
    BSTTree *tree = bst_create(compare_strings);
    int result = bst_insert(tree, (void *)"Paris");
    ck_assert_int_eq(result, 0);
    ck_assert_uint_eq(bst_count_nodes(tree), 1);
    
    const void *found = bst_search(tree, (void *)"Paris");
    ck_assert_ptr_nonnull(found);
    ck_assert_str_eq((const char *)found, "Paris");
    bst_delete(tree, NULL);
}
END_TEST

// Test: Insert multiple cities
START_TEST(test_insert_multiple)
{
    BSTTree *tree = bst_create(compare_strings);
    bst_insert(tree, (void *)"London");
    bst_insert(tree, (void *)"Berlin");
    bst_insert(tree, (void *)"Tokyo");
    
    ck_assert_uint_eq(bst_count_nodes(tree), 3);
    
    ck_assert_ptr_nonnull(bst_search(tree, (void *)"London"));
    ck_assert_ptr_nonnull(bst_search(tree, (void *)"Berlin"));
    ck_assert_ptr_nonnull(bst_search(tree, (void *)"Tokyo"));
    
    bst_delete(tree, NULL);
}
END_TEST

// Test: Insert duplicate cities (should not insert)
START_TEST(test_insert_duplicate)
{
    BSTTree *tree = bst_create(compare_strings);
    bst_insert(tree, (void *)"Madrid");
    bst_insert(tree, (void *)"Madrid");
    
    size_t count = bst_count_nodes(tree);
    ck_assert_uint_eq(count, 1);
    
    bst_delete(tree, NULL);
}
END_TEST

// Test: Insert NULL data
START_TEST(test_insert_null)
{
    BSTTree *tree = bst_create(compare_strings);
    bst_insert(tree, (void *)"Rome");
    int result = bst_insert(tree, NULL);
    
    size_t count = bst_count_nodes(tree);
    ck_assert_uint_eq(count, 1);
    ck_assert_int_eq(result, -1);
    
    bst_delete(tree, NULL);
}
END_TEST

// Test: Search for existing city
START_TEST(test_search_found)
{
    BSTTree *tree = bst_create(compare_strings);
    bst_insert(tree, (void *)"Amsterdam");
    bst_insert(tree, (void *)"Brussels");
    bst_insert(tree, (void *)"Copenhagen");
    
    const void *found = bst_search(tree, (void *)"Brussels");
    ck_assert_ptr_nonnull(found);
    ck_assert_str_eq((const char *)found, "Brussels");
    
    bst_delete(tree, NULL);
}
END_TEST

// Test: Search for non-existing city
START_TEST(test_search_not_found)
{
    BSTTree *tree = bst_create(compare_strings);
    bst_insert(tree, (void *)"Vienna");
    bst_insert(tree, (void *)"Prague");
    
    const void *found = bst_search(tree, (void *)"Budapest");
    ck_assert_ptr_null(found);
    
    bst_delete(tree, NULL);
}
END_TEST

// Test: Search in empty tree
START_TEST(test_search_empty)
{
    BSTTree *tree = bst_create(compare_strings);
    const void *found = bst_search(tree, (void *)"Athens");
    ck_assert_ptr_null(found);
    bst_delete(tree, NULL);
}
END_TEST

// Test: Search with NULL data
START_TEST(test_search_null)
{
    BSTTree *tree = bst_create(compare_strings);
    bst_insert(tree, (void *)"Oslo");
    
    const void *found = bst_search(tree, NULL);
    ck_assert_ptr_null(found);
    
    bst_delete(tree, NULL);
}
END_TEST

// Test: Find minimum in tree
START_TEST(test_find_min)
{
    BSTTree *tree = bst_create(compare_strings);
    bst_insert(tree, (void *)"Munich");
    bst_insert(tree, (void *)"Hamburg");
    bst_insert(tree, (void *)"Stuttgart");
    bst_insert(tree, (void *)"Frankfurt");
    
    const void *min = bst_find_min(tree);
    ck_assert_ptr_nonnull(min);
    ck_assert_str_eq((const char *)min, "Frankfurt");
    
    bst_delete(tree, NULL);
}
END_TEST

// Test: Find minimum in empty tree
START_TEST(test_find_min_empty)
{
    BSTTree *tree = bst_create(compare_strings);
    const void *min = bst_find_min(tree);
    ck_assert_ptr_null(min);
    bst_delete(tree, NULL);
}
END_TEST

// Test: Remove leaf node
START_TEST(test_remove_leaf)
{
    BSTTree *tree = bst_create(compare_strings);
    bst_insert(tree, (void *)"Dublin");
    bst_insert(tree, (void *)"Cork");
    bst_insert(tree, (void *)"Galway");
    
    bst_remove(tree, (void *)"Cork");
    
    const void *found = bst_search(tree, (void *)"Cork");
    ck_assert_ptr_null(found);
    ck_assert_uint_eq(bst_count_nodes(tree), 2);
    
    bst_delete(tree, NULL);
}
END_TEST

// Test: Remove node with one child
START_TEST(test_remove_one_child)
{
    BSTTree *tree = bst_create(compare_strings);
    bst_insert(tree, (void *)"Lisbon");
    bst_insert(tree, (void *)"Porto");
    bst_insert(tree, (void *)"Sintra");
    
    bst_remove(tree, (void *)"Porto");
    
    const void *found = bst_search(tree, (void *)"Porto");
    ck_assert_ptr_null(found);
    ck_assert_ptr_nonnull(bst_search(tree, (void *)"Sintra"));
    
    bst_delete(tree, NULL);
}
END_TEST

// Test: Remove node with two children
START_TEST(test_remove_two_children)
{
    BSTTree *tree = bst_create(compare_strings);
    bst_insert(tree, (void *)"Milan");
    bst_insert(tree, (void *)"Florence");
    bst_insert(tree, (void *)"Venice");
    bst_insert(tree, (void *)"Rome");
    
    bst_remove(tree, (void *)"Milan");
    
    const void *found = bst_search(tree, (void *)"Milan");
    ck_assert_ptr_null(found);
    ck_assert_uint_eq(bst_count_nodes(tree), 3);
    ck_assert_ptr_nonnull(bst_search(tree, (void *)"Florence"));
    ck_assert_ptr_nonnull(bst_search(tree, (void *)"Venice"));
    
    bst_delete(tree, NULL);
}
END_TEST

// Test: Remove root node
START_TEST(test_remove_root)
{
    BSTTree *tree = bst_create(compare_strings);
    bst_insert(tree, (void *)"Warsaw");
    bst_remove(tree, (void *)"Warsaw");
    
    ck_assert_uint_eq(bst_count_nodes(tree), 0);
    bst_delete(tree, NULL);
}
END_TEST

// Test: Remove from empty tree
START_TEST(test_remove_empty)
{
    BSTTree *tree = bst_create(compare_strings);
    bst_remove(tree, (void *)"Barcelona");
    ck_assert_uint_eq(bst_count_nodes(tree), 0);
    bst_delete(tree, NULL);
}
END_TEST

// Test: Remove non-existing city
START_TEST(test_remove_not_found)
{
    BSTTree *tree = bst_create(compare_strings);
    bst_insert(tree, (void *)"Helsinki");
    bst_insert(tree, (void *)"Turku");
    
    bst_remove(tree, (void *)"Tampere");
    
    ck_assert_uint_eq(bst_count_nodes(tree), 2);
    
    bst_delete(tree, NULL);
}
END_TEST

// Test: Height of empty tree
START_TEST(test_height_empty)
{
    BSTTree *tree = bst_create(compare_strings);
    int height = bst_height(tree);
    ck_assert_int_eq(height, -1);
    bst_delete(tree, NULL);
}
END_TEST

// Test: Height of single node
START_TEST(test_height_single)
{
    BSTTree *tree = bst_create(compare_strings);
    bst_insert(tree, (void *)"Zurich");
    
    int height = bst_height(tree);
    ck_assert_int_eq(height, 0);
    
    bst_delete(tree, NULL);
}
END_TEST

// Test: Height of balanced tree
START_TEST(test_height_balanced)
{
    BSTTree *tree = bst_create(compare_strings);
    bst_insert(tree, (void *)"Geneva");
    bst_insert(tree, (void *)"Basel");
    bst_insert(tree, (void *)"Lausanne");
    
    int height = bst_height(tree);
    ck_assert_int_eq(height, 1);
    
    bst_delete(tree, NULL);
}
END_TEST

// Test: Height of unbalanced tree
START_TEST(test_height_unbalanced)
{
    BSTTree *tree = bst_create(compare_strings);
    bst_insert(tree, (void *)"A");
    bst_insert(tree, (void *)"B");
    bst_insert(tree, (void *)"C");
    bst_insert(tree, (void *)"D");
    
    int height = bst_height(tree);
    ck_assert_int_eq(height, 3);
    
    bst_delete(tree, NULL);
}
END_TEST

// Test: Count nodes in empty tree
START_TEST(test_count_empty)
{
    BSTTree *tree = bst_create(compare_strings);
    size_t count = bst_count_nodes(tree);
    ck_assert_uint_eq(count, 0);
    bst_delete(tree, NULL);
}
END_TEST

// Test: Count nodes in tree
START_TEST(test_count_nodes)
{
    BSTTree *tree = bst_create(compare_strings);
    bst_insert(tree, (void *)"Lyon");
    bst_insert(tree, (void *)"Marseille");
    bst_insert(tree, (void *)"Nice");
    bst_insert(tree, (void *)"Toulouse");
    bst_insert(tree, (void *)"Nantes");
    
    size_t count = bst_count_nodes(tree);
    ck_assert_uint_eq(count, 5);
    
    bst_delete(tree, NULL);
}
END_TEST

// Test: Complex operations sequence
START_TEST(test_complex_operations)
{
    BSTTree *tree = bst_create(compare_strings);
    
    // Insert cities
    bst_insert(tree, (void *)"Madrid");
    bst_insert(tree, (void *)"Barcelona");
    bst_insert(tree, (void *)"Valencia");
    bst_insert(tree, (void *)"Seville");
    bst_insert(tree, (void *)"Zaragoza");
    
    ck_assert_uint_eq(bst_count_nodes(tree), 5);
    
    // Search for cities
    ck_assert_ptr_nonnull(bst_search(tree, (void *)"Barcelona"));
    ck_assert_ptr_nonnull(bst_search(tree, (void *)"Valencia"));
    
    // Remove a city
    bst_remove(tree, (void *)"Barcelona");
    ck_assert_uint_eq(bst_count_nodes(tree), 4);
    ck_assert_ptr_null(bst_search(tree, (void *)"Barcelona"));
    
    // Insert duplicate
    bst_insert(tree, (void *)"Madrid");
    ck_assert_uint_eq(bst_count_nodes(tree), 4);
    
    // Remove multiple cities
    bst_remove(tree, (void *)"Seville");
    bst_remove(tree, (void *)"Zaragoza");
    ck_assert_uint_eq(bst_count_nodes(tree), 2);
    
    bst_delete(tree, NULL);
}
END_TEST

// Test: Alphabetical ordering
START_TEST(test_alphabetical_order)
{
    BSTTree *tree = bst_create(compare_strings);
    bst_insert(tree, (void *)"Munich");
    bst_insert(tree, (void *)"Berlin");
    bst_insert(tree, (void *)"Hamburg");
    bst_insert(tree, (void *)"Frankfurt");
    bst_insert(tree, (void *)"Cologne");
    
    // Check that Berlin (alphabetically first) is the minimum
    const void *min = bst_find_min(tree);
    ck_assert_str_eq((const char *)min, "Berlin");
    
    // Verify all cities are still accessible
    ck_assert_ptr_nonnull(bst_search(tree, (void *)"Munich"));
    ck_assert_ptr_nonnull(bst_search(tree, (void *)"Hamburg"));
    
    bst_delete(tree, NULL);
}
END_TEST

// Test: Delete tree
START_TEST(test_delete_tree)
{
    BSTTree *tree = bst_create(compare_strings);
    bst_insert(tree, (void *)"Bern");
    bst_insert(tree, (void *)"Geneva");
    bst_insert(tree, (void *)"Basel");
    
    // Delete should free all memory without crashing
    bst_delete(tree, NULL);
    // If we reach here, deletion succeeded
}
END_TEST

// Test: Delete NULL tree
START_TEST(test_delete_null_tree)
{
    // Should not crash
    bst_delete(NULL, NULL);
}
END_TEST

// Suite creation for Check framework
Suite *bst_suite(void)
{
    Suite *s;
    TCase *tc_init, *tc_insert, *tc_search, *tc_remove, *tc_metrics, *tc_complex;

    s = suite_create("BST");

    // Initialization test case
    tc_init = tcase_create("Tree Initialization");
    tcase_add_test(tc_init, test_create_tree);
    tcase_add_test(tc_init, test_create_tree_null);
    suite_add_tcase(s, tc_init);

    // Insert operations test case
    tc_insert = tcase_create("Insert Operations");
    tcase_add_test(tc_insert, test_insert_single);
    tcase_add_test(tc_insert, test_insert_multiple);
    tcase_add_test(tc_insert, test_insert_duplicate);
    tcase_add_test(tc_insert, test_insert_null);
    suite_add_tcase(s, tc_insert);

    // Search operations test case
    tc_search = tcase_create("Search Operations");
    tcase_add_test(tc_search, test_search_found);
    tcase_add_test(tc_search, test_search_not_found);
    tcase_add_test(tc_search, test_search_empty);
    tcase_add_test(tc_search, test_search_null);
    tcase_add_test(tc_search, test_find_min);
    tcase_add_test(tc_search, test_find_min_empty);
    suite_add_tcase(s, tc_search);

    // Remove operations test case
    tc_remove = tcase_create("Remove Operations");
    tcase_add_test(tc_remove, test_remove_leaf);
    tcase_add_test(tc_remove, test_remove_one_child);
    tcase_add_test(tc_remove, test_remove_two_children);
    tcase_add_test(tc_remove, test_remove_root);
    tcase_add_test(tc_remove, test_remove_empty);
    tcase_add_test(tc_remove, test_remove_not_found);
    suite_add_tcase(s, tc_remove);

    // Metrics test case
    tc_metrics = tcase_create("Tree Metrics");
    tcase_add_test(tc_metrics, test_height_empty);
    tcase_add_test(tc_metrics, test_height_single);
    tcase_add_test(tc_metrics, test_height_balanced);
    tcase_add_test(tc_metrics, test_height_unbalanced);
    tcase_add_test(tc_metrics, test_count_empty);
    tcase_add_test(tc_metrics, test_count_nodes);
    suite_add_tcase(s, tc_metrics);

    // Complex operations test case
    tc_complex = tcase_create("Complex Operations");
    tcase_add_test(tc_complex, test_complex_operations);
    tcase_add_test(tc_complex, test_alphabetical_order);
    tcase_add_test(tc_complex, test_delete_tree);
    tcase_add_test(tc_complex, test_delete_null_tree);
    suite_add_tcase(s, tc_complex);

    return s;
}

// Main test runner
int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = bst_suite();
    sr = srunner_create(s);

    // Run tests with verbose output to see each test result
    srunner_run_all(sr, CK_VERBOSE);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
