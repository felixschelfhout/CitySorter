#include "bst.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ============================================================================
// PRIVATE TYPE DEFINITIONS
// ============================================================================

/**
 * @brief BST Node structure (opaque, internal use only)
 *
 * Represents a single node in the Binary Search Tree.
 * This structure is not exposed in the public API to prevent accidental corruption.
 */
typedef struct BSTNode {
    const void *data;      // Generic data pointer (user-managed memory, const for safety)
    struct BSTNode *left;  // Left child (data ordered according to comparison function)
    struct BSTNode *right; // Right child (data ordered according to comparison function)
} BSTNode;

/**
 * @brief BST Tree structure (opaque, internal use only)
 *
 * Encapsulates a Binary Search Tree with a consistent comparison function.
 * The tree-wide comparison function ensures the tree structure remains valid.
 */
struct BSTTree {
    BSTNode *root_node;     // Root node of the tree
    bst_compare_fn compare; // Comparison function to compare data elements for ordering in the tree
    data_retrieved_cb data_retrieved_cb; // Callback function invoked when a node is retrieved
};

// ============================================================================
// STATIC HELPER FUNCTIONS (Internal Use Only)
// ============================================================================

/**
 * Create a new BST node with the given data pointer
 */
static BSTNode *create_node(const void *data) {
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
 * Insert data into the BST (recursive helper)
 */
static BSTNode *insert_recursive(BSTNode *node, const void *data, bst_compare_fn compare) {
    if (node == NULL) {
        // Empty spot found, create a new node
        return create_node(data);
    }

    // Walk the tree according to the comparison function until an empty spot is found
    int cmp = compare(data, node->data);

    if (cmp < 0) {
        node->left = insert_recursive(node->left, data, compare);
    } else if (cmp > 0) {
        node->right = insert_recursive(node->right, data, compare);
    }
    // If cmp == 0, the data already exists, so don't insert duplicates

    return node;
}

/**
 * Search for data in the BST (recursive helper)
 */
static BSTNode *search_recursive(BSTNode *node, const void *data, bst_compare_fn compare) {
    if (!node || !data) {
        return NULL;
    }

    int cmp = compare(data, node->data);

    if (cmp == 0) {
        return node;
    }
    if (cmp < 0) {
        return search_recursive(node->left, data, compare);
    }
    return search_recursive(node->right, data, compare);
}

/**
 * Remove data from the BST (recursive helper).
 * This reorders the tree to maintain the BST properties after deletion.
 */
static BSTNode *remove_recursive(BSTNode *node, const void *data, bst_compare_fn compare) {
    if (!node || !data) {
        return node;
    }

    int cmp = compare(data, node->data);

    if (cmp < 0) {
        node->left = remove_recursive(node->left, data, compare);
    } else if (cmp > 0) {
        node->right = remove_recursive(node->right, data, compare);
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

        // Delete the successor node, it may have right nodes but never left nodes
        node->right = remove_recursive(node->right, successor->data, compare);
    }

    return node;
}

/**
 * Retrieve high-low in the BST (recursive helper)
 */
static void retrieve_high_low_recursive(BSTNode *node, data_retrieved_cb data_retrieved_cb) {
    if (node == NULL || !data_retrieved_cb) {
        return;
    }

    // Process right subtree of current node
    retrieve_high_low_recursive(node->right, data_retrieved_cb);

    // Call the callback for current node
    data_retrieved_cb(node->data);

    // Process left subtree of current node
    retrieve_high_low_recursive(node->left, data_retrieved_cb);
}

/**
 * Delete the entire BST (recursive helper) without reordering the nodes.
 */
static void delete_tree_recursive(BSTNode *node) {
    if (node == NULL) {
        return;
    }

    // Recursively delete left and right subtrees
    delete_tree_recursive(node->left);
    delete_tree_recursive(node->right);

    // Free the node itself
    free(node);
}

/**
 * Get the height of the BST (recursive helper)
 */
static int height_recursive(BSTNode *node) {
    if (node == NULL) {
        return -1;
    }

    int left_height = height_recursive(node->left);
    int right_height = height_recursive(node->right);

    return 1 + (left_height > right_height ? left_height : right_height);
}

/**
 * Get the number of nodes in the BST (recursive helper)
 */
static size_t count_nodes_recursive(BSTNode *node) {
    if (node == NULL) {
        return 0;
    }

    // current node + left subtree + right subtree
    return 1 + count_nodes_recursive(node->left) + count_nodes_recursive(node->right);
}

/**
 * Get the width of the BST by tracking horizontal distances (recursive helper)
 * Walks the entire tree and tracks min/max horizontal positions from root
 */
static void width_recursive_helper(BSTNode *node, int distance, int *min_distance,
                                   int *max_distance) {
    if (node == NULL) {
        return;
    }

    // Update min and max distances
    if (distance < *min_distance) {
        *min_distance = distance;
    }
    if (distance > *max_distance) {
        *max_distance = distance;
    }

    // Traverse left (distance - 1) and right (distance + 1)
    width_recursive_helper(node->left, distance - 1, min_distance, max_distance);
    width_recursive_helper(node->right, distance + 1, min_distance, max_distance);
}

/**
 * Get the width of the BST (wrapper for recursive helper)
 */
static int width_recursive(BSTNode *node) {
    if (node == NULL) {
        return 0;
    }

    int min_distance = 0;
    int max_distance = 0;

    width_recursive_helper(node, 0, &min_distance, &max_distance);

    // Width is the span from leftmost to rightmost node
    return max_distance - min_distance + 1;
}

// ============================================================================
// PUBLIC API (Functions declared in bst.h)
// ============================================================================

/**
 * Create a new BST with a given comparison function
 */
BSTTree *bst_create(bst_compare_fn compare, data_retrieved_cb data_retrieved_cb) {
    if (!compare) {
        return NULL;
    }
    if (!data_retrieved_cb) {
        return NULL;
    }

    BSTTree *tree = (BSTTree *)malloc(sizeof(BSTTree));
    if (!tree) {
        return NULL;
    }

    tree->root_node = NULL;
    tree->compare = compare;
    tree->data_retrieved_cb = data_retrieved_cb;

    return tree;
}

/**
 * Insert data into the BST
 * return -1 if insertion failed (e.g. data already exists or error), 0 if successful
 */
int bst_insert(BSTTree *tree, const void *data) {
    if (!tree || !data) {
        return -1;
    }

    tree->root_node = insert_recursive(tree->root_node, data, tree->compare);
    return 0;
}

/**
 * Search for data in the BST
 */
const void *bst_search(BSTTree *tree, const void *data) {
    if (!tree || !data) {
        return NULL;
    }

    BSTNode *node = search_recursive(tree->root_node, data, tree->compare);
    return node ? node->data : NULL;
}

/**
 * Find the node with the minimum value (leftmost node)
 */
const void *bst_get_min(BSTTree *tree) {
    if (!tree || !tree->root_node) {
        return NULL;
    }

    BSTNode *node = tree->root_node;
    while (node->left != NULL) {
        node = node->left;
    }

    return node->data;
}

/**
 * Find the node with the maximum value (rightmost node)
 */
const void *bst_get_max(BSTTree *tree) {
    if (!tree || !tree->root_node) {
        return NULL;
    }

    BSTNode *node = tree->root_node;
    while (node->right != NULL) {
        node = node->right;
    }

    return node->data;
}

/**
 * Remove data from the BST
 */
int bst_remove(BSTTree *tree, const void *data) {
    if (!tree || !data) {
        return 0;
    }

    const BSTNode *old_root_node = tree->root_node;
    tree->root_node = remove_recursive(tree->root_node, data, tree->compare);
    return old_root_node != tree->root_node ||
           (tree->root_node != NULL && tree->compare(data, tree->root_node->data) == 0);
}

/**
 * Calls the registered callback for each node starting with the
 * rightmost node (highest) and ending with the leftmost (lowest) node.
 */
void bst_retrieve_data_high_to_low(BSTTree *tree) {
    if (!tree || !tree->data_retrieved_cb) {
        return;
    }

    retrieve_high_low_recursive(tree->root_node, tree->data_retrieved_cb);
}

/**
 * Delete the entire BST and free all memory for nodes
 */
void bst_delete(BSTTree *tree) {
    if (!tree) {
        return;
    }

    delete_tree_recursive(tree->root_node);
    free(tree);
}

/**
 * Get the height of the BST
 */
int bst_height(BSTTree *tree) {
    if (!tree) {
        return -1;
    }
    return height_recursive(tree->root_node);
}

/**
 * Get the width of the BST
 */
int bst_width(BSTTree *tree) {
    if (!tree) {
        return 0;
    }
    return width_recursive(tree->root_node);
}

/**
 * Get the number of nodes in the BST
 */
size_t bst_count_nodes(BSTTree *tree) {
    if (!tree) {
        return 0;
    }
    return count_nodes_recursive(tree->root_node);
}

/**
 * Render a single level of nodes in the tree visualization
 */
static void dump_render_nodes(BSTNode **nodes, size_t level_count, size_t width) {
    char *line = (char *)malloc(width + 1);
    if (!line) {
        return;
    }
    memset(line, ' ', width);
    line[width] = '\0';

    size_t segment = width / level_count;
    for (size_t i = 0; i < level_count; i++) {
        if (!nodes[i]) {
            continue;
        }
        size_t pos = segment / 2 + segment * i;
        if (pos < width) {
            line[pos] = 'o';
        }
    }

    printf("%s\n", line);
    free(line);
}

/**
 * Render edges between current and next level in the tree visualization
 */
static void dump_render_edges(BSTNode **nodes, size_t level_count, size_t width) {
    char *edges = (char *)malloc(width + 1);
    if (!edges) {
        return;
    }
    memset(edges, ' ', width);
    edges[width] = '\0';

    size_t segment = width / level_count;
    size_t next_segment = segment / 2;
    for (size_t i = 0; i < level_count; i++) {
        const BSTNode *node = nodes[i];
        size_t parent_pos = segment / 2 + segment * i;

        if (node) {
            if (node->left) {
                size_t left_pos = next_segment / 2 + next_segment * (i * 2);
                size_t edge_pos = (parent_pos + left_pos) / 2;
                if (edge_pos < width) {
                    edges[edge_pos] = '/';
                }
            }
            if (node->right) {
                size_t right_pos = next_segment / 2 + next_segment * (i * 2 + 1);
                size_t edge_pos = (parent_pos + right_pos) / 2;
                if (edge_pos < width) {
                    edges[edge_pos] = '\\';
                }
            }
        }
    }

    printf("%s\n", edges);
    free(edges);
}

/**
 * Dump the tree shape to stdout using a simple ASCII layout.
 */
void bst_dump_tree(BSTTree *tree) {
    const size_t terminal_width = 120;

    if (!tree || !tree->root_node) {
        printf("(empty)\n");
        return;
    }

    int max_level = height_recursive(tree->root_node);
    if (max_level < 0) {
        printf("(empty)\n");
        return;
    }

    size_t width = 1;
    for (int i = 0; i < max_level + 2; i++) {
        if (width > terminal_width / 2) {
            printf("...\n");
            return;
        }
        width *= 2;
    }

    if (width > terminal_width) {
        printf("...\n");
        return;
    }

    size_t level_count = 1;
    BSTNode **current = (BSTNode **)malloc(level_count * sizeof(BSTNode *));
    if (!current) {
        return;
    }
    current[0] = tree->root_node;

    for (int level = 0; level <= max_level; level++) {
        dump_render_nodes(current, level_count, width);

        if (level == max_level) {
            break;
        }

        size_t next_count = level_count * 2;
        BSTNode **next = (BSTNode **)malloc(next_count * sizeof(BSTNode *));
        if (!next) {
            free((void *)current);
            return;
        }
        for (size_t i = 0; i < next_count; i++) {
            next[i] = NULL;
        }

        dump_render_edges(current, level_count, width);

        // Advance to next level
        for (size_t i = 0; i < level_count; i++) {
            if (current[i]) {
                next[i * 2] = current[i]->left;
                next[i * 2 + 1] = current[i]->right;
            }
        }

        free((void *)current);
        current = next;
        level_count = next_count;
    }

    free((void *)current);
}
