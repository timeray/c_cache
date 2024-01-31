#include <stdlib.h>
#include <string.h>

#include "cache.h"


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
    if (cache != NULL) {
        delete_hashtable(cache->htable);
        while (!is_list_empty(cache->list)) {
            page_t* page = list_back(cache->list);
            delete_page(page);
            list_pop_back(cache->list);
        }
        delete_list(cache->list);
    }
}


const page_t* cached_call(lru_cache_t* cache, const char* key, page_t* (*get_page_slow)(const char*)) {
    list_node_t* node = hashtable_get(cache->htable, key);
    page_t* page;
    if (node == NULL) {
        page = get_page_slow(key);
        if (list_length(cache->list) == cache->max_size) {
            page_t* del_page = list_back(cache->list);
            list_pop_back(cache->list);
            hashtable_delete_entry(cache->htable, del_page->key);
            delete_page(del_page);
        }
        list_node_t* new_node = list_push_front(cache->list, page);
        hashtable_put(cache->htable, key, new_node);
    } else {
        page = list_node_get_page(node);
        list_move_upfront(cache->list, node);
    }
    return page;
    
}


size_t cache_length(const lru_cache_t* cache) {
    return list_length(cache->list);
}
