#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

typedef struct {
    int table_size, prime, a, b, c;
} HashFunction;

int hash(HashFunction *hash_function, int key) {
    return ((hash_function->a * key * key + hash_function->b * key + hash_function->c) % hash_function->prime) % hash_function->table_size;
}

void init_hash_function(HashFunction *hash_function, int table_size) {
    hash_function->table_size = table_size;
    hash_function->prime = generate_prime(table_size);
    hash_function->a = rand() % (hash_function->prime - 1) - 1;
    hash_function->b = rand() % (hash_function->prime) ;
    hash_function->c = rand() % (hash_function->prime + 1) + 1;
}


bool is_prime(int num) {
    if (num < 2) return false;
    for (int i = 2; i * i <= num; ++i) {
        if (num % i == 0) return false;
    }
    return true;
}

int generate_prime(int table_size) {
    int prime_candidate = table_size + (table_size % 2 == 0 ? 1 : 0);
    while (!is_prime(prime_candidate)) {
        prime_candidate += 2;
    }
    return prime_candidate;
}

typedef struct {
    int *table1, *table2;
    HashFunction hash_function1, hash_function2;
} CuckooHash;

int lookup(CuckooHash *cuckoo_hash, int key) {
    int hash1 = hash(&cuckoo_hash->hash_function1, key);
    int hash2 = hash(&cuckoo_hash->hash_function2, key);
    if (cuckoo_hash->table1[hash1] == key || cuckoo_hash->table2[hash2] == key) {
        return 1;
    }
    return 0;
}

bool insert(CuckooHash *cuckoo_hash, int key) {
    int max_attempts = cuckoo_hash->hash_function1.table_size * 2;
    int hash1, hash2, temp;
    for (int i = 0; i < max_attempts; ++i) {
        hash1 = hash(&cuckoo_hash->hash_function1, key);
        if (cuckoo_hash->table1[hash1] == -1) {
            cuckoo_hash->table1[hash1] = key;
            return true;
        }
        hash2 = hash(&cuckoo_hash->hash_function2, key);
        if (cuckoo_hash->table2[hash2] == -1) {
            cuckoo_hash->table2[hash2] = key;
            return true;
        }
        if (i % 2 == 0) {
            temp = cuckoo_hash->table1[hash1];
            cuckoo_hash->table1[hash1] = key;
            key = temp;
        } else {
            temp = cuckoo_hash->table2[hash2];
            cuckoo_hash->table2[hash2] = key;
            key = temp;
        }
        // Rehashing
        if (i == max_attempts - 1) {
            int new_table_size = cuckoo_hash->hash_function1.table_size * 2;
            CuckooHash new_cuckoo_hash;
            init_cuckoo_hash(&new_cuckoo_hash, new_table_size);
            for (int j = 0; j < cuckoo_hash->hash_function1.table_size; ++j) {
                if (cuckoo_hash->table1[j] != -1) {
                    insert(&new_cuckoo_hash, cuckoo_hash->table1[j]);
                }
                if (cuckoo_hash->table2[j] != -1) {
                    insert(&new_cuckoo_hash, cuckoo_hash->table2[j]);
                }
            }
            free_cuckoo_hash(cuckoo_hash);
            *cuckoo_hash = new_cuckoo_hash;
            return insert(cuckoo_hash, key);
        }
    }
    return false;
}

void delete_key(CuckooHash *cuckoo_hash, int key) {
    int hash1 = hash(&cuckoo_hash->hash_function1, key);
    int hash2 = hash(&cuckoo_hash->hash_function2, key);
    if (cuckoo_hash->table1[hash1] == key) {
        cuckoo_hash->table1[hash1] = -1;
    }
    if (cuckoo_hash->table2[hash2] == key) {
        cuckoo_hash->table2[hash2] = -1;
    }
}

void init_cuckoo_hash(CuckooHash *cuckoo_hash, int table_size) {
    cuckoo_hash->table1 = (int *)malloc(table_size * sizeof(int));
    cuckoo_hash->table2 = (int *)malloc(table_size * sizeof(int));
    for (int i = 0; i < table_size; ++i) {
        cuckoo_hash->table1[i] = -1;
        cuckoo_hash->table2[i] = -1;
    }
    init_hash_function(&cuckoo_hash->hash_function1, table_size);
    init_hash_function(&cuckoo_hash->hash_function2, table_size);
}

void free_cuckoo_hash(CuckooHash *cuckoo_hash) {
    free(cuckoo_hash->table1);
    free(cuckoo_hash->table2);
}


int main() {
    int m, n;
    scanf("%d %d", &m, &n);

    srand(time(NULL));
    CuckooHash cuckoo_hash;
    init_cuckoo_hash(&cuckoo_hash, m);

    for (int i = 0; i < n; ++i) {
        char operation;
        int key;
        scanf(" %c%d", &operation, &key);

        if (operation == 'L') {
            printf("%d\n", lookup(&cuckoo_hash, key));
        } else if (operation == 'I') {
            insert(&cuckoo_hash, key);
        } else if (operation == 'D') {
            delete_key(&cuckoo_hash, key);
        }
    }

    free_cuckoo_hash(&cuckoo_hash);
    return 0;
}