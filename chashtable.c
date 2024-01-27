#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "chashtable.h"


#define MAX_LOAD_FACTOR 1


struct hashtable_entry_t {
    tkey_t key;
    list_node_t* node;
    hashtable_entry_t* next;
};


struct hashtable_t {
    hashtable_entry_t **table;
    size_t n_buckets;
    size_t n_entries;
};


static void rehash(hashtable_t*);
static hashtable_entry_t** get_bucket(const hashtable_t*, const tkey_t*);
static hashtable_entry_t* find_entry(const hashtable_t*, const tkey_t*);


static hashtable_entry_t* create_hashtable_entry(const tkey_t* key_ptr, list_node_t* node, hashtable_entry_t* next) {
    hashtable_entry_t* entry = malloc(sizeof(hashtable_entry_t));
    entry->key = *key_ptr;
    entry->node = node;
    entry->next = next;
    return entry;
}


static void delete_hashtable_entry(hashtable_entry_t* entry) {
    free(entry);
}


hashtable_t* create_hashtable(void) {
    hashtable_t* htable = malloc(sizeof(hashtable_t));
    htable->table = NULL;
    htable->n_buckets = 0;
    htable->n_entries = 0;
    return htable;
}


void delete_hashtable(hashtable_t* htable) {
    if (htable != NULL) {
        for (size_t i = 0; i < htable->n_buckets; ++i) {
            hashtable_entry_t* next = htable->table[i];
            while(next != NULL) {
                hashtable_entry_t* entry = next;
                next = entry->next;
                free(entry);
            }
        }
        free(htable->table);
        free(htable);
    }
}


bool hashtable_is_empty(const hashtable_t* htable) {
    return htable->n_entries == 0;
}


size_t hashtable_length(const hashtable_t* htable) {
    return htable->n_entries;
}


list_node_t* hashtable_get(const hashtable_t* htable, const tkey_t* key_ptr) {
    hashtable_entry_t* entry = find_entry(htable, key_ptr);
    if (entry != NULL) {
        return entry->node;
    } else {
        return NULL;
    }
}


void hashtable_put(hashtable_t* htable, const tkey_t* key_ptr, list_node_t* node) {
    if (htable->n_entries == 0) {
        htable->table = malloc(sizeof(hashtable_entry_t*));
        ++htable->n_buckets;

        hashtable_entry_t* entry = create_hashtable_entry(key_ptr, node, NULL);
        htable->table[0] = entry;
    } else {
        hashtable_entry_t* entry = find_entry(htable, key_ptr);

        // Overwrite existing node
        if (entry != NULL) {
            entry->node = node;
            return;
        }

        // Create new entry in the head of list
        hashtable_entry_t** bucket_ptr = get_bucket(htable, key_ptr);
        hashtable_entry_t* new_entry = create_hashtable_entry(key_ptr, node, NULL);
        if (*bucket_ptr != NULL) {
            new_entry->next = *bucket_ptr;
        }
        *bucket_ptr = new_entry;
        
    }
    ++htable->n_entries;
    rehash(htable);
}


bool hashtable_delete_entry(hashtable_t* htable, const tkey_t* key_ptr) {
    if (htable->n_entries == 0) {
        return false;
    }

    hashtable_entry_t* entry = find_entry(htable, key_ptr);
    if (entry == NULL) {
        return false;
    }

    hashtable_entry_t** bucket_ptr = get_bucket(htable, key_ptr);
    if ((*bucket_ptr)->next == NULL) {
        // Then bucket has only one entry
        *bucket_ptr = NULL;
    } else if (*bucket_ptr == entry) {
        // Entry is first in the list
        *bucket_ptr = entry->next;
    } else {
        // Search list for entry (we need to store previous entry)
        hashtable_entry_t* test_entry = *bucket_ptr;
        while (test_entry->next != entry) {  // should exist
            test_entry = test_entry->next;
        }
        test_entry->next = entry->next;
    }
    
    delete_hashtable_entry(entry);
    --htable->n_entries;

    if (htable->n_entries != 0) {
        rehash(htable);        
    } else {
        free(htable->table);
        htable->table = NULL;
        htable->n_buckets = 0;
    }
    return true;
}


void hashtable_print(const hashtable_t* htable) {
    printf("Hash table %p\n", htable);
    if (htable->n_entries == 0) {
        puts("Hash table is empty");
        return;
    }
    printf("n entries = %zu, n_buckets = %zu\n", htable->n_entries, htable->n_buckets);
    for (size_t buck = 0; buck < htable->n_buckets; ++buck) {
        printf("->Bucket %zu\n", buck);
        hashtable_entry_t* entry = htable->table[buck];
        size_t count = 0;
        while (entry != NULL) {
            printf("\tEntry %zu: addr=%p, key=%s, node=%p, next=%p\n", count, entry, entry->key, entry->node, entry->next);
            entry = entry->next;
            ++count;
        }
    }
}


static void rehash(hashtable_t* htable) {
    // When this funcion is called, n_buckets > 0 and n_entries > 0
    if (htable->n_entries == 1) {
        return;
    }
    float load_factor = (float) htable->n_entries / htable->n_buckets;
    if ((load_factor > MAX_LOAD_FACTOR) || (load_factor < (float) MAX_LOAD_FACTOR / 4)) {
        // Perform rehash
        size_t new_n_buckets = 1;
        if (load_factor > MAX_LOAD_FACTOR) {
            new_n_buckets = htable->n_buckets * 2;
        } else if (htable->n_buckets > 1) {
            new_n_buckets = htable->n_buckets / 2;
        }
        
        printf("REHASH, n_entries = %zu, old_n_buckets = %zu, new_n_buckets = %zu\n", htable->n_entries, htable->n_buckets, new_n_buckets);

        hashtable_entry_t** old_table = htable->table;
        size_t old_n_buckets = htable->n_buckets;
        htable->table = malloc(sizeof(hashtable_entry_t*) * new_n_buckets);
        for (size_t i = 0; i < new_n_buckets; ++i) {
            htable->table[i] = NULL;
        }
        htable->n_buckets = new_n_buckets;

        for (size_t buck_num = 0; buck_num < old_n_buckets; ++buck_num) {
            hashtable_entry_t* node = old_table[buck_num];
            while (node != NULL) {
                hashtable_entry_t* next_node = node->next;
                hashtable_entry_t** new_bucket_ptr = get_bucket(htable, &node->key);
                if (*new_bucket_ptr == NULL) {
                    // First node of the list
                    node->next = NULL;
                } else {
                    // Add to list head
                    node->next = *new_bucket_ptr;
                }
                *new_bucket_ptr = node;
                node = next_node;
            }
        }
        free(old_table);
    }
}


static hashtable_entry_t** get_bucket(const hashtable_t* htable, const tkey_t* key_ptr) {
    return &htable->table[key_hash(key_ptr) % htable->n_buckets];
}


static hashtable_entry_t* find_entry(const hashtable_t* htable, const tkey_t* key_ptr) {
    if (htable->n_entries == 0) {
        return NULL;
    }

    hashtable_entry_t* entry = *get_bucket(htable, key_ptr);
    while (entry != NULL) {        
        if (key_equal(&entry->key, key_ptr) == true) {
            break;
        }
        entry = entry->next;
    }
    return entry;
}

