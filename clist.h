#pragma once

#include <stdbool.h>
#include "page.h"

// Doubly linked list
typedef struct list_t list_t;
typedef struct list_node_t list_node_t;

page_t* list_node_get_page(list_node_t*);

list_t* create_list(void);
void delete_list(list_t*);

list_node_t* create_list_node(void);
void delete_list_node(list_node_t*);

list_node_t* list_push_front(list_t*, page_t*);
list_node_t* list_push_back(list_t*, page_t*);
void list_pop_front(list_t*);
void list_pop_back(list_t*);

void list_move_upfront(list_t*, list_node_t*);

page_t* list_front(const list_t*);
page_t* list_back(const list_t*);

size_t list_length(const list_t*);
bool is_list_empty(const list_t*);
void list_print(const list_t*);
