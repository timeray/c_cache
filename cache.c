#include <stdlib.h>
#include <string.h>

#include "cache.h"
#include "chashtable.h"
#include "clist.h"


struct lru_cache_t {
    hashtable_t* htable;
    list_t* list;
    const size_t max_size;
};


lru_cache_t* create_cache(size_t size) {
    if (size == 0) {
        return NULL;
    }
    lru_cache_t cache = {.htable=create_hashtable(), .list=create_list(), .max_size=size};
    lru_cache_t* cache_ptr = malloc(sizeof(lru_cache_t));
    memcpy(cache_ptr, &cache, sizeof(lru_cache_t));
    return cache_ptr;
}


void delete_cache(lru_cache_t* cache) {
    delete_hashtable(cache->htable);
    while (!is_list_empty(cache->list)) {
        page_t* page = list_back(cache->list);
        delete_page(page);
        list_pop_back(cache->list);
    }
    delete_list(cache->list);
}


page_t* cached_call(lru_cache_t* cache, const tkey_t* key_ptr, page_t* (*get_page_slow)(const tkey_t*)) {
    list_node_t* node = hashtable_get(cache->htable, key_ptr);
    page_t* page;
    if (node == NULL) {
        page = get_page_slow(key_ptr);
        if (list_length(cache->list) == cache->size) {
            delete_page(list_back(cache->list));
            list_pop_back(cache->list);
            hashtable_delete_entry(back_key??);
        }
        list_node_t* node = list_push_front(cache->list, page);
        hashtable_put(cache->htable, key_ptr, node);
    } else {
        page = node->page;
        list_move_upfront(cache->list, node);
    }
    return page;
    
}
