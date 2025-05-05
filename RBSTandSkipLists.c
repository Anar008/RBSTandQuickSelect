#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define M 1000000
#define MAX_LEVEL 16
#define P 0.5

// --- Randomized BST ---

typedef struct RBSTNode {
    int key;
    struct RBSTNode *left, *right;
} RBSTNode;

RBSTNode* create_rbst_node(int key) {
    RBSTNode* node = (RBSTNode*)malloc(sizeof(RBSTNode));
    node->key = key;
    node->left = node->right = NULL;
    return node;
}

int size(RBSTNode* node) {
    if (!node) return 0;
    return 1 + size(node->left) + size(node->right);
}

RBSTNode* rotate_left(RBSTNode* x) {
    RBSTNode* y = x->right;
    x->right = y->left;
    y->left = x;
    return y;
}

RBSTNode* rotate_right(RBSTNode* y) {
    RBSTNode* x = y->left;
    y->left = x->right;
    x->right = y;
    return x;
}

RBSTNode* insert_root(RBSTNode* node, int key) {
    if (!node) return create_rbst_node(key);
    if (key < node->key) {
        node->left = insert_root(node->left, key);
        return rotate_right(node);
    } else {
        node->right = insert_root(node->right, key);
        return rotate_left(node);
    }
}

RBSTNode* rbst_insert(RBSTNode* root, int key) {
    if (!root) return create_rbst_node(key);
    if (rand() % (size(root) + 1) == 0)
        return insert_root(root, key);
    if (key < root->key)
        root->left = rbst_insert(root->left, key);
    else
        root->right = rbst_insert(root->right, key);
    return root;
}

int rbst_search(RBSTNode* root, int key, int* steps) {
    *steps = 0;
    while (root) {
        (*steps)++;
        if (key == root->key) return 1;
        root = (key < root->key) ? root->left : root->right;
    }
    return 0;
}

// --- Skip List ---

typedef struct SkipNode {
    int key;
    struct SkipNode** forward;
} SkipNode;

typedef struct SkipList {
    int level;
    SkipNode* header;
} SkipList;

SkipNode* create_skip_node(int key, int level) {
    SkipNode* node = (SkipNode*)malloc(sizeof(SkipNode));
    node->key = key;
    node->forward = (SkipNode**)malloc(sizeof(SkipNode*) * (level + 1));
    for (int i = 0; i <= level; ++i)
        node->forward[i] = NULL;
    return node;
}

SkipList* create_skip_list() {
    SkipList* list = (SkipList*)malloc(sizeof(SkipList));
    list->level = 0;
    list->header = create_skip_node(-1, MAX_LEVEL);
    return list;
}

int random_level() {
    int lvl = 0;
    while ((double)rand() / RAND_MAX < P && lvl < MAX_LEVEL)
        lvl++;
    return lvl;
}

void skip_insert(SkipList* list, int key) {
    SkipNode* update[MAX_LEVEL + 1];
    SkipNode* x = list->header;
    for (int i = list->level; i >= 0; i--) {
        while (x->forward[i] && x->forward[i]->key < key)
            x = x->forward[i];
        update[i] = x;
    }
    x = x->forward[0];
    if (!x || x->key != key) {
        int lvl = random_level();
        if (lvl > list->level) {
            for (int i = list->level + 1; i <= lvl; i++)
                update[i] = list->header;
            list->level = lvl;
        }
        x = create_skip_node(key, lvl);
        for (int i = 0; i <= lvl; i++) {
            x->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = x;
        }
    }
}

int skip_search(SkipList* list, int key, int* steps) {
    *steps = 0;
    SkipNode* x = list->header;
    for (int i = list->level; i >= 0; i--) {
        while (x->forward[i] && x->forward[i]->key < key) {
            x = x->forward[i];
            (*steps)++;
        }
        (*steps)++;
    }
    x = x->forward[0];
    return (x && x->key == key);
}

void benchmark(int n) {
    printf("\nn = %d\n", n);
    int* data = (int*)malloc(n * sizeof(int));
    int* queries = (int*)malloc(M * sizeof(int));
    for (int i = 0; i < n; i++)
        data[i] = rand();

    for (int i = 0; i < M / 2; i++) {
        queries[i] = data[rand() % n];
    }
    for (int i = M / 2; i < M; i++) {
        queries[i] = rand();
    }

    RBSTNode* bst = NULL;
    SkipList* skip = create_skip_list();

    for (int i = 0; i < n; i++) {
        bst = rbst_insert(bst, data[i]);
        skip_insert(skip, data[i]);
    }

    long long total_bst_steps = 0;
    long long total_skip_steps = 0;

    for (int i = 0; i < M; i++) {
        int steps_bst, steps_skip;
        rbst_search(bst, queries[i], &steps_bst);
        skip_search(skip, queries[i], &steps_skip);
        total_bst_steps += steps_bst;
        total_skip_steps += steps_skip;
    }

    printf("RBST: %.2f\n", total_bst_steps / (double)M);
    printf("Skip: %.2f\n", total_skip_steps / (double)M);

    free(data);
    free(queries);
}

int main() {
    srand(time(NULL));
    int sizes[] = {5000, 10000, 20000, 50000, 100000};
    for (int i = 0; i < 5; i++)
        benchmark(sizes[i]);
    return 0;
}
