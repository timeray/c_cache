#pragma once

#include <clist.h>
#include <page.h>

typedef struct hashmap_t hashmap_t;


struct hashtable_entry_t {
    key_t key;
    list_node_t* node;
};


struct hashtable_t {
    hashtable_entry_t **table;
    size_t n_buckets;
    size_t n_entries;
};


hashtable_t* create_hashtable(void);
void delete_hashtable(hashtable_t*);
list_node_t* hashtable_get(hashtable_t*, key_t);
void hashtable_put(hashtable_t*, key_t, list_node_t*);
void hashtable_delete_entry(hashtable_t*, key_t);
