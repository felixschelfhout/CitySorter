#ifndef BST_H
#define BST_H

#include <stddef.h>

/**
 * BST Node structure
 * Represents a single node in the Binary Search Tree containing a city name
 */
typedef struct BSTNode {
    char *city;              // City name (dynamically allocated)
    struct BSTNode *left;    // Left child (cities alphabetically before this city)
    struct BSTNode *right;   // Right child (cities alphabetically after this city)
} BSTNode;

/**
 * Create a new BST node with the given city name
 * @param city The city name (string will be duplicated)
 * @return Pointer to the newly created node, or NULL on failure
 */
BSTNode *bst_create_node(const char *city);

/**
 * Insert a city into the BST
 * @param root Pointer to the root of the BST (or NULL for empty tree)
 * @param city The city name to insert
 * @return Pointer to the root of the modified BST
 */
BSTNode *bst_insert(BSTNode *root, const char *city);

/**
 * Search for a city in the BST
 * @param root Pointer to the root of the BST
 * @param city The city name to search for
 * @return Pointer to the node containing the city, or NULL if not found
 */
BSTNode *bst_search(BSTNode *root, const char *city);

/**
 * Find the node with the minimum value (leftmost node)
 * @param root Pointer to the root of the BST
 * @return Pointer to the node with the minimum city name
 */
BSTNode *bst_find_min(BSTNode *root);

/**
 * Remove a city from the BST
 * @param root Pointer to the root of the BST
 * @param city The city name to remove
 * @return Pointer to the root of the modified BST
 */
BSTNode *bst_remove(BSTNode *root, const char *city);

/**
 * Print the BST in in-order traversal (alphabetically sorted)
 * @param root Pointer to the root of the BST
 */
void bst_print_inorder(BSTNode *root);

/**
 * Print the BST in a rotated format (right → root → left) for visualization
 * @param root Pointer to the root of the BST
 * @param space Spacing for indentation (use 0 initially)
 */
void bst_print_rotated(BSTNode *root, int space);

/**
 * Delete the entire BST and free all memory
 * @param root Pointer to the root of the BST
 */
void bst_delete_tree(BSTNode *root);

/**
 * Get the height of the BST
 * @param root Pointer to the root of the BST
 * @return The height of the tree (0 for single node, -1 for empty tree)
 */
int bst_height(BSTNode *root);

/**
 * Get the number of nodes in the BST
 * @param root Pointer to the root of the BST
 * @return The number of nodes in the tree
 */
size_t bst_count_nodes(BSTNode *root);

#endif // BST_H
