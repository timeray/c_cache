#include <stdio.h>
#include "clist.h"
#include "page.h"


int main(void) {
    puts("Test C double linked list");    
    list_t* lst = create_list();
    printf("List size = %zu\n", list_length(lst));
    list_print(lst);

    page_t* page = create_page("Hello");
    printf("Page1 address = %p\n", page);

    puts("Push front");
    list_push_front(lst, page);
    list_print(lst);

    puts("Push front");
    list_push_front(lst, page);
    list_print(lst);

    puts("Push back");
    list_push_back(lst, page);
    list_print(lst);

    page_t* page2 = create_page("TheWorld");
    printf("Page2 address = %p\n", page2);

    puts("Push front");
    list_push_front(lst, page2);
    list_print(lst);

    printf("List size = %zu\n", list_length(lst));
    printf("Front page address = %p\n", list_front(lst));
    printf("Back page address = %p\n", list_back(lst));

    puts("Pop front");
    list_pop_front(lst);
    list_print(lst);

    puts("Pop back");
    list_pop_back(lst);
    list_print(lst);

    delete_list(lst);
}
