#pragma once

#include "page.h"

// Doubly linked list
typedef struct list_t list_t;
typedef struct list_node_t list_node_t;

list_t* create_list(void);
void delete_list(list_t*);

void list_push_front(list_t*, page_t*);
void list_push_back(list_t* list, page_t*);
void list_pop_front(list_t*);
void list_pop_back(list_t*);


page_t* list_front(const list_t*);
page_t* list_back(const list_t*);

size_t list_length(const list_t*);
void list_print(const list_t*);
