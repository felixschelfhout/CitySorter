#include "bst.h"
#include <check.h>
#include <stdio.h>
#include <stdlib.h>
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

/**
 * Integer comparison function for BST
 * returns negative value if a is less than b
 * returns positive value if a is greater than b
 * returns 0, if a and b are equal
 */
static int compare_ints(const void *a, const void *b) {
    const int val_a = *(const int *)a;
    const int val_b = *(const int *)b;

    if (val_a < val_b) {
        return -1;
    }
    if (val_a > val_b) {
        return 1;
    }
    return 0;
}

/**
 * Integer print function for BST
 */
static void print_int(const void *data) {
    printf("%d\n", *(const int *)data);
}

typedef struct OutputBuffer {
    char *data;
    size_t size;
    size_t capacity;
} OutputBuffer;

static OutputBuffer *g_output_buffer = NULL;

static int output_buffer_init(OutputBuffer *buffer, size_t capacity) {
    buffer->data = (char *)malloc(capacity);
    if (!buffer->data) {
        return 0;
    }
    buffer->size = 0;
    buffer->capacity = capacity;
    buffer->data[0] = '\0';
    return 1;
}

static void output_buffer_free(OutputBuffer *buffer) {
    free(buffer->data);
    buffer->data = NULL;
    buffer->size = 0;
    buffer->capacity = 0;
}

/**
 * Author's note:
 * I don't like the use of realloc here, but I let it slide for the sake of testing.
 * In production code, I would likely use a different approach to manage the buffer size more
 * efficiently.
 */
static int output_buffer_append(OutputBuffer *buffer, const char *text) {
    size_t len = strlen(text);
    if (buffer->size + len + 1 > buffer->capacity) {
        size_t new_capacity = buffer->capacity * 2;
        if (new_capacity < buffer->size + len + 1) {
            new_capacity = buffer->size + len + 1;
        }
        char *new_data = (char *)realloc(buffer->data, new_capacity);
        if (!new_data) {
            return 0;
        }
        buffer->data = new_data;
        buffer->capacity = new_capacity;
    }

    memcpy(buffer->data + buffer->size, text, len);
    buffer->size += len;
    buffer->data[buffer->size] = '\0';
    return 1;
}

static void print_string_to_buffer(const void *data) {
    if (!g_output_buffer) {
        return;
    }
    output_buffer_append(g_output_buffer, (const char *)data);
    output_buffer_append(g_output_buffer, "\n");
}

static void print_int_to_buffer(const void *data) {
    if (!g_output_buffer) {
        return;
    }
    char buffer_str[32];
    snprintf(buffer_str, sizeof(buffer_str), "%d", *(const int *)data);
    output_buffer_append(g_output_buffer, buffer_str);
    output_buffer_append(g_output_buffer, "\n");
}

// Test: Create a tree
START_TEST(test_create_tree) {
    BSTTree *tree = bst_create(compare_strings, print_string);
    ck_assert_ptr_nonnull(tree);
    ck_assert_uint_eq(bst_count_nodes(tree), 0);
    bst_delete(tree);
}
END_TEST

// Test: Create tree with NULL compare
START_TEST(test_create_tree_null) {
    BSTTree *tree = bst_create(NULL, NULL);
    ck_assert_ptr_null(tree);
}
END_TEST

// Test: Insert single city
START_TEST(test_insert_single) {
    BSTTree *tree = bst_create(compare_strings, print_string);
    int result = bst_insert(tree, (void *)"Paris");
    ck_assert_int_eq(result, 0);
    ck_assert_uint_eq(bst_count_nodes(tree), 1);

    const void *found = bst_search(tree, (void *)"Paris");
    ck_assert_ptr_nonnull(found);
    ck_assert_str_eq((const char *)found, "Paris");
    bst_delete(tree);
}
END_TEST

// Test: Insert multiple cities
START_TEST(test_insert_multiple) {
    BSTTree *tree = bst_create(compare_strings, print_string);
    bst_insert(tree, (void *)"London");
    bst_insert(tree, (void *)"Berlin");
    bst_insert(tree, (void *)"Tokyo");

    ck_assert_uint_eq(bst_count_nodes(tree), 3);

    ck_assert_ptr_nonnull(bst_search(tree, (void *)"London"));
    ck_assert_ptr_nonnull(bst_search(tree, (void *)"Berlin"));
    ck_assert_ptr_nonnull(bst_search(tree, (void *)"Tokyo"));

    bst_delete(tree);
}
END_TEST

// Test: Insert duplicate cities (should not insert)
START_TEST(test_insert_duplicate) {
    BSTTree *tree = bst_create(compare_strings, print_string);
    bst_insert(tree, (void *)"Madrid");
    bst_insert(tree, (void *)"Madrid");

    size_t count = bst_count_nodes(tree);
    ck_assert_uint_eq(count, 1);

    bst_delete(tree);
}
END_TEST

// Test: Insert NULL data
START_TEST(test_insert_null) {
    BSTTree *tree = bst_create(compare_strings, print_string);
    bst_insert(tree, (void *)"Rome");
    int result = bst_insert(tree, NULL);

    size_t count = bst_count_nodes(tree);
    ck_assert_uint_eq(count, 1);
    ck_assert_int_eq(result, -1);

    bst_delete(tree);
}
END_TEST

// Test: Search for existing city
START_TEST(test_search_found) {
    BSTTree *tree = bst_create(compare_strings, print_string);
    bst_insert(tree, (void *)"Amsterdam");
    bst_insert(tree, (void *)"Brussels");
    bst_insert(tree, (void *)"Copenhagen");

    const void *found = bst_search(tree, (void *)"Brussels");
    ck_assert_ptr_nonnull(found);
    ck_assert_str_eq((const char *)found, "Brussels");

    bst_delete(tree);
}
END_TEST

// Test: Search for non-existing city
START_TEST(test_search_not_found) {
    BSTTree *tree = bst_create(compare_strings, print_string);
    bst_insert(tree, (void *)"Vienna");
    bst_insert(tree, (void *)"Prague");

    const void *found = bst_search(tree, (void *)"Budapest");
    ck_assert_ptr_null(found);

    bst_delete(tree);
}
END_TEST

// Test: Search in empty tree
START_TEST(test_search_empty) {
    BSTTree *tree = bst_create(compare_strings, print_string);
    const void *found = bst_search(tree, (void *)"Athens");
    ck_assert_ptr_null(found);
    bst_delete(tree);
}
END_TEST

// Test: Search with NULL data
START_TEST(test_search_null) {
    BSTTree *tree = bst_create(compare_strings, print_string);
    bst_insert(tree, (void *)"Oslo");

    const void *found = bst_search(tree, NULL);
    ck_assert_ptr_null(found);

    bst_delete(tree);
}
END_TEST

// Test: Find minimum in tree
START_TEST(test_find_min) {
    BSTTree *tree = bst_create(compare_strings, print_string);
    bst_insert(tree, (void *)"Munich");
    bst_insert(tree, (void *)"Hamburg");
    bst_insert(tree, (void *)"Stuttgart");
    bst_insert(tree, (void *)"Frankfurt");

    const void *min = bst_get_min(tree);
    ck_assert_ptr_nonnull(min);
    ck_assert_str_eq((const char *)min, "Frankfurt");

    bst_delete(tree);
}
END_TEST

// Test: Find minimum in empty tree
START_TEST(test_find_min_empty) {
    BSTTree *tree = bst_create(compare_strings, print_string);
    const void *min = bst_get_min(tree);
    ck_assert_ptr_null(min);
    bst_delete(tree);
}
END_TEST

// Test: Find maximum in tree
START_TEST(test_find_max) {
    BSTTree *tree = bst_create(compare_strings, print_string);
    bst_insert(tree, (void *)"Munich");
    bst_insert(tree, (void *)"Hamburg");
    bst_insert(tree, (void *)"Stuttgart");
    bst_insert(tree, (void *)"Frankfurt");

    const void *max = bst_get_max(tree);
    ck_assert_ptr_nonnull(max);
    ck_assert_str_eq((const char *)max, "Stuttgart");

    bst_delete(tree);
}
END_TEST

// Test: Find maximum in empty tree
START_TEST(test_find_max_empty) {
    BSTTree *tree = bst_create(compare_strings, print_string);
    const void *max = bst_get_max(tree);
    ck_assert_ptr_null(max);
    bst_delete(tree);
}
END_TEST

// Test: Print output
START_TEST(test_bst_retrieve_high_low) {
    BSTTree *tree = bst_create(compare_strings, print_string_to_buffer);
    bst_insert(tree, (void *)"B");
    bst_insert(tree, (void *)"A");
    bst_insert(tree, (void *)"C");

    OutputBuffer buffer;
    ck_assert_int_eq(output_buffer_init(&buffer, 32), 1);
    g_output_buffer = &buffer;
    bst_retrieve_data_high_to_low(tree);
    g_output_buffer = NULL;

    // Find position of each expected element
    char *pos_c = strstr(buffer.data, "C\n");
    char *pos_b = strstr(buffer.data, "B\n");
    char *pos_a = strstr(buffer.data, "A\n");

    // Verify all elements were found
    ck_assert_ptr_nonnull(pos_c);
    ck_assert_ptr_nonnull(pos_b);
    ck_assert_ptr_nonnull(pos_a);

    // Verify all elements were printed in the correct order (C at top, A at bottom)
    ck_assert(pos_c < pos_b);
    ck_assert(pos_b < pos_a);

    output_buffer_free(&buffer);
    bst_delete(tree);
}
END_TEST

// Test: Print rotated balanced int tree
static void check_positions_nonnull(const char *pos_80, const char *pos_70, const char *pos_60,
                                    const char *pos_50, const char *pos_40, const char *pos_30,
                                    const char *pos_20) {
    ck_assert_ptr_nonnull(pos_80);
    ck_assert_ptr_nonnull(pos_70);
    ck_assert_ptr_nonnull(pos_60);
    ck_assert_ptr_nonnull(pos_50);
    ck_assert_ptr_nonnull(pos_40);
    ck_assert_ptr_nonnull(pos_30);
    ck_assert_ptr_nonnull(pos_20);
}

static void check_positions_ordering(const char *pos_80, const char *pos_70, const char *pos_60,
                                     const char *pos_50, const char *pos_40, const char *pos_30,
                                     const char *pos_20) {
    ck_assert(pos_80 < pos_70);
    ck_assert(pos_70 < pos_60);
    ck_assert(pos_60 < pos_50);
    ck_assert(pos_50 < pos_40);
    ck_assert(pos_40 < pos_30);
    ck_assert(pos_30 < pos_20);
}

static void verify_rotated_tree_positions(const char *buffer_data) {
    // Find position of each expected element
    const char *pos_80 = strstr(buffer_data, "80\n");
    const char *pos_70 = strstr(buffer_data, "70\n");
    const char *pos_60 = strstr(buffer_data, "60\n");
    const char *pos_50 = strstr(buffer_data, "50\n");
    const char *pos_40 = strstr(buffer_data, "40\n");
    const char *pos_30 = strstr(buffer_data, "30\n");
    const char *pos_20 = strstr(buffer_data, "20\n");

    // Verify all elements were found
    check_positions_nonnull(pos_80, pos_70, pos_60, pos_50, pos_40, pos_30, pos_20);

    // Verify all elements were printed in the correct order (80 at top, 20 at bottom)
    check_positions_ordering(pos_80, pos_70, pos_60, pos_50, pos_40, pos_30, pos_20);
}

START_TEST(test_bst_retrieve_high_low_int) {
    BSTTree *tree = bst_create(compare_ints, print_int_to_buffer);
    int values[] = {50, 30, 70, 20, 40, 60, 80};

    for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); i++) {
        bst_insert(tree, &values[i]);
    }

    // Retrieve all values of the tree by printing them into a char buffer
    // this implementation serves as an example of a print function installed as retrieve callback.
    // (Technically we could have verified the order of each node without first printing to a
    // buffer)
    OutputBuffer buffer;
    ck_assert_int_eq(output_buffer_init(&buffer, 256), 1);
    g_output_buffer = &buffer;
    bst_retrieve_data_high_to_low(tree);
    g_output_buffer = NULL;

    // Verify the char buffer
    ck_assert_ptr_nonnull(buffer.data);
    printf(" === Retrieving data from BST (high to low) ===\n");
    printf("%s", buffer.data);
    printf("=== End of retrieval ===\n");

    verify_rotated_tree_positions(buffer.data);

    output_buffer_free(&buffer);
    bst_delete(tree);
}

// Test: Remove leaf node
START_TEST(test_remove_leaf) {
    BSTTree *tree = bst_create(compare_strings, print_string);
    bst_insert(tree, (void *)"Dublin");
    bst_insert(tree, (void *)"Cork");
    bst_insert(tree, (void *)"Galway");

    bst_remove(tree, (void *)"Cork");

    const void *found = bst_search(tree, (void *)"Cork");
    ck_assert_ptr_null(found);
    ck_assert_uint_eq(bst_count_nodes(tree), 2);

    bst_delete(tree);
}
END_TEST

// Test: Remove node with one child
START_TEST(test_remove_one_child) {
    BSTTree *tree = bst_create(compare_strings, print_string);
    bst_insert(tree, (void *)"Lisbon");
    bst_insert(tree, (void *)"Porto");
    bst_insert(tree, (void *)"Sintra");

    bst_remove(tree, (void *)"Porto");

    const void *found = bst_search(tree, (void *)"Porto");
    ck_assert_ptr_null(found);
    ck_assert_ptr_nonnull(bst_search(tree, (void *)"Sintra"));

    bst_delete(tree);
}
END_TEST

// Test: Remove node with two children
START_TEST(test_remove_two_children) {
    BSTTree *tree = bst_create(compare_strings, print_string);
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

    bst_delete(tree);
}
END_TEST

// Test: Remove root node
START_TEST(test_remove_root) {
    BSTTree *tree = bst_create(compare_strings, print_string);
    bst_insert(tree, (void *)"Warsaw");
    bst_remove(tree, (void *)"Warsaw");

    ck_assert_uint_eq(bst_count_nodes(tree), 0);
    bst_delete(tree);
}
END_TEST

// Test: Remove from empty tree
START_TEST(test_remove_empty) {
    BSTTree *tree = bst_create(compare_strings, print_string);
    bst_remove(tree, (void *)"Barcelona");
    ck_assert_uint_eq(bst_count_nodes(tree), 0);
    bst_delete(tree);
}
END_TEST

// Test: Remove non-existing city
START_TEST(test_remove_not_found) {
    BSTTree *tree = bst_create(compare_strings, print_string);
    bst_insert(tree, (void *)"Helsinki");
    bst_insert(tree, (void *)"Turku");

    bst_remove(tree, (void *)"Tampere");

    ck_assert_uint_eq(bst_count_nodes(tree), 2);

    bst_delete(tree);
}
END_TEST

// Test: Height of empty tree
START_TEST(test_height_empty) {
    BSTTree *tree = bst_create(compare_strings, print_string);
    int height = bst_height(tree);
    ck_assert_int_eq(height, -1);
    bst_delete(tree);
}
END_TEST

// Test: Height of single node
START_TEST(test_height_single) {
    BSTTree *tree = bst_create(compare_strings, print_string);
    bst_insert(tree, (void *)"Zurich");

    int height = bst_height(tree);
    ck_assert_int_eq(height, 0);

    bst_delete(tree);
}
END_TEST

// Test: Height of balanced tree
START_TEST(test_height_balanced) {
    BSTTree *tree = bst_create(compare_strings, print_string);
    bst_insert(tree, (void *)"Geneva");
    bst_insert(tree, (void *)"Basel");
    bst_insert(tree, (void *)"Lausanne");

    int height = bst_height(tree);
    ck_assert_int_eq(height, 1);

    bst_delete(tree);
}
END_TEST

// Test: Height of unbalanced tree
START_TEST(test_height_unbalanced) {
    BSTTree *tree = bst_create(compare_strings, print_string);
    bst_insert(tree, (void *)"A");
    bst_insert(tree, (void *)"B");
    bst_insert(tree, (void *)"C");
    bst_insert(tree, (void *)"D");

    int height = bst_height(tree);
    ck_assert_int_eq(height, 3);

    bst_delete(tree);
}
END_TEST

// Test: Width of empty tree
START_TEST(test_width_empty) {
    BSTTree *tree = bst_create(compare_strings, print_string);
    int width = bst_width(tree);
    ck_assert_int_eq(width, 0);
    bst_delete(tree);
}
END_TEST

// Test: Width of single node
START_TEST(test_width_single) {
    BSTTree *tree = bst_create(compare_strings, print_string);
    bst_insert(tree, (void *)"Zurich");

    int width = bst_width(tree);
    ck_assert_int_eq(width, 1);

    bst_delete(tree);
}
END_TEST

// Test: Width of tree with left and right children
START_TEST(test_width_balanced) {
    BSTTree *tree = bst_create(compare_strings, print_string);
    bst_insert(tree, (void *)"Geneva");
    bst_insert(tree, (void *)"Basel");
    bst_insert(tree, (void *)"Lausanne");

    // Tree structure:
    //       Geneva (0)
    //       /      \
    //    Basel   Lausanne
    //    (-1)       (1)
    // Width: 1 - (-1) + 1 = 3
    int width = bst_width(tree);
    ck_assert_int_eq(width, 3);

    bst_delete(tree);
}
END_TEST

// Test: Width of tree with only left children
START_TEST(test_width_left_skewed) {
    BSTTree *tree = bst_create(compare_strings, print_string);
    bst_insert(tree, (void *)"D");
    bst_insert(tree, (void *)"C");
    bst_insert(tree, (void *)"B");
    bst_insert(tree, (void *)"A");

    // Tree structure:
    //       D (0)
    //       /
    //      C (-1)
    //      /
    //     B (-2)
    //     /
    //    A (-3)
    // Width: 0 - (-3) + 1 = 4
    int width = bst_width(tree);
    ck_assert_int_eq(width, 4);

    bst_delete(tree);
}
END_TEST

// Test: Width of tree with only right children
START_TEST(test_width_right_skewed) {
    BSTTree *tree = bst_create(compare_strings, print_string);
    bst_insert(tree, (void *)"A");
    bst_insert(tree, (void *)"B");
    bst_insert(tree, (void *)"C");
    bst_insert(tree, (void *)"D");

    // Tree structure:
    //    A (0)
    //      \
    //       B (1)
    //         \
    //          C (2)
    //            \
    //             D (3)
    // Width: 3 - 0 + 1 = 4
    int width = bst_width(tree);
    ck_assert_int_eq(width, 4);

    bst_delete(tree);
}
END_TEST

// Test: Width of complex balanced tree
START_TEST(test_width_complex) {
    BSTTree *tree = bst_create(compare_ints, print_int);
    int values[] = {50, 30, 70, 20, 40, 60, 80};

    for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); i++) {
        bst_insert(tree, &values[i]);
    }

    // Tree structure:
    //            50 (0)
    //           /     \
    //         30 (-1)  70 (1)
    //        /  \      /  \
    //      20   40   60    80
    //     (-2) (0)  (0)   (2)
    // Min position: -2 (node 20)
    // Max position: 2 (node 80)
    // Width: 2 - (-2) + 1 = 5
    int width = bst_width(tree);
    ck_assert_int_eq(width, 5);

    bst_delete(tree);
}
END_TEST

// Test: Count nodes in empty tree
START_TEST(test_count_empty) {
    BSTTree *tree = bst_create(compare_strings, print_string);
    size_t count = bst_count_nodes(tree);
    ck_assert_uint_eq(count, 0);
    bst_delete(tree);
}
END_TEST

// Test: Count nodes in tree
START_TEST(test_count_nodes) {
    BSTTree *tree = bst_create(compare_strings, print_string);
    bst_insert(tree, (void *)"Lyon");
    bst_insert(tree, (void *)"Marseille");
    bst_insert(tree, (void *)"Nice");
    bst_insert(tree, (void *)"Toulouse");
    bst_insert(tree, (void *)"Nantes");

    size_t count = bst_count_nodes(tree);
    ck_assert_uint_eq(count, 5);

    bst_delete(tree);
}
END_TEST

// Test: Complex operations sequence
START_TEST(test_complex_operations) {
    BSTTree *tree = bst_create(compare_strings, print_string);

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

    bst_delete(tree);
}
END_TEST

// Test: Alphabetical ordering
START_TEST(test_alphabetical_order) {
    BSTTree *tree = bst_create(compare_strings, print_string);
    bst_insert(tree, (void *)"Munich");
    bst_insert(tree, (void *)"Berlin");
    bst_insert(tree, (void *)"Hamburg");
    bst_insert(tree, (void *)"Frankfurt");
    bst_insert(tree, (void *)"Cologne");

    // Check that Berlin (alphabetically first) is the minimum
    const void *min = bst_get_min(tree);
    ck_assert_str_eq((const char *)min, "Berlin");

    // Verify all cities are still accessible
    ck_assert_ptr_nonnull(bst_search(tree, (void *)"Munich"));
    ck_assert_ptr_nonnull(bst_search(tree, (void *)"Hamburg"));

    bst_delete(tree);
}
END_TEST

// Test: Delete tree
START_TEST(test_delete_tree) {
    BSTTree *tree = bst_create(compare_strings, print_string);
    bst_insert(tree, (void *)"Bern");
    bst_insert(tree, (void *)"Geneva");
    bst_insert(tree, (void *)"Basel");

    // Delete should free all memory without crashing
    bst_delete(tree);
    // If we reach here, deletion succeeded
}
END_TEST

// Test: Delete NULL tree
START_TEST(test_delete_null_tree) {
    // Should not crash
    bst_delete(NULL);
}
END_TEST

// Test: Integer data type support
START_TEST(test_insert_search_ints) {
    BSTTree *tree = bst_create(compare_ints, print_int);
    // Using a local variable is okay since we will only use it in this scope
    const int values[] = {42, 7, 19, 100, 3};

    for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); i++) {
        ck_assert_int_eq(bst_insert(tree, &values[i]), 0);
    }

    const int search_value = 19;
    const void *p_found = bst_search(tree, &search_value);
    ck_assert_ptr_nonnull(p_found);
    const int found = *(const int *)p_found;
    ck_assert_int_eq(found, search_value);

    const int expected_min = 3;
    const void *p_min = bst_get_min(tree);
    ck_assert_ptr_nonnull(p_min);
    const int min = *(const int *)p_min;
    ck_assert_int_eq(min, expected_min);

    bst_delete(tree);
}
END_TEST

// Test: Dump tree visualization
START_TEST(test_dump_tree) {
    BSTTree *tree = bst_create(compare_ints, print_int);

    // Create a balanced tree
    const int values[] = {50, 30, 70, 20, 40, 60, 80};
    for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); i++) {
        bst_insert(tree, &values[i]);
    }

    printf("\n=== Tree Dump (Balanced 7-node tree) ===\n");
    bst_dump_tree(tree);
    printf("=== End Tree Dump ===\n");

    bst_delete(tree);
}
END_TEST

// Test: Dump empty tree
START_TEST(test_dump_empty_tree) {
    BSTTree *tree = bst_create(compare_ints, print_int);
    printf("\n=== Tree Dump (Empty tree) ===\n");
    bst_dump_tree(tree);
    printf("=== End Tree Dump ===\n");

    bst_delete(tree);
}
END_TEST

// Test: Dump single node tree
START_TEST(test_dump_single_node) {
    BSTTree *tree = bst_create(compare_ints, print_int);
    const int value = 42;
    bst_insert(tree, &value);

    printf("\n=== Tree Dump (Single node) ===\n");
    bst_dump_tree(tree);
    printf("=== End Tree Dump ===\n");

    bst_delete(tree);
}
END_TEST

// Suite creation for Check framework
Suite *bst_suite(void) {
    Suite *s;
    TCase *tc_init;
    TCase *tc_insert;
    TCase *tc_search;
    TCase *tc_print;
    TCase *tc_remove;
    TCase *tc_metrics;
    TCase *tc_complex;
    TCase *tc_types;

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
    tcase_add_test(tc_search, test_find_max);
    tcase_add_test(tc_search, test_find_max_empty);
    suite_add_tcase(s, tc_search);

    // Print operations test case
    tc_print = tcase_create("Print Operations");
    tcase_add_test(tc_print, test_bst_retrieve_high_low);
    tcase_add_test(tc_print, test_bst_retrieve_high_low_int);
    tcase_add_test(tc_print, test_dump_tree);
    tcase_add_test(tc_print, test_dump_empty_tree);
    tcase_add_test(tc_print, test_dump_single_node);
    suite_add_tcase(s, tc_print);

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
    tcase_add_test(tc_metrics, test_width_empty);
    tcase_add_test(tc_metrics, test_width_single);
    tcase_add_test(tc_metrics, test_width_balanced);
    tcase_add_test(tc_metrics, test_width_left_skewed);
    tcase_add_test(tc_metrics, test_width_right_skewed);
    tcase_add_test(tc_metrics, test_width_complex);
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

    // Data type coverage test case
    tc_types = tcase_create("Data Types");
    tcase_add_test(tc_types, test_insert_search_ints);
    suite_add_tcase(s, tc_types);

    return s;
}

// Main test runner
int main(void) {
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
