#pragma once

#include "clist.h"
#include "chashtable.h"


typedef struct lru_cache_t lru_cache_t;

lru_cache_t* create_cache(size_t size);
void delete_cache(lru_cache_t*);
const page_t* cached_call(lru_cache_t*, const char*, page_t* (*)(const char*));
size_t cache_length(const lru_cache_t*);

