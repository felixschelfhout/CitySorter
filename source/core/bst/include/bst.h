#ifndef BST_H
#define BST_H

/**
 * @file bst.h
 * @brief Generic Binary Search Tree (BST) implementation for sorted data storage
 *
 * @defgroup BST Binary Search Tree
 * @{
 *
 * A generic Binary Search Tree implementation optimized for storing and retrieving sorted data.
 * The tree maintains ordering based on a user-provided comparison function and is designed to be
 * a read-only data structure after initial population. No modifications to data should occur
 * after insertion, as this could invalidate the tree's ordering guarantees.
 *
 * ## Design Philosophy
 * This BST is optimized for **lookup and retrieval**, not modification. The implementation
 * provides an opaque tree interface to prevent accidental corruption through direct manipulation
 * of internal nodes. Use it to:
 * - **Store sorted data**: Insert data and leverage the ordered structure
 * - **Retrieve data**: Search and traverse without modifying the tree
 * - **Display results**: Use print functions for visualization
 *
 * ## Features
 * - **Generic Data Storage**: Store any data type using `void*` pointers
 * - **Consistent Comparison**: Tree-wide consistent comparison function prevents invalid tree
 * states
 * - **Safe Access**: Opaque tree structure prevents accidental internal manipulation
 * - **Const Correctness**: Read-only access to retrieved data
 * - **Core Operations**: Insert, search, traverse and remove data
 * - **Tree Metrics**: Query height and node count
 * - **Flexible Output**: Custom print functions for data visualization
 *
 * ## Usage Example
 * @code
 * // Define a comparison function for your data type
 * int compare_ints(const void *a, const void *b) {
 *     int val_a = *(int *)a;
 *     int val_b = *(int *)b;
 *     return (val_a > val_b) - (val_a < val_b);
 * }
 *
 * // Define a print function for visualization
 * void print_int(const void *data) {
 *     printf("%d\n", *(const int *)data);
 * }
 *
 * // Create and populate the tree
 * BSTTree *tree = bst_create(compare_ints, print_int);
 * bst_insert(tree, (void *)&my_int);
 * bst_insert(tree, (void *)&another_int);
 *
 * // Search and retrieve data (read-only)
 * const void *found = bst_search(tree, (void *)&search_value);
 * if (found) {
 *     printf("Found: %d\n", *(const int *)found);
 *     // DO NOT modify the retrieved data!
 * }
 *
 * // Display sorted data
 * bst_print_inorder(tree);
 *
 * // Cleanup
 * bst_delete(tree, NULL);
 * @endcode
 *
 * ## Memory Management
 * - **Tree Structure**: Automatically managed by the BST
 * - **Data**: User is responsible for allocating and freeing data pointers
 * - **Custom Cleanup**: Optional free function can be provided to `bst_delete()` for data cleanup
 *
 * ## Safety Guarantees
 * - **Read-Only Access**: Use tree for retrieval only; do not modify data after insertion
 * - **Opaque Structure**: Internal tree nodes are hidden from direct access
 * - **Const Correctness**: Returned data is marked const to encourage read-only usage
 *
 * @see bst_create()
 * @see bst_insert()
 * @see bst_remove()
 * @see bst_search()
 * @see bst_delete()
 *
 * @}
 */

#include <stddef.h>

/**
 * Comparison function type for generic BST operations
 * @param a First data pointer
 * @param b Second data pointer
 * @return negative if a < b, 0 if a == b, positive if a > b
 */
typedef int (*bst_compare_fn)(const void *a, const void *b);
/**
 * Callback function type for node retrieval operations
 * @param data The data pointer of the retrieved node
 */
typedef void (*data_retrieved_cb)(const void *data);

/**
 * @brief Opaque Binary Search Tree structure
 *
 * The internal structure of the tree is hidden to prevent accidental corruption.
 * Users interact with the tree only through the public API functions.
 */
typedef struct BSTTree BSTTree;

/**
 * Create a new BST with a given comparison function
 * @param compare Comparison function for ordering data in the tree
 * @param data_retrieved_cb Callback function invoked when a node is retrieved
 * @return Pointer to the newly created tree, or NULL on failure
 */
BSTTree *bst_create(bst_compare_fn compare, data_retrieved_cb data_retrieved_cb);

/**
 * Delete the entire BST without reordering the tree.
 * @param tree Pointer to the BST
 */
void bst_delete(BSTTree *tree);

/**
 * Insert data into the BST
 * @param tree Pointer to the BST
 * @param data The data pointer to insert (const for safety)
 * @return true if insertion was successful, false if data already exists or error
 */
int bst_insert(BSTTree *tree, const void *data);

/**
 * Remove data from the BST and reorder the tree to maintain BST properties.
 * @param tree Pointer to the BST
 * @param data The data pointer to remove
 * @return true if removal was successful, false if data not found
 */
int bst_remove(BSTTree *tree, const void *data);

/**
 * Search for data in the BST
 * @param tree Pointer to the BST
 * @param data The data to search for
 * @return Constant pointer to the data if found, NULL if not found
 *         The returned pointer is const to prevent accidental modification
 */
const void *bst_search(BSTTree *tree, const void *data);

/**
 * Find the minimum value in the BST (leftmost node, read-only)
 * @param tree Pointer to the BST
 * @return Constant pointer to the data with the minimum value, NULL if tree is empty
 *         The returned pointer is const to prevent accidental modification
 */
const void *bst_get_min(BSTTree *tree);

/**
 * Find the maximum value in the BST (rightmost node, read-only)
 * @param tree Pointer to the BST
 * @return Constant pointer to the data with the maximum value, NULL if tree is empty
 *         The returned pointer is const to prevent accidental modification
 */
const void *bst_get_max(BSTTree *tree);

/**
 * Calls the registered data_retrieved_cb function for each node of the tree, starting with the
 * rightmost node (highest) and ending with the leftmost (lowest) node.
 *
 * @param tree Pointer to the BST
 */
void bst_retrieve_data_high_to_low(BSTTree *tree);

/**
 * Get the height of the BST
 * @param tree Pointer to the BST
 * @return The height of the tree (0 for single node, -1 for empty tree)
 */
int bst_height(BSTTree *tree);

/**
 * Get the width of the BST
 * @param tree Pointer to the BST
 * @return The horizontal span of the tree (distance from leftmost to rightmost node + 1, 0 for
 * empty tree)
 */
int bst_width(BSTTree *tree);

/**
 * Get the number of nodes in the BST
 * @param tree Pointer to the BST
 * @return The number of nodes in the tree
 */
size_t bst_count_nodes(BSTTree *tree);

#endif // BST_H
