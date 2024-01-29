#pragma once

#include <stddef.h>
#include <stdbool.h>

struct page_t {
    char* key;
    char* data;
};

typedef struct page_t page_t;

page_t* create_page(const char*, const char*);
page_t* copy_page(const page_t*);
void delete_page(page_t*);
unsigned long key_hash(const char*);
bool key_equal(const char*, const char*);
char* string_dup(const char*);
