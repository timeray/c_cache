#include <stdlib.h>
#include <stdio.h>
#include "list.h"

struct list_node_t {
    list_node_t* next;
    list_node_t* prev;
    page_t* page;
};


struct list_t {
    list_node_t* head;
    list_node_t* tail;
    size_t size;
};


page_t* list_node_get_page(list_node_t* node) {
    return node->page;
}


bool is_list_empty(const list_t* list) {
    return ((list->head == NULL) && (list->tail == NULL));
}

list_t* create_list(void) {
    list_t* list_ptr = malloc(sizeof(list_t));
    if (list_ptr == NULL) {
        puts("malloc failed");
        exit(EXIT_FAILURE);
    }
    list_ptr->head = NULL;
    list_ptr->tail = NULL;
    list_ptr->size = 0;
    return list_ptr;
}


void delete_list(list_t* list) {
    if (list != NULL) {
        while (!is_list_empty(list)) {
            list_pop_back(list);
        }
        free(list);
    }
}


list_node_t* create_list_node(void) {
    list_node_t* res = calloc(1, sizeof(list_node_t));
    if (res == NULL) {
        puts("calloc failed");
        exit(EXIT_FAILURE);
    }
    return res;
}


void delete_list_node(list_node_t* node) {
    free(node);
}


list_node_t* list_push_front(list_t* list, page_t* page) {
    list_node_t* new_node = create_list_node();

    new_node->page = page;
    new_node->next = list->head;
    if (list->head != NULL) {
        list->head->prev = new_node;
    } else {
        list->tail = new_node;
    }
    list->head = new_node;
    ++list->size;
    return new_node;
}


list_node_t* list_push_back(list_t* list, page_t* page) {
    list_node_t* new_node = create_list_node();
    new_node->page = page;
    new_node->prev = list->tail;
    if (list->tail != NULL) {
        list->tail->next = new_node;
    } else {
        list->head = new_node;
    }
    list->tail = new_node;
    ++list->size;
    return new_node;
}


void list_pop_front(list_t* list) {
    list_node_t* node = list->head;
    list->head = node->next;
    if (list->head != NULL) {
        // not last element
        list->head->prev = NULL;
    } else {
        // last element
        list->tail = NULL;
    }
    --list->size;
    delete_list_node(node);
}


void list_pop_back(list_t* list) {
    list_node_t* node = list->tail;
    list->tail = node->prev;
    if (list->tail != NULL) {
        // not last element
        list->tail->next = NULL;
    } else {
        // last element
        list->head = NULL;
    }
    --list->size;
    delete_list_node(node);
}


page_t* list_front(const list_t* list) {
    if (!is_list_empty(list)) {
        return list->head->page;
    } else {
        return NULL;
    }
}


page_t* list_back(const list_t* list) {
    if (!is_list_empty(list)) {
        return list->tail->page;
    } else {
        return NULL;
    }
}


void list_move_upfront(list_t* list, list_node_t* node) {
    if (node == list->head) {
        return;
    }
    node->prev->next = node->next;
    if (node != list->tail) {
        node->next->prev = node->prev;
    } else {
        list->tail = node->prev;
    }
    list->head->prev = node;
    node->next = list->head;
    node->prev = NULL;
    list->head = node;
}


size_t list_length(const list_t* list) {
    return list->size;
}


void list_print(const list_t* list) {
    printf("List %p\n", list);
    printf("Head: %p, tail: %p\n", list->head, list->tail);
    if (is_list_empty(list)) {
        puts("Empty list");
    }
    list_node_t* node = list->head;
    size_t count = 1;
    while (node != NULL) {
        printf("Node %zu: addr = %p, next = %p, prev = %p, page = %p\n",
               count++, node, node->next, node->prev, node->page);
        node = node->next;
    }
}

