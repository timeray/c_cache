#pragma once

#include <stddef.h>

struct page_t {
    size_t size;
    char* data;
};

typedef struct page_t page_t;

page_t* create_page(const char*);
void delete_page(page_t*);

