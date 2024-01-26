#pragma once

#include <clist.h>
#include <page.h>

typedef struct hashtable_t hashtable_t;
typedef struct hashtable_entry_t hashtable_entry_t;


hashtable_t* create_hashtable(void);
void delete_hashtable(hashtable_t*);
bool hashtable_is_empty(const hashtable_t*);
size_t hashtable_length(const hashtable_t*);
list_node_t* hashtable_get(const hashtable_t*, const tkey_t*);
void hashtable_put(hashtable_t*, const tkey_t*, list_node_t*);
bool hashtable_delete_entry(hashtable_t*, const tkey_t*);

void hashtable_print(const hashtable_t*);
