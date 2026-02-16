#include "bst.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
 * Create a new BST node with the given city name
 */
BSTNode *bst_create_node(const char *city) {
    if (!city) {
        return NULL;
    }

    BSTNode *node = (BSTNode *)malloc(sizeof(BSTNode));
    if (!node) {
        return NULL;
    }

    node->city = (char *)malloc(strlen(city) + 1);
    if (!node->city) {
        free(node);
        return NULL;
    }

    strcpy(node->city, city);
    node->left = NULL;
    node->right = NULL;

    return node;
}

/**
 * Insert a city into the BST
 */
BSTNode *bst_insert(BSTNode *root, const char *city) {
    if (!city) {
        return root;
    }

    // Base case: empty tree
    if (root == NULL) {
        return bst_create_node(city);
    }

    // Compare city with root's city
    int cmp = strcmp(city, root->city);

    if (cmp < 0) {
        // Insert into left subtree
        root->left = bst_insert(root->left, city);
    } else if (cmp > 0) {
        // Insert into right subtree
        root->right = bst_insert(root->right, city);
    }
    // If cmp == 0, the city already exists, so don't insert duplicates

    return root;
}

/**
 * Search for a city in the BST
 */
BSTNode *bst_search(BSTNode *root, const char *city) {
    if (!root || !city) {
        return NULL;
    }

    int cmp = strcmp(city, root->city);

    if (cmp == 0) {
        return root;
    } else if (cmp < 0) {
        return bst_search(root->left, city);
    } else {
        return bst_search(root->right, city);
    }
}

/**
 * Find the node with the minimum value (leftmost node)
 */
BSTNode *bst_find_min(BSTNode *root) {
    if (!root) {
        return NULL;
    }

    while (root->left != NULL) {
        root = root->left;
    }

    return root;
}

/**
 * Remove a city from the BST
 */
BSTNode *bst_remove(BSTNode *root, const char *city) {
    if (!root || !city) {
        return root;
    }

    int cmp = strcmp(city, root->city);

    if (cmp < 0) {
        root->left = bst_remove(root->left, city);
    } else if (cmp > 0) {
        root->right = bst_remove(root->right, city);
    } else {
        // Node to be deleted found

        // Case 1: Node has no children (leaf node)
        if (root->left == NULL && root->right == NULL) {
            free(root->city);
            free(root);
            return NULL;
        }

        // Case 2: Node has only one child
        if (root->left == NULL) {
            BSTNode *temp = root->right;
            free(root->city);
            free(root);
            return temp;
        }

        if (root->right == NULL) {
            BSTNode *temp = root->left;
            free(root->city);
            free(root);
            return temp;
        }

        // Case 3: Node has two children
        // Find the in-order successor (smallest node in right subtree)
        BSTNode *successor = bst_find_min(root->right);

        // Replace root's city with successor's city
        free(root->city);
        root->city = (char *)malloc(strlen(successor->city) + 1);
        if (root->city) {
            strcpy(root->city, successor->city);
        }

        // Delete the successor node
        root->right = bst_remove(root->right, successor->city);
    }

    return root;
}

/**
 * Print the BST in in-order traversal (alphabetically sorted)
 */
void bst_print_inorder(BSTNode *root) {
    if (root == NULL) {
        return;
    }

    bst_print_inorder(root->left);
    printf("%s\n", root->city);
    bst_print_inorder(root->right);
}

/**
 * Print the BST in a rotated format (right → root → left) for visualization
 */
void bst_print_rotated(BSTNode *root, int space) {
    if (root == NULL) {
        return;
    }

    // Increase space for next level
    space += 5;

    // Process right subtree
    bst_print_rotated(root->right, space);

    // Print current node
    printf("\n");
    for (int i = 5; i < space; i++) {
        printf(" ");
    }
    printf("%s\n", root->city);

    // Process left subtree
    bst_print_rotated(root->left, space);
}

/**
 * Delete the entire BST and free all memory
 */
void bst_delete_tree(BSTNode *root) {
    if (root == NULL) {
        return;
    }

    // Recursively delete left and right subtrees
    bst_delete_tree(root->left);
    bst_delete_tree(root->right);

    // Free the node itself
    free(root->city);
    free(root);
}

/**
 * Get the height of the BST
 */
int bst_height(BSTNode *root) {
    if (root == NULL) {
        return -1;
    }

    int left_height = bst_height(root->left);
    int right_height = bst_height(root->right);

    return 1 + (left_height > right_height ? left_height : right_height);
}

/**
 * Get the number of nodes in the BST
 */
size_t bst_count_nodes(BSTNode *root) {
    if (root == NULL) {
        return 0;
    }

    return 1 + bst_count_nodes(root->left) + bst_count_nodes(root->right);
}
