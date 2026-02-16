#include "bst.h"
#include <check.h>
#include <stdlib.h>

// Test: Create a single node
START_TEST(test_create_node)
{
    BSTNode *node = bst_create_node("Stockholm");
    ck_assert_ptr_nonnull(node);
    ck_assert_str_eq(node->city, "Stockholm");
    ck_assert_ptr_null(node->left);
    ck_assert_ptr_null(node->right);
    bst_delete_tree(node);
}
END_TEST

// Test: Create node with NULL city
START_TEST(test_create_node_null)
{
    BSTNode *node = bst_create_node(NULL);
    ck_assert_ptr_null(node);
}
END_TEST

// Test: Insert single city
START_TEST(test_insert_single)
{
    BSTNode *root = NULL;
    root = bst_insert(root, "Paris");
    ck_assert_ptr_nonnull(root);
    ck_assert_str_eq(root->city, "Paris");
    bst_delete_tree(root);
}
END_TEST

// Test: Insert multiple cities
START_TEST(test_insert_multiple)
{
    BSTNode *root = NULL;
    root = bst_insert(root, "London");
    root = bst_insert(root, "Berlin");
    root = bst_insert(root, "Tokyo");
    
    ck_assert_ptr_nonnull(root);
    ck_assert_str_eq(root->city, "London");
    ck_assert_ptr_nonnull(root->left);
    ck_assert_str_eq(root->left->city, "Berlin");
    ck_assert_ptr_nonnull(root->right);
    ck_assert_str_eq(root->right->city, "Tokyo");
    
    bst_delete_tree(root);
}
END_TEST

// Test: Insert duplicate cities (should not insert)
START_TEST(test_insert_duplicate)
{
    BSTNode *root = NULL;
    root = bst_insert(root, "Madrid");
    root = bst_insert(root, "Madrid");
    
    size_t count = bst_count_nodes(root);
    ck_assert_uint_eq(count, 1);
    
    bst_delete_tree(root);
}
END_TEST

// Test: Insert NULL city
START_TEST(test_insert_null)
{
    BSTNode *root = NULL;
    root = bst_insert(root, "Rome");
    root = bst_insert(root, NULL);
    
    size_t count = bst_count_nodes(root);
    ck_assert_uint_eq(count, 1);
    
    bst_delete_tree(root);
}
END_TEST

// Test: Search for existing city
START_TEST(test_search_found)
{
    BSTNode *root = NULL;
    root = bst_insert(root, "Amsterdam");
    root = bst_insert(root, "Brussels");
    root = bst_insert(root, "Copenhagen");
    
    BSTNode *found = bst_search(root, "Brussels");
    ck_assert_ptr_nonnull(found);
    ck_assert_str_eq(found->city, "Brussels");
    
    bst_delete_tree(root);
}
END_TEST

// Test: Search for non-existing city
START_TEST(test_search_not_found)
{
    BSTNode *root = NULL;
    root = bst_insert(root, "Vienna");
    root = bst_insert(root, "Prague");
    
    BSTNode *found = bst_search(root, "Budapest");
    ck_assert_ptr_null(found);
    
    bst_delete_tree(root);
}
END_TEST

// Test: Search in empty tree
START_TEST(test_search_empty)
{
    BSTNode *root = NULL;
    BSTNode *found = bst_search(root, "Athens");
    ck_assert_ptr_null(found);
}
END_TEST

// Test: Search with NULL city
START_TEST(test_search_null)
{
    BSTNode *root = NULL;
    root = bst_insert(root, "Oslo");
    
    BSTNode *found = bst_search(root, NULL);
    ck_assert_ptr_null(found);
    
    bst_delete_tree(root);
}
END_TEST

// Test: Find minimum in tree
START_TEST(test_find_min)
{
    BSTNode *root = NULL;
    root = bst_insert(root, "Munich");
    root = bst_insert(root, "Hamburg");
    root = bst_insert(root, "Stuttgart");
    root = bst_insert(root, "Frankfurt");
    
    BSTNode *min = bst_find_min(root);
    ck_assert_ptr_nonnull(min);
    ck_assert_str_eq(min->city, "Frankfurt");
    
    bst_delete_tree(root);
}
END_TEST

// Test: Find minimum in empty tree
START_TEST(test_find_min_empty)
{
    BSTNode *root = NULL;
    BSTNode *min = bst_find_min(root);
    ck_assert_ptr_null(min);
}
END_TEST

// Test: Remove leaf node
START_TEST(test_remove_leaf)
{
    BSTNode *root = NULL;
    root = bst_insert(root, "Dublin");
    root = bst_insert(root, "Cork");
    root = bst_insert(root, "Galway");
    
    root = bst_remove(root, "Cork");
    
    BSTNode *found = bst_search(root, "Cork");
    ck_assert_ptr_null(found);
    ck_assert_uint_eq(bst_count_nodes(root), 2);
    
    bst_delete_tree(root);
}
END_TEST

// Test: Remove node with one child
START_TEST(test_remove_one_child)
{
    BSTNode *root = NULL;
    root = bst_insert(root, "Lisbon");
    root = bst_insert(root, "Porto");
    root = bst_insert(root, "Sintra");
    
    root = bst_remove(root, "Porto");
    
    BSTNode *found = bst_search(root, "Porto");
    ck_assert_ptr_null(found);
    ck_assert_ptr_nonnull(bst_search(root, "Sintra"));
    
    bst_delete_tree(root);
}
END_TEST

// Test: Remove node with two children
START_TEST(test_remove_two_children)
{
    BSTNode *root = NULL;
    root = bst_insert(root, "Milan");
    root = bst_insert(root, "Florence");
    root = bst_insert(root, "Venice");
    root = bst_insert(root, "Rome");
    
    root = bst_remove(root, "Milan");
    
    BSTNode *found = bst_search(root, "Milan");
    ck_assert_ptr_null(found);
    ck_assert_uint_eq(bst_count_nodes(root), 3);
    ck_assert_ptr_nonnull(bst_search(root, "Florence"));
    ck_assert_ptr_nonnull(bst_search(root, "Venice"));
    
    bst_delete_tree(root);
}
END_TEST

// Test: Remove root node
START_TEST(test_remove_root)
{
    BSTNode *root = NULL;
    root = bst_insert(root, "Warsaw");
    root = bst_remove(root, "Warsaw");
    
    ck_assert_ptr_null(root);
}
END_TEST

// Test: Remove from empty tree
START_TEST(test_remove_empty)
{
    BSTNode *root = NULL;
    root = bst_remove(root, "Barcelona");
    ck_assert_ptr_null(root);
}
END_TEST

// Test: Remove non-existing city
START_TEST(test_remove_not_found)
{
    BSTNode *root = NULL;
    root = bst_insert(root, "Helsinki");
    root = bst_insert(root, "Turku");
    
    root = bst_remove(root, "Tampere");
    
    ck_assert_uint_eq(bst_count_nodes(root), 2);
    
    bst_delete_tree(root);
}
END_TEST

// Test: Height of empty tree
START_TEST(test_height_empty)
{
    BSTNode *root = NULL;
    int height = bst_height(root);
    ck_assert_int_eq(height, -1);
}
END_TEST

// Test: Height of single node
START_TEST(test_height_single)
{
    BSTNode *root = NULL;
    root = bst_insert(root, "Zurich");
    
    int height = bst_height(root);
    ck_assert_int_eq(height, 0);
    
    bst_delete_tree(root);
}
END_TEST

// Test: Height of balanced tree
START_TEST(test_height_balanced)
{
    BSTNode *root = NULL;
    root = bst_insert(root, "Geneva");
    root = bst_insert(root, "Basel");
    root = bst_insert(root, "Lausanne");
    
    int height = bst_height(root);
    ck_assert_int_eq(height, 1);
    
    bst_delete_tree(root);
}
END_TEST

// Test: Height of unbalanced tree
START_TEST(test_height_unbalanced)
{
    BSTNode *root = NULL;
    root = bst_insert(root, "A");
    root = bst_insert(root, "B");
    root = bst_insert(root, "C");
    root = bst_insert(root, "D");
    
    int height = bst_height(root);
    ck_assert_int_eq(height, 3);
    
    bst_delete_tree(root);
}
END_TEST

// Test: Count nodes in empty tree
START_TEST(test_count_empty)
{
    BSTNode *root = NULL;
    size_t count = bst_count_nodes(root);
    ck_assert_uint_eq(count, 0);
}
END_TEST

// Test: Count nodes in tree
START_TEST(test_count_nodes)
{
    BSTNode *root = NULL;
    root = bst_insert(root, "Lyon");
    root = bst_insert(root, "Marseille");
    root = bst_insert(root, "Nice");
    root = bst_insert(root, "Toulouse");
    root = bst_insert(root, "Nantes");
    
    size_t count = bst_count_nodes(root);
    ck_assert_uint_eq(count, 5);
    
    bst_delete_tree(root);
}
END_TEST

// Test: Complex operations sequence
START_TEST(test_complex_operations)
{
    BSTNode *root = NULL;
    
    // Insert cities
    root = bst_insert(root, "Madrid");
    root = bst_insert(root, "Barcelona");
    root = bst_insert(root, "Valencia");
    root = bst_insert(root, "Seville");
    root = bst_insert(root, "Zaragoza");
    
    ck_assert_uint_eq(bst_count_nodes(root), 5);
    
    // Search for cities
    ck_assert_ptr_nonnull(bst_search(root, "Barcelona"));
    ck_assert_ptr_nonnull(bst_search(root, "Valencia"));
    
    // Remove a city
    root = bst_remove(root, "Barcelona");
    ck_assert_uint_eq(bst_count_nodes(root), 4);
    ck_assert_ptr_null(bst_search(root, "Barcelona"));
    
    // Insert duplicate
    root = bst_insert(root, "Madrid");
    ck_assert_uint_eq(bst_count_nodes(root), 4);
    
    // Remove multiple cities
    root = bst_remove(root, "Seville");
    root = bst_remove(root, "Zaragoza");
    ck_assert_uint_eq(bst_count_nodes(root), 2);
    
    bst_delete_tree(root);
}
END_TEST

// Test: Alphabetical ordering
START_TEST(test_alphabetical_order)
{
    BSTNode *root = NULL;
    root = bst_insert(root, "Munich");
    root = bst_insert(root, "Berlin");
    root = bst_insert(root, "Hamburg");
    root = bst_insert(root, "Frankfurt");
    root = bst_insert(root, "Cologne");
    
    // Check that Berlin (alphabetically first) is leftmost
    BSTNode *min = bst_find_min(root);
    ck_assert_str_eq(min->city, "Berlin");
    
    // Verify structure
    ck_assert_str_eq(root->city, "Munich");
    ck_assert_ptr_nonnull(root->left);
    
    bst_delete_tree(root);
}
END_TEST

// Test: Delete tree
START_TEST(test_delete_tree)
{
    BSTNode *root = NULL;
    root = bst_insert(root, "Bern");
    root = bst_insert(root, "Geneva");
    root = bst_insert(root, "Basel");
    
    // Delete should free all memory without crashing
    bst_delete_tree(root);
    // If we reach here, deletion succeeded
}
END_TEST

// Test: Delete NULL tree
START_TEST(test_delete_null_tree)
{
    BSTNode *root = NULL;
    // Should not crash
    bst_delete_tree(root);
}
END_TEST

// Suite creation for Check framework
Suite *bst_suite(void)
{
    Suite *s;
    TCase *tc_node, *tc_insert, *tc_search, *tc_remove, *tc_metrics, *tc_complex;

    s = suite_create("BST");

    // Node operations test case
    tc_node = tcase_create("Node Operations");
    tcase_add_test(tc_node, test_create_node);
    tcase_add_test(tc_node, test_create_node_null);
    suite_add_tcase(s, tc_node);

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
