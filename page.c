#include <page.h>
#include <string.h>
#include <stdlib.h>


page_t* create_page(const char* arr) {
    page_t* page = malloc(sizeof(page_t));
    page->size = strlen(arr);
    page->data = malloc(page->size + 1);
    strcpy(page->data, arr);
    return page;
}


page_t* copy_page(const page_t* page) {
    page_t* new_page = create_page(page->data);
    return new_page;
}


void delete_page(page_t* page) {
    free(page->data);
    free(page);
}


/* djb2 hash function. URL: http://www.cse.yorku.ca/~oz/hash.html */
static unsigned long hash(const unsigned char *str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}



unsigned long key_hash(const tkey_t* key_ptr) {
    return hash((const unsigned char*)*key_ptr);
}


bool key_equal(const tkey_t* lhs, const tkey_t* rhs) {
    if (*lhs == *rhs) {
        return true;
    }
    size_t lhs_len = strlen(*lhs);
    size_t rhs_len = strlen(*rhs);
    if (lhs_len != rhs_len) {
        return false;
    }
    return !strcmp(*lhs, *rhs);
}

