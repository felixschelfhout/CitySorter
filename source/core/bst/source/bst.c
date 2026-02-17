#include "bst.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * @brief BST Node structure (opaque, internal use only)
 * 
 * Represents a single node in the Binary Search Tree.
 * This structure is not exposed in the public API to prevent accidental corruption.
 */
typedef struct BSTNode {
    const void *data;        // Generic data pointer (user-managed memory, const for safety)
    struct BSTNode *left;    // Left child (data ordered according to comparison function)
    struct BSTNode *right;   // Right child (data ordered according to comparison function)
} BSTNode;

/**
 * @brief BST Tree structure (opaque, internal use only)
 * 
 * Encapsulates a Binary Search Tree with a consistent comparison function.
 * The tree-wide comparison function ensures the tree structure remains valid.
 */
struct BSTTree {
    BSTNode *root;           // Root of the tree
    bst_compare_fn compare;  // Comparison function to compare data elements for ordering in the tree
};

/**
 * Create a new BST node with the given data pointer
 */
static BSTNode *_bst_create_node(const void *data) {
    BSTNode *node = (BSTNode *)malloc(sizeof(BSTNode));
    if (!node) {
        return NULL;
    }

    node->data = data;
    node->left = NULL;
    node->right = NULL;

    return node;
}

/**
 * Create a new BST with a given comparison function
 */
BSTTree *bst_create(bst_compare_fn compare) {
    if (!compare) {
        return NULL;
    }

    BSTTree *tree = (BSTTree *)malloc(sizeof(BSTTree));
    if (!tree) {
        return NULL;
    }

    tree->root = NULL;
    tree->compare = compare;

    return tree;
}

/**
 * Insert data into the BST (recursive helper)
 */
static BSTNode *_bst_insert_recursive(BSTNode *node, const void *data, bst_compare_fn compare) {
    if (node == NULL) {
        // Empty spot found, create a new node
        return _bst_create_node(data);
    }

    // Walk the tree according to the comparison function until an empty spot is found
    int cmp = compare(data, node->data);

    if (cmp < 0) {
        node->left = _bst_insert_recursive(node->left, data, compare);
    } else if (cmp > 0) {
        node->right = _bst_insert_recursive(node->right, data, compare);
    }
    // If cmp == 0, the data already exists, so don't insert duplicates

    return node;
}

/**
 * Insert data into the BST
 * return -1 if insertion failed (e.g. data already exists or error), 0 if successful
 */
int bst_insert(BSTTree *tree, const void *data) {
    if (!tree || !data) {
        return -1;
    }

    tree->root = _bst_insert_recursive(tree->root, data, tree->compare);
    return 0;
}

/**
 * Search for data in the BST (recursive helper)
 */
static BSTNode *_bst_search_recursive(BSTNode *node, const void *data, bst_compare_fn compare) {
    if (!node || !data) {
        return NULL;
    }

    int cmp = compare(data, node->data);

    if (cmp == 0) {
        return node;
    } else if (cmp < 0) {
        return _bst_search_recursive(node->left, data, compare);
    } else {
        return _bst_search_recursive(node->right, data, compare);
    }
}

/**
 * Search for data in the BST
 */
const void *bst_search(BSTTree *tree, const void *data) {
    if (!tree || !data) {
        return NULL;
    }

    BSTNode *node = _bst_search_recursive(tree->root, data, tree->compare);
    return node ? node->data : NULL;
}

/**
 * Find the node with the minimum value (leftmost node)
 */
const void *bst_find_min(BSTTree *tree) {
    if (!tree || !tree->root) {
        return NULL;
    }

    BSTNode *node = tree->root;
    while (node->left != NULL) {
        node = node->left;
    }

    return node->data;
}

/**
 * Remove data from the BST (recursive helper)
 */
static BSTNode *_bst_remove_recursive(BSTNode *node, const void *data, bst_compare_fn compare) {
    if (!node || !data) {
        return node;
    }

    int cmp = compare(data, node->data);

    if (cmp < 0) {
        node->left = _bst_remove_recursive(node->left, data, compare);
    } else if (cmp > 0) {
        node->right = _bst_remove_recursive(node->right, data, compare);
    } else {
        // Node to be deleted found

        // Case 1: Node has no children (leaf node)
        if (node->left == NULL && node->right == NULL) {
            free(node);
            return NULL;
        }

        // Case 2: Node has only one child
        if (node->left == NULL) {
            BSTNode *temp = node->right;
            free(node);
            return temp;
        }

        if (node->right == NULL) {
            BSTNode *temp = node->left;
            free(node);
            return temp;
        }

        // Case 3: Node has two children
        // Find the in-order successor (smallest node in right subtree)
        BSTNode *successor = node->right;
        while (successor->left != NULL) {
            successor = successor->left;
        }

        // Replace root's data with successor's data
        node->data = successor->data;

        // Delete the successor node
        node->right = _bst_remove_recursive(node->right, successor->data, compare);
    }

    return node;
}

/**
 * Remove data from the BST
 */
int bst_remove(BSTTree *tree, const void *data) {
    if (!tree || !data) {
        return 0;
    }

    BSTNode *old_root = tree->root;
    tree->root = _bst_remove_recursive(tree->root, data, tree->compare);
    return old_root != tree->root || (tree->root != NULL && tree->compare(data, tree->root->data) == 0);
}

/**
 * Print the BST in in-order traversal using a print function (recursive helper)
 */
static void _bst_print_inorder_recursive(BSTNode *root, void (*print)(const void *)) {
    if (root == NULL || !print) {
        return;
    }

    _bst_print_inorder_recursive(root->left, print);
    print(root->data);
    _bst_print_inorder_recursive(root->right, print);
}

/**
 * Print the BST in in-order traversal using a print function
 */
void bst_print_inorder(BSTTree *tree, void (*print)(const void *)) {
    if (!tree || !print) {
        return;
    }

    _bst_print_inorder_recursive(tree->root, print);
}

/**
 * Print the BST in a rotated format (right → root → left) for visualization (recursive helper)
 */
static void _bst_print_rotated_recursive(BSTNode *root, int space, void (*print)(const void *)) {
    if (root == NULL || !print) {
        return;
    }

    // Increase space for next level
    space += 5;

    // Process right subtree
    _bst_print_rotated_recursive(root->right, space, print);

    // Print current node
    printf("\n");
    for (int i = 5; i < space; i++) {
        printf(" ");
    }
    print(root->data);

    // Process left subtree
    _bst_print_rotated_recursive(root->left, space, print);
}

/**
 * Print the BST in a rotated format (right → root → left) for visualization
 */
void bst_print_rotated(BSTTree *tree, int space, void (*print)(const void *)) {
    if (!tree || !print) {
        return;
    }

    _bst_print_rotated_recursive(tree->root, space, print);
}

/**
 * Delete the entire BST and free all memory for nodes (recursive helper)
 */
static void _bst_delete_tree_recursive(BSTNode *root, void (*free_data)(void *)) {
    if (root == NULL) {
        return;
    }

    // Recursively delete left and right subtrees
    _bst_delete_tree_recursive(root->left, free_data);
    _bst_delete_tree_recursive(root->right, free_data);

    // Free the node itself
    free(root);
}

/**
 * Delete the entire BST and free all memory for nodes
 */
void bst_delete(BSTTree *tree, void (*free_data)(void *)) {
    if (!tree) {
        return;
    }

    _bst_delete_tree_recursive(tree->root, free_data);
    free(tree);
}

/**
 * Get the height of the BST (recursive helper)
 */
static int _bst_height_recursive(BSTNode *root) {
    if (root == NULL) {
        return -1;
    }

    int left_height = _bst_height_recursive(root->left);
    int right_height = _bst_height_recursive(root->right);

    return 1 + (left_height > right_height ? left_height : right_height);
}

/**
 * Get the height of the BST
 */
int bst_height(BSTTree *tree) {
    if (!tree) {
        return -1;
    }
    return _bst_height_recursive(tree->root);
}

/**
 * Get the number of nodes in the BST (recursive helper)
 */
static size_t _bst_count_nodes_recursive(BSTNode *root) {
    if (root == NULL) {
        return 0;
    }

    return 1 + _bst_count_nodes_recursive(root->left) + _bst_count_nodes_recursive(root->right);
}

/**
 * Get the number of nodes in the BST
 */
size_t bst_count_nodes(BSTTree *tree) {
    if (!tree) {
        return 0;
    }
    return _bst_count_nodes_recursive(tree->root);
}
