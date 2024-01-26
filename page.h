#pragma once

#include <stddef.h>
#include <stdbool.h>

struct page_t {
    size_t size;
    char* data;
};

typedef struct page_t page_t;
typedef char* tkey_t;

page_t* create_page(const char*);
page_t* copy_page(const page_t*);
void delete_page(page_t*);
unsigned long key_hash(const tkey_t*);
bool key_equal(const tkey_t*, const tkey_t*);
