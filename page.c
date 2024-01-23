#include <page.h>
#include <string.h>
#include <stdlib.h>


page_t* create_page(const char* arr) {
    page_t* page = malloc(sizeof(page_t));
    page->size = strlen(arr);
    page->data = malloc(page->size + 1);
    strcpy_s(page->data, page->size + 1, arr);
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


int key_hash(const key_t* key_ptr) {
    return 1;
}


bool key_equal(const key_t* lhs, const key_t* rhs) {
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

