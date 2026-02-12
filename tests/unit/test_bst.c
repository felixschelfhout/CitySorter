#include "bst.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

// Test statistics
static int tests_run = 0;
static int tests_passed = 0;
static int tests_failed = 0;

// Color codes for terminal output
#define COLOR_GREEN "\033[0;32m"
#define COLOR_RED "\033[0;31m"
#define COLOR_RESET "\033[0m"
#define COLOR_YELLOW "\033[0;33m"
#define COLOR_CYAN "\033[0;36m"

// Test macros
#define TEST(name) void name()
#define RUN_TEST(test) do { \
    printf(COLOR_CYAN "Running: %s" COLOR_RESET "\n", #test); \
    tests_run++; \
    test(); \
    tests_passed++; \
    printf(COLOR_GREEN "✓ PASSED: %s" COLOR_RESET "\n\n", #test); \
} while(0)

#define ASSERT(condition, message) do { \
    if (!(condition)) { \
        printf(COLOR_RED "✗ FAILED: %s" COLOR_RESET "\n", message); \
        printf("  at %s:%d\n\n", __FILE__, __LINE__); \
        tests_failed++; \
        return; \
    } \
} while(0)

#define ASSERT_NULL(ptr, message) ASSERT((ptr) == NULL, message)
#define ASSERT_NOT_NULL(ptr, message) ASSERT((ptr) != NULL, message)
#define ASSERT_EQUAL(a, b, message) ASSERT((a) == (b), message)
#define ASSERT_STR_EQUAL(a, b, message) ASSERT(strcmp((a), (b)) == 0, message)

// Test: Create a single node
TEST(test_create_node) {
    BSTNode *node = bst_create_node("Stockholm");
    ASSERT_NOT_NULL(node, "Node creation failed");
    ASSERT_STR_EQUAL(node->city, "Stockholm", "City name mismatch");
    ASSERT_NULL(node->left, "Left child should be NULL");
    ASSERT_NULL(node->right, "Right child should be NULL");
    bst_delete_tree(node);
}

// Test: Create node with NULL city
TEST(test_create_node_null) {
    BSTNode *node = bst_create_node(NULL);
    ASSERT_NULL(node, "Creating node with NULL city should return NULL");
}

// Test: Insert single city
TEST(test_insert_single) {
    BSTNode *root = NULL;
    root = bst_insert(root, "Paris");
    ASSERT_NOT_NULL(root, "Root should not be NULL after insertion");
    ASSERT_STR_EQUAL(root->city, "Paris", "Root city mismatch");
    bst_delete_tree(root);
}

// Test: Insert multiple cities
TEST(test_insert_multiple) {
    BSTNode *root = NULL;
    root = bst_insert(root, "London");
    root = bst_insert(root, "Berlin");
    root = bst_insert(root, "Tokyo");
    
    ASSERT_NOT_NULL(root, "Root should not be NULL");
    ASSERT_STR_EQUAL(root->city, "London", "Root city mismatch");
    ASSERT_NOT_NULL(root->left, "Left child should exist (Berlin)");
    ASSERT_STR_EQUAL(root->left->city, "Berlin", "Left child city mismatch");
    ASSERT_NOT_NULL(root->right, "Right child should exist (Tokyo)");
    ASSERT_STR_EQUAL(root->right->city, "Tokyo", "Right child city mismatch");
    
    bst_delete_tree(root);
}

// Test: Insert duplicate cities (should not insert)
TEST(test_insert_duplicate) {
    BSTNode *root = NULL;
    root = bst_insert(root, "Madrid");
    root = bst_insert(root, "Madrid");
    
    size_t count = bst_count_nodes(root);
    ASSERT_EQUAL(count, 1, "Duplicate city should not be inserted");
    
    bst_delete_tree(root);
}

// Test: Insert NULL city
TEST(test_insert_null) {
    BSTNode *root = NULL;
    root = bst_insert(root, "Rome");
    root = bst_insert(root, NULL);
    
    size_t count = bst_count_nodes(root);
    ASSERT_EQUAL(count, 1, "NULL city should not be inserted");
    
    bst_delete_tree(root);
}

// Test: Search for existing city
TEST(test_search_found) {
    BSTNode *root = NULL;
    root = bst_insert(root, "Amsterdam");
    root = bst_insert(root, "Brussels");
    root = bst_insert(root, "Copenhagen");
    
    BSTNode *found = bst_search(root, "Brussels");
    ASSERT_NOT_NULL(found, "Search should find Brussels");
    ASSERT_STR_EQUAL(found->city, "Brussels", "Found city mismatch");
    
    bst_delete_tree(root);
}

// Test: Search for non-existing city
TEST(test_search_not_found) {
    BSTNode *root = NULL;
    root = bst_insert(root, "Vienna");
    root = bst_insert(root, "Prague");
    
    BSTNode *found = bst_search(root, "Budapest");
    ASSERT_NULL(found, "Search should not find Budapest");
    
    bst_delete_tree(root);
}

// Test: Search in empty tree
TEST(test_search_empty) {
    BSTNode *root = NULL;
    BSTNode *found = bst_search(root, "Athens");
    ASSERT_NULL(found, "Search in empty tree should return NULL");
}

// Test: Search with NULL city
TEST(test_search_null) {
    BSTNode *root = NULL;
    root = bst_insert(root, "Oslo");
    
    BSTNode *found = bst_search(root, NULL);
    ASSERT_NULL(found, "Search with NULL city should return NULL");
    
    bst_delete_tree(root);
}

// Test: Find minimum in tree
TEST(test_find_min) {
    BSTNode *root = NULL;
    root = bst_insert(root, "Munich");
    root = bst_insert(root, "Hamburg");
    root = bst_insert(root, "Stuttgart");
    root = bst_insert(root, "Frankfurt");
    
    BSTNode *min = bst_find_min(root);
    ASSERT_NOT_NULL(min, "Find min should return a node");
    ASSERT_STR_EQUAL(min->city, "Frankfurt", "Minimum city should be Frankfurt");
    
    bst_delete_tree(root);
}

// Test: Find minimum in empty tree
TEST(test_find_min_empty) {
    BSTNode *root = NULL;
    BSTNode *min = bst_find_min(root);
    ASSERT_NULL(min, "Find min in empty tree should return NULL");
}

// Test: Remove leaf node
TEST(test_remove_leaf) {
    BSTNode *root = NULL;
    root = bst_insert(root, "Dublin");
    root = bst_insert(root, "Cork");
    root = bst_insert(root, "Galway");
    
    root = bst_remove(root, "Cork");
    
    BSTNode *found = bst_search(root, "Cork");
    ASSERT_NULL(found, "Cork should be removed");
    ASSERT_EQUAL(bst_count_nodes(root), 2, "Tree should have 2 nodes");
    
    bst_delete_tree(root);
}

// Test: Remove node with one child
TEST(test_remove_one_child) {
    BSTNode *root = NULL;
    root = bst_insert(root, "Lisbon");
    root = bst_insert(root, "Porto");
    root = bst_insert(root, "Sintra");
    
    root = bst_remove(root, "Porto");
    
    BSTNode *found = bst_search(root, "Porto");
    ASSERT_NULL(found, "Porto should be removed");
    ASSERT_NOT_NULL(bst_search(root, "Sintra"), "Sintra should still exist");
    
    bst_delete_tree(root);
}

// Test: Remove node with two children
TEST(test_remove_two_children) {
    BSTNode *root = NULL;
    root = bst_insert(root, "Milan");
    root = bst_insert(root, "Florence");
    root = bst_insert(root, "Venice");
    root = bst_insert(root, "Rome");
    
    root = bst_remove(root, "Milan");
    
    BSTNode *found = bst_search(root, "Milan");
    ASSERT_NULL(found, "Milan should be removed");
    ASSERT_EQUAL(bst_count_nodes(root), 3, "Tree should have 3 nodes");
    ASSERT_NOT_NULL(bst_search(root, "Florence"), "Florence should exist");
    ASSERT_NOT_NULL(bst_search(root, "Venice"), "Venice should exist");
    
    bst_delete_tree(root);
}

// Test: Remove root node
TEST(test_remove_root) {
    BSTNode *root = NULL;
    root = bst_insert(root, "Warsaw");
    root = bst_remove(root, "Warsaw");
    
    ASSERT_NULL(root, "Root should be NULL after removal");
}

// Test: Remove from empty tree
TEST(test_remove_empty) {
    BSTNode *root = NULL;
    root = bst_remove(root, "Barcelona");
    ASSERT_NULL(root, "Removing from empty tree should return NULL");
}

// Test: Remove non-existing city
TEST(test_remove_not_found) {
    BSTNode *root = NULL;
    root = bst_insert(root, "Helsinki");
    root = bst_insert(root, "Turku");
    
    root = bst_remove(root, "Tampere");
    
    ASSERT_EQUAL(bst_count_nodes(root), 2, "Tree should still have 2 nodes");
    
    bst_delete_tree(root);
}

// Test: Height of empty tree
TEST(test_height_empty) {
    BSTNode *root = NULL;
    int height = bst_height(root);
    ASSERT_EQUAL(height, -1, "Height of empty tree should be -1");
}

// Test: Height of single node
TEST(test_height_single) {
    BSTNode *root = NULL;
    root = bst_insert(root, "Zurich");
    
    int height = bst_height(root);
    ASSERT_EQUAL(height, 0, "Height of single node should be 0");
    
    bst_delete_tree(root);
}

// Test: Height of balanced tree
TEST(test_height_balanced) {
    BSTNode *root = NULL;
    root = bst_insert(root, "Geneva");
    root = bst_insert(root, "Basel");
    root = bst_insert(root, "Lausanne");
    
    int height = bst_height(root);
    ASSERT_EQUAL(height, 1, "Height of balanced tree should be 1");
    
    bst_delete_tree(root);
}

// Test: Height of unbalanced tree
TEST(test_height_unbalanced) {
    BSTNode *root = NULL;
    root = bst_insert(root, "A");
    root = bst_insert(root, "B");
    root = bst_insert(root, "C");
    root = bst_insert(root, "D");
    
    int height = bst_height(root);
    ASSERT_EQUAL(height, 3, "Height of unbalanced tree should be 3");
    
    bst_delete_tree(root);
}

// Test: Count nodes in empty tree
TEST(test_count_empty) {
    BSTNode *root = NULL;
    size_t count = bst_count_nodes(root);
    ASSERT_EQUAL(count, 0, "Count of empty tree should be 0");
}

// Test: Count nodes in tree
TEST(test_count_nodes) {
    BSTNode *root = NULL;
    root = bst_insert(root, "Lyon");
    root = bst_insert(root, "Marseille");
    root = bst_insert(root, "Nice");
    root = bst_insert(root, "Toulouse");
    root = bst_insert(root, "Nantes");
    
    size_t count = bst_count_nodes(root);
    ASSERT_EQUAL(count, 5, "Count should be 5");
    
    bst_delete_tree(root);
}

// Test: Complex operations sequence
TEST(test_complex_operations) {
    BSTNode *root = NULL;
    
    // Insert cities
    root = bst_insert(root, "Madrid");
    root = bst_insert(root, "Barcelona");
    root = bst_insert(root, "Valencia");
    root = bst_insert(root, "Seville");
    root = bst_insert(root, "Zaragoza");
    
    ASSERT_EQUAL(bst_count_nodes(root), 5, "Should have 5 cities");
    
    // Search for cities
    ASSERT_NOT_NULL(bst_search(root, "Barcelona"), "Barcelona should be found");
    ASSERT_NOT_NULL(bst_search(root, "Valencia"), "Valencia should be found");
    
    // Remove a city
    root = bst_remove(root, "Barcelona");
    ASSERT_EQUAL(bst_count_nodes(root), 4, "Should have 4 cities after removal");
    ASSERT_NULL(bst_search(root, "Barcelona"), "Barcelona should not be found");
    
    // Insert duplicate
    root = bst_insert(root, "Madrid");
    ASSERT_EQUAL(bst_count_nodes(root), 4, "Duplicate should not be inserted");
    
    // Remove multiple cities
    root = bst_remove(root, "Seville");
    root = bst_remove(root, "Zaragoza");
    ASSERT_EQUAL(bst_count_nodes(root), 2, "Should have 2 cities");
    
    bst_delete_tree(root);
}

// Test: Alphabetical ordering
TEST(test_alphabetical_order) {
    BSTNode *root = NULL;
    root = bst_insert(root, "Munich");
    root = bst_insert(root, "Berlin");
    root = bst_insert(root, "Hamburg");
    root = bst_insert(root, "Frankfurt");
    root = bst_insert(root, "Cologne");
    
    // Check that Berlin (alphabetically first) is leftmost
    BSTNode *min = bst_find_min(root);
    ASSERT_STR_EQUAL(min->city, "Berlin", "Berlin should be the minimum");
    
    // Verify structure
    ASSERT_STR_EQUAL(root->city, "Munich", "Root should be Munich");
    ASSERT_NOT_NULL(root->left, "Munich should have left subtree");
    
    bst_delete_tree(root);
}

// Test: Delete tree
TEST(test_delete_tree) {
    BSTNode *root = NULL;
    root = bst_insert(root, "Bern");
    root = bst_insert(root, "Geneva");
    root = bst_insert(root, "Basel");
    
    // Delete should free all memory without crashing
    bst_delete_tree(root);
    // If we reach here, deletion succeeded
    ASSERT(1, "Tree deletion succeeded");
}

// Test: Delete NULL tree
TEST(test_delete_null_tree) {
    BSTNode *root = NULL;
    // Should not crash
    bst_delete_tree(root);
    ASSERT(1, "Deleting NULL tree succeeded");
}

// Main test runner
int main() {
    printf("\n");
    printf("================================================\n");
    printf("         BST Unit Tests\n");
    printf("================================================\n\n");
    
    // Run all tests
    RUN_TEST(test_create_node);
    RUN_TEST(test_create_node_null);
    RUN_TEST(test_insert_single);
    RUN_TEST(test_insert_multiple);
    RUN_TEST(test_insert_duplicate);
    RUN_TEST(test_insert_null);
    RUN_TEST(test_search_found);
    RUN_TEST(test_search_not_found);
    RUN_TEST(test_search_empty);
    RUN_TEST(test_search_null);
    RUN_TEST(test_find_min);
    RUN_TEST(test_find_min_empty);
    RUN_TEST(test_remove_leaf);
    RUN_TEST(test_remove_one_child);
    RUN_TEST(test_remove_two_children);
    RUN_TEST(test_remove_root);
    RUN_TEST(test_remove_empty);
    RUN_TEST(test_remove_not_found);
    RUN_TEST(test_height_empty);
    RUN_TEST(test_height_single);
    RUN_TEST(test_height_balanced);
    RUN_TEST(test_height_unbalanced);
    RUN_TEST(test_count_empty);
    RUN_TEST(test_count_nodes);
    RUN_TEST(test_complex_operations);
    RUN_TEST(test_alphabetical_order);
    RUN_TEST(test_delete_tree);
    RUN_TEST(test_delete_null_tree);
    
    // Print summary
    printf("================================================\n");
    printf("         Test Summary\n");
    printf("================================================\n");
    printf("Tests Run:    %s%d%s\n", COLOR_CYAN, tests_run, COLOR_RESET);
    printf("Tests Passed: %s%d%s\n", COLOR_GREEN, tests_passed, COLOR_RESET);
    printf("Tests Failed: %s%d%s\n", tests_failed > 0 ? COLOR_RED : COLOR_GREEN, tests_failed, COLOR_RESET);
    printf("------------------------------------------------\n");
    
    if (tests_failed == 0) {
        printf("%s✓ All tests passed!%s\n", COLOR_GREEN, COLOR_RESET);
        printf("================================================\n\n");
        return 0;
    } else {
        printf("%s✗ Some tests failed!%s\n", COLOR_RED, COLOR_RESET);
        printf("================================================\n\n");
        return 1;
    }
}
