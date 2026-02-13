#include "bst.h"
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <string.h>

// Test: Create a single node
void test_create_node(void) {
    BSTNode *node = bst_create_node("Stockholm");
    CU_ASSERT_PTR_NOT_NULL(node);
    CU_ASSERT_STRING_EQUAL(node->city, "Stockholm");
    CU_ASSERT_PTR_NULL(node->left);
    CU_ASSERT_PTR_NULL(node->right);
    bst_delete_tree(node);
}

// Test: Create node with NULL city
void test_create_node_null(void) {
    BSTNode *node = bst_create_node(NULL);
    CU_ASSERT_PTR_NULL(node);
}

// Test: Insert single city
void test_insert_single(void) {
    BSTNode *root = NULL;
    root = bst_insert(root, "Paris");
    CU_ASSERT_PTR_NOT_NULL(root);
    CU_ASSERT_STRING_EQUAL(root->city, "Paris");
    bst_delete_tree(root);
}

// Test: Insert multiple cities
void test_insert_multiple(void) {
    BSTNode *root = NULL;
    root = bst_insert(root, "London");
    root = bst_insert(root, "Berlin");
    root = bst_insert(root, "Tokyo");
    
    CU_ASSERT_PTR_NOT_NULL(root);
    CU_ASSERT_STRING_EQUAL(root->city, "London");
    CU_ASSERT_PTR_NOT_NULL(root->left);
    CU_ASSERT_STRING_EQUAL(root->left->city, "Berlin");
    CU_ASSERT_PTR_NOT_NULL(root->right);
    CU_ASSERT_STRING_EQUAL(root->right->city, "Tokyo");
    
    bst_delete_tree(root);
}

// Test: Insert duplicate cities (should not insert)
void test_insert_duplicate(void) {
    BSTNode *root = NULL;
    root = bst_insert(root, "Madrid");
    root = bst_insert(root, "Madrid");
    
    size_t count = bst_count_nodes(root);
    CU_ASSERT_EQUAL(count, 1);
    
    bst_delete_tree(root);
}

// Test: Insert NULL city
void test_insert_null(void) {
    BSTNode *root = NULL;
    root = bst_insert(root, "Rome");
    root = bst_insert(root, NULL);
    
    size_t count = bst_count_nodes(root);
    CU_ASSERT_EQUAL(count, 1);
    
    bst_delete_tree(root);
}

// Test: Search for existing city
void test_search_found(void) {
    BSTNode *root = NULL;
    root = bst_insert(root, "Amsterdam");
    root = bst_insert(root, "Brussels");
    root = bst_insert(root, "Copenhagen");
    
    BSTNode *found = bst_search(root, "Brussels");
    CU_ASSERT_PTR_NOT_NULL(found);
    CU_ASSERT_STRING_EQUAL(found->city, "Brussels");
    
    bst_delete_tree(root);
}

// Test: Search for non-existing city
void test_search_not_found(void) {
    BSTNode *root = NULL;
    root = bst_insert(root, "Vienna");
    root = bst_insert(root, "Prague");
    
    BSTNode *found = bst_search(root, "Budapest");
    CU_ASSERT_PTR_NULL(found);
    
    bst_delete_tree(root);
}

// Test: Search in empty tree
void test_search_empty(void) {
    BSTNode *root = NULL;
    BSTNode *found = bst_search(root, "Athens");
    CU_ASSERT_PTR_NULL(found);
}

// Test: Search with NULL city
void test_search_null(void) {
    BSTNode *root = NULL;
    root = bst_insert(root, "Oslo");
    
    BSTNode *found = bst_search(root, NULL);
    CU_ASSERT_PTR_NULL(found);
    
    bst_delete_tree(root);
}

// Test: Find minimum in tree
void test_find_min(void) {
    BSTNode *root = NULL;
    root = bst_insert(root, "Munich");
    root = bst_insert(root, "Hamburg");
    root = bst_insert(root, "Stuttgart");
    root = bst_insert(root, "Frankfurt");
    
    BSTNode *min = bst_find_min(root);
    CU_ASSERT_PTR_NOT_NULL(min);
    CU_ASSERT_STRING_EQUAL(min->city, "Frankfurt");
    
    bst_delete_tree(root);
}

// Test: Find minimum in empty tree
void test_find_min_empty(void) {
    BSTNode *root = NULL;
    BSTNode *min = bst_find_min(root);
    CU_ASSERT_PTR_NULL(min);
}

// Test: Remove leaf node
void test_remove_leaf(void) {
    BSTNode *root = NULL;
    root = bst_insert(root, "Dublin");
    root = bst_insert(root, "Cork");
    root = bst_insert(root, "Galway");
    
    root = bst_remove(root, "Cork");
    
    BSTNode *found = bst_search(root, "Cork");
    CU_ASSERT_PTR_NULL(found);
    CU_ASSERT_EQUAL(bst_count_nodes(root), 2);
    
    bst_delete_tree(root);
}

// Test: Remove node with one child
void test_remove_one_child(void) {
    BSTNode *root = NULL;
    root = bst_insert(root, "Lisbon");
    root = bst_insert(root, "Porto");
    root = bst_insert(root, "Sintra");
    
    root = bst_remove(root, "Porto");
    
    BSTNode *found = bst_search(root, "Porto");
    CU_ASSERT_PTR_NULL(found);
    CU_ASSERT_PTR_NOT_NULL(bst_search(root, "Sintra"));
    
    bst_delete_tree(root);
}

// Test: Remove node with two children
void test_remove_two_children(void) {
    BSTNode *root = NULL;
    root = bst_insert(root, "Milan");
    root = bst_insert(root, "Florence");
    root = bst_insert(root, "Venice");
    root = bst_insert(root, "Rome");
    
    root = bst_remove(root, "Milan");
    
    BSTNode *found = bst_search(root, "Milan");
    CU_ASSERT_PTR_NULL(found);
    CU_ASSERT_EQUAL(bst_count_nodes(root), 3);
    CU_ASSERT_PTR_NOT_NULL(bst_search(root, "Florence"));
    CU_ASSERT_PTR_NOT_NULL(bst_search(root, "Venice"));
    
    bst_delete_tree(root);
}

// Test: Remove root node
void test_remove_root(void) {
    BSTNode *root = NULL;
    root = bst_insert(root, "Warsaw");
    root = bst_remove(root, "Warsaw");
    
    CU_ASSERT_PTR_NULL(root);
}

// Test: Remove from empty tree
void test_remove_empty(void) {
    BSTNode *root = NULL;
    root = bst_remove(root, "Barcelona");
    CU_ASSERT_PTR_NULL(root);
}

// Test: Remove non-existing city
void test_remove_not_found(void) {
    BSTNode *root = NULL;
    root = bst_insert(root, "Helsinki");
    root = bst_insert(root, "Turku");
    
    root = bst_remove(root, "Tampere");
    
    CU_ASSERT_EQUAL(bst_count_nodes(root), 2);
    
    bst_delete_tree(root);
}

// Test: Height of empty tree
void test_height_empty(void) {
    BSTNode *root = NULL;
    int height = bst_height(root);
    CU_ASSERT_EQUAL(height, -1);
}

// Test: Height of single node
void test_height_single(void) {
    BSTNode *root = NULL;
    root = bst_insert(root, "Zurich");
    
    int height = bst_height(root);
    CU_ASSERT_EQUAL(height, 0);
    
    bst_delete_tree(root);
}

// Test: Height of balanced tree
void test_height_balanced(void) {
    BSTNode *root = NULL;
    root = bst_insert(root, "Geneva");
    root = bst_insert(root, "Basel");
    root = bst_insert(root, "Lausanne");
    
    int height = bst_height(root);
    CU_ASSERT_EQUAL(height, 1);
    
    bst_delete_tree(root);
}

// Test: Height of unbalanced tree
void test_height_unbalanced(void) {
    BSTNode *root = NULL;
    root = bst_insert(root, "A");
    root = bst_insert(root, "B");
    root = bst_insert(root, "C");
    root = bst_insert(root, "D");
    
    int height = bst_height(root);
    CU_ASSERT_EQUAL(height, 3);
    
    bst_delete_tree(root);
}

// Test: Count nodes in empty tree
void test_count_empty(void) {
    BSTNode *root = NULL;
    size_t count = bst_count_nodes(root);
    CU_ASSERT_EQUAL(count, 0);
}

// Test: Count nodes in tree
void test_count_nodes(void) {
    BSTNode *root = NULL;
    root = bst_insert(root, "Lyon");
    root = bst_insert(root, "Marseille");
    root = bst_insert(root, "Nice");
    root = bst_insert(root, "Toulouse");
    root = bst_insert(root, "Nantes");
    
    size_t count = bst_count_nodes(root);
    CU_ASSERT_EQUAL(count, 5);
    
    bst_delete_tree(root);
}

// Test: Complex operations sequence
void test_complex_operations(void) {
    BSTNode *root = NULL;
    
    // Insert cities
    root = bst_insert(root, "Madrid");
    root = bst_insert(root, "Barcelona");
    root = bst_insert(root, "Valencia");
    root = bst_insert(root, "Seville");
    root = bst_insert(root, "Zaragoza");
    
    CU_ASSERT_EQUAL(bst_count_nodes(root), 5);
    
    // Search for cities
    CU_ASSERT_PTR_NOT_NULL(bst_search(root, "Barcelona"));
    CU_ASSERT_PTR_NOT_NULL(bst_search(root, "Valencia"));
    
    // Remove a city
    root = bst_remove(root, "Barcelona");
    CU_ASSERT_EQUAL(bst_count_nodes(root), 4);
    CU_ASSERT_PTR_NULL(bst_search(root, "Barcelona"));
    
    // Insert duplicate
    root = bst_insert(root, "Madrid");
    CU_ASSERT_EQUAL(bst_count_nodes(root), 4);
    
    // Remove multiple cities
    root = bst_remove(root, "Seville");
    root = bst_remove(root, "Zaragoza");
    CU_ASSERT_EQUAL(bst_count_nodes(root), 2);
    
    bst_delete_tree(root);
}

// Test: Alphabetical ordering
void test_alphabetical_order(void) {
    BSTNode *root = NULL;
    root = bst_insert(root, "Munich");
    root = bst_insert(root, "Berlin");
    root = bst_insert(root, "Hamburg");
    root = bst_insert(root, "Frankfurt");
    root = bst_insert(root, "Cologne");
    
    // Check that Berlin (alphabetically first) is leftmost
    BSTNode *min = bst_find_min(root);
    CU_ASSERT_STRING_EQUAL(min->city, "Berlin");
    
    // Verify structure
    CU_ASSERT_STRING_EQUAL(root->city, "Munich");
    CU_ASSERT_PTR_NOT_NULL(root->left);
    
    bst_delete_tree(root);
}

// Test: Delete tree
void test_delete_tree(void) {
    BSTNode *root = NULL;
    root = bst_insert(root, "Bern");
    root = bst_insert(root, "Geneva");
    root = bst_insert(root, "Basel");
    
    // Delete should free all memory without crashing
    bst_delete_tree(root);
    // If we reach here, deletion succeeded
    CU_PASS("Tree deletion succeeded");
}

// Test: Delete NULL tree
void test_delete_null_tree(void) {
    BSTNode *root = NULL;
    // Should not crash
    bst_delete_tree(root);
    CU_PASS("Deleting NULL tree succeeded");
}

// Main test runner
int main(void) {
    // Initialize CUnit test registry
    if (CU_initialize_registry() != CUE_SUCCESS) {
        return CU_get_error();
    }
    
    // Add test suite
    CU_pSuite pSuite = CU_add_suite("BST_Test_Suite", NULL, NULL);
    if (pSuite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    // Add tests to the suite
    if ((CU_add_test(pSuite, "test_create_node", test_create_node) == NULL) ||
        (CU_add_test(pSuite, "test_create_node_null", test_create_node_null) == NULL) ||
        (CU_add_test(pSuite, "test_insert_single", test_insert_single) == NULL) ||
        (CU_add_test(pSuite, "test_insert_multiple", test_insert_multiple) == NULL) ||
        (CU_add_test(pSuite, "test_insert_duplicate", test_insert_duplicate) == NULL) ||
        (CU_add_test(pSuite, "test_insert_null", test_insert_null) == NULL) ||
        (CU_add_test(pSuite, "test_search_found", test_search_found) == NULL) ||
        (CU_add_test(pSuite, "test_search_not_found", test_search_not_found) == NULL) ||
        (CU_add_test(pSuite, "test_search_empty", test_search_empty) == NULL) ||
        (CU_add_test(pSuite, "test_search_null", test_search_null) == NULL) ||
        (CU_add_test(pSuite, "test_find_min", test_find_min) == NULL) ||
        (CU_add_test(pSuite, "test_find_min_empty", test_find_min_empty) == NULL) ||
        (CU_add_test(pSuite, "test_remove_leaf", test_remove_leaf) == NULL) ||
        (CU_add_test(pSuite, "test_remove_one_child", test_remove_one_child) == NULL) ||
        (CU_add_test(pSuite, "test_remove_two_children", test_remove_two_children) == NULL) ||
        (CU_add_test(pSuite, "test_remove_root", test_remove_root) == NULL) ||
        (CU_add_test(pSuite, "test_remove_empty", test_remove_empty) == NULL) ||
        (CU_add_test(pSuite, "test_remove_not_found", test_remove_not_found) == NULL) ||
        (CU_add_test(pSuite, "test_height_empty", test_height_empty) == NULL) ||
        (CU_add_test(pSuite, "test_height_single", test_height_single) == NULL) ||
        (CU_add_test(pSuite, "test_height_balanced", test_height_balanced) == NULL) ||
        (CU_add_test(pSuite, "test_height_unbalanced", test_height_unbalanced) == NULL) ||
        (CU_add_test(pSuite, "test_count_empty", test_count_empty) == NULL) ||
        (CU_add_test(pSuite, "test_count_nodes", test_count_nodes) == NULL) ||
        (CU_add_test(pSuite, "test_complex_operations", test_complex_operations) == NULL) ||
        (CU_add_test(pSuite, "test_alphabetical_order", test_alphabetical_order) == NULL) ||
        (CU_add_test(pSuite, "test_delete_tree", test_delete_tree) == NULL) ||
        (CU_add_test(pSuite, "test_delete_null_tree", test_delete_null_tree) == NULL)) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    // Run tests using the basic interface
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    
    // Get test results
    int failures = CU_get_number_of_failures();
    
    // Cleanup and return
    CU_cleanup_registry();
    return failures;
}
