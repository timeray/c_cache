#include <page.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


page_t* create_page(const char* key, const char* data) {
    page_t* page = malloc(sizeof(page_t));
    if (page == NULL) {
        puts("malloc failed");
        exit(EXIT_FAILURE);
    }
    page->key = string_dup(key);
    page->data = string_dup(data);
    return page;
}


page_t* copy_page(const page_t* page) {
    page_t* new_page = create_page(page->key, page->data);
    return new_page;
}


void delete_page(page_t* page) {
    if (page != NULL) {
        free(page->key);
        free(page->data);
        free(page);
    }
}


/* djb2 hash function. URL: http://www.cse.yorku.ca/~oz/hash.html */
static unsigned long hash(const unsigned char *str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}



unsigned long key_hash(const char* str) {
    return hash((const unsigned char*) str);
}


bool key_equal(const char* lhs, const char* rhs) {
    if (lhs == rhs) {
        return true;
    }
    size_t lhs_len = strlen(lhs);
    size_t rhs_len = strlen(rhs);
    if (lhs_len != rhs_len) {
        return false;
    }
    return !strcmp(lhs, rhs);
}


char* string_dup(const char* str) {
    size_t len = strlen(str);
    char* new_key = malloc(len + 1);
    if (new_key == NULL) {
        puts("malloc failed");
        exit(EXIT_FAILURE);
    }
    strcpy(new_key, str);
    return new_key;
}
