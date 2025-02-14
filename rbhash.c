#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define HASHSIZE 999983

typedef struct rb_node {
    long id;
    long score;
    int color;
    int size;
    struct rb_node *left;
    struct rb_node *right;
    struct rb_node *parent;
} rb_node;

typedef struct hash_node {
    long id;
    long score;
    struct hash_node *next;
} hash_node;

long hash_function(long key) {
    return key % HASHSIZE;
}

void insert_hash(hash_node *hash_table[], long id, long score) {
    long index = hash_function(id);
    hash_node *new_node = (hash_node *)calloc(1, sizeof(hash_node));
    new_node->id = id;
    new_node->score = score;
    new_node->next = hash_table[index];
    hash_table[index] = new_node;
}

void delete_hash(hash_node *hash_table[], long id) {
    long index = hash_function(id);
    hash_node *curr = hash_table[index];
    hash_node *prev = NULL;

    while (curr) {
        if (curr->id == id) {
            if (prev) {
                prev->next = curr->next;
            } else {
                hash_table[index] = curr->next;
            }
            free(curr);
            break;
        }
        prev = curr;
        curr = curr->next;
    }
}

long lookup_hash(hash_node *hash_table[], long id) {
    long index = hash_function(id);
    hash_node *curr = hash_table[index];

    while (curr) {
        if (curr->id == id) {
            return curr->score;
        }
        curr = curr->next;
    }
    return 0;
}

rb_node *create_rb_node(long id, long score) {
    rb_node *new_node = (rb_node *)malloc(sizeof(rb_node));
    new_node->id = id;
    new_node->score = score;
    new_node->color = 1;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->parent = NULL;
    new_node->size = 1;
    return new_node;
}

rb_node *rotate_left(rb_node *root, rb_node *x) {
    rb_node *y = x->right;
    x->right = y->left;
    if (y->left) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (!x->parent) {
        root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
    x->size = (x->left ? x->left->size : 0) + (x->right ? x->right->size : 0) + 1;
    y->size = (y->left ? y->left->size : 0) + (y->right ? y->right->size : 0) + 1;
    return root;
}

rb_node *rotate_right(rb_node *root, rb_node *x) {
    rb_node *y = x->left;
    x->left = y->right;
    if (y->right) {
        y->right->parent = x;
    }
    y->parent = x->parent;
    if (!x->parent) {
        root = y;
    } else if (x == x->parent->right) {
        x->parent->right = y;
    } else {
        x->parent->left = y;
    }
    y->right = x;
    x->parent = y;
    x->size = (x->left ? x->left->size : 0) + (x->right ? x->right->size : 0) + 1;
    y->size = (y->left ? y->left->size : 0) + (y->right ? y->right->size : 0) + 1;

    return root;
}

rb_node *rb_insert_fixup(rb_node *root, rb_node *z) {
    while (z->parent && z->parent->color == 1) {
        if (z->parent == z->parent->parent->left) {
            rb_node *y = z->parent->parent->right;
            if (y && y->color == 1) {
                z->parent->color = 0;
                y->color = 0;
                z->parent->parent->color = 1;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    root = rotate_left(root, z);
                }
                z->parent->color = 0;
                z->parent->parent->color = 1;
                root = rotate_right(root, z->parent->parent);
            }
        } else {
            rb_node *y = z->parent->parent->left;
            if (y && y->color == 1) {
                z->parent->color = 0;
                y->color = 0;
                z->parent->parent->color = 1;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    root = rotate_right(root, z);
                }
                z->parent->color = 0;
                z->parent->parent->color = 1;
                root = rotate_left(root, z->parent->parent);
            }
        }
    }
    root->color = 0;
    return root;
}

rb_node *rb_insert(rb_node *root, long id, long score) {
    rb_node *z = create_rb_node(id, score);
    rb_node *y = NULL;
    rb_node *x = root;

    while (x) {
        y = x;
        x->size++;  
        if (score < x->score || (score == x->score && id < x->id)) {
            x = x->left;
        } else {
            x = x->right;
        }
    }

    z->parent = y;
    if (!y) {
        root = z;
    } else if (score < y->score || (score == y->score && id < y->id)) {
        y->left = z;
    } else {
        y->right = z;
    }

    return rb_insert_fixup(root, z);
}

long rb_select(rb_node *root, int k) {
    if (!root) {
        return -1;
    }

    int r = root->right ? root->right->size + 1 : 1;

    if (k == r) {
        return root->id;
    } else if (k < r) {
        return rb_select(root->right, k);
    } else {
        return rb_select(root->left, k - r);
    }
}

long rb_minimum(rb_node *root, int k) {
    rb_node *min_node = NULL;
    int min_score = INT_MAX;
    rb_node *current = root;

    while (current) {
        if (current->score >= k) {
            if (current->score < min_score || (current->score == min_score && current->id < min_node->id)) {
                min_node = current;
                min_score = current->score;
            }
            current = current->left;
        } else {
            current = current->right;
        }
    }

    return min_node ? min_node->id : -1;
}

int rb_count(rb_node *root, long l, long r) {
    if (!root) {
        return 0;
    }

    if (root->score < l) {
        return rb_count(root->right, l, r);
    } else if (root->score > r) {
        return rb_count(root->left, l, r);
    } else {
        return 1 + rb_count(root->left, l, r) + rb_count(root->right, l, r);
    }
}

rb_node *rb_transplant(rb_node *root, rb_node *u, rb_node *v) {
    if (!u->parent) {
        root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    if (v) {
        v->parent = u->parent;
    }
    return root;
}

rb_node *rb_delete_fixup(rb_node *root, rb_node *x) {
    while (x != root && (x->color == 0)) {
        if (x == x->parent->left) {
            rb_node *w = x->parent->right;
            if (w->color == 1) {
                w->color = 0;
                x->parent->color = 1;
                root = rotate_left(root, x->parent);
                w = x->parent->right;
            }
            if ((!w->left || w->left->color == 0) && (!w->right || w->right->color == 0)) {
                w->color = 1;
                x = x->parent;
            } else {
                if (!w->right || w->right->color == 0) {
                    w->left->color = 0;
                    w->color = 1;
                    root = rotate_right(root, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = 0;
                w->right->color = 0;
                root = rotate_left(root, x->parent);
                x = root;
            }
        } else {
            rb_node *w = x->parent->left;
            if (w->color == 1) {
                w->color = 0;
                x->parent->color = 1;
                root = rotate_right(root, x->parent);
                w = x->parent->left;
            }
            if ((!w->right || w->right->color == 0) && (!w->left || w->left->color == 0)) {
                w->color = 1;
                x = x->parent;
            } else {
                if (!w->left || w->left->color == 0) {
                    w->right->color = 0;
                    w->color = 1;
                    root = rotate_left(root, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = 0;
                w->left->color = 0;
                root = rotate_right(root, x->parent);
                x = root;
            }
        }
    }
    if (x) {
        x->color = 0;
    }
    return root;
}


rb_node *tree_minimum(rb_node *node) {
    while (node->left) {
        node = node->left;
    }
    return node;
}

rb_node *rb_delete(rb_node *root, long id, long score) {
    rb_node *z = root;
    while (z && z->id != id) {
        if (score < z->score || (score == z->score && id < z->id)) {
            z = z->left;
        } else {
            z = z->right;
        }
    }

    if (!z) {
        return root;
    }

    rb_node *y = z;
    int y_original_color = y->color;
    rb_node *x;

    if (!z->left) {
        x = z->right;
        root = rb_transplant(root, z, z->right);
    } else if (!z->right) {
        x = z->left;
        root = rb_transplant(root, z, z->left);
    } else {
        y = tree_minimum(z->right);
        y_original_color = y->color;
        x = y->right;

        if (y->parent == z) {
            if (x) {
                x->parent = y;
            }
        } else {
            root = rb_transplant(root, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }

        root = rb_transplant(root, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    if (y_original_color == 0) {
        root = rb_delete_fixup(root, x);
    }

    rb_node *node = x ? x->parent : NULL;
    while (node) {
        node->size--;
        node = node->parent;
    }

    free(z);
    return root;
}

void free_hash_table(hash_node *hash_table[], int size) {
    for (int i = 0; i < size; i++) {
        hash_node *current = hash_table[i];
        while (current) {
            hash_node *temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(hash_table);
}


int main() {
    long n;
    scanf("%ld", &n);
    getchar();

    rb_node *rb_tree = NULL;
    hash_node **hash_table = (hash_node **)calloc(HASHSIZE, sizeof(hash_node *));

    for (int i = 0; i < n; i++) {
        char op;
        long x, y;
        op = getchar(); 

        switch (op) {
            case 'I':
                scanf("%ld %ld", &x, &y);
                getchar(); 
                insert_hash(hash_table, x, y);
                rb_tree = rb_insert(rb_tree, x, y);
                break;
            case 'D':
                scanf("%ld %ld", &x, &y);
                getchar();
                if (y != 0) {
                    delete_hash(hash_table, x);
                    //rb_tree = rb_delete(rb_tree, x, y);
                }
                break;
            case 'L':
                scanf("%ld", &x);
                getchar(); 
                printf("%ld\n", lookup_hash(hash_table, x));
                break;
            case 'S':
                scanf("%ld", &x);
                getchar(); 
                printf("%ld\n", rb_select(rb_tree, x));
                break;
            case 'M':
                scanf("%ld", &x);
                getchar(); 
                printf("%ld\n", rb_minimum(rb_tree, x));
                break;
            case 'C':
                scanf("%ld %ld", &x, &y);
                getchar(); 
                printf("%d\n", rb_count(rb_tree, x, y));
                break;
            default:
                break;
        }
    }
    free_hash_table(hash_table, HASHSIZE);
    return 0;
}
