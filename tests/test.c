#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include "../page.h"
#include "../clist.h"
#include "../chashtable.h"


START_TEST(test_page)
{
    {
        page_t* page = create_page("");
        ck_assert_uint_eq(page->size, 0);
        ck_assert_str_eq(page->data, "");
        delete_page(page);
    }
    {
        page_t* page = create_page("Hello");
        ck_assert_uint_eq(page->size, 5);
        ck_assert_str_eq(page->data, "Hello");
        delete_page(page);
    }
    {
        page_t* page = create_page("Ref_page");
        page_t* page_copy = copy_page(page);
        delete_page(page);
        ck_assert_str_eq(page_copy->data, "Ref_page");
        ck_assert_uint_eq(page_copy->size, 8);
        delete_page(page_copy);
    }
}
END_TEST


START_TEST(test_key)
{
    key_t key1 = {"Hello, key"};
    key_t key2 = key1;
    key_t key3 = {"Hello, keyw"};
    ck_assert_int_eq(key_hash(&key1), 1);
    ck_assert_str_eq(key1, "Hello, key");
    ck_assert(key_equal(&key1, &key2));
    ck_assert(!key_equal(&key1, &key3));
}
END_TEST


START_TEST(test_list_create)
{
    list_t* list = create_list();

    ck_assert(is_list_empty(list));
    ck_assert_msg(list_length(list) == 0, "failure");
    delete_list(list);
}
END_TEST


START_TEST(test_list_push_pop)
{
    list_t* list = create_list();

    page_t* page1 = create_page("page1");
    page_t* page2 = create_page("page2");
    page_t* page3 = create_page("page3");
    page_t* page4 = create_page("page4");
    page_t* page5 = create_page("page5");

    list_push_front(list, page1);
    ck_assert_uint_eq(list_length(list), 1);
    ck_assert(!is_list_empty(list));
    page_t* head = list_front(list);
    page_t* tail = list_back(list);
    ck_assert_ptr_nonnull(head);
    ck_assert_ptr_nonnull(tail);
    ck_assert(head == tail);
    ck_assert(head == page1);

    list_push_back(list, page2);
    ck_assert_uint_eq(list_length(list), 2);
    head = list_front(list);
    tail = list_back(list);
    ck_assert(head == page1);
    ck_assert(tail == page2);

    list_push_back(list, page3);
    list_push_front(list, page4);
    list_push_front(list, page5);
    ck_assert_uint_eq(list_length(list), 5);
    head = list_front(list);
    tail = list_back(list);
    ck_assert(head == page5);
    ck_assert(tail == page3);

    list_pop_back(list);
    list_pop_back(list);
    ck_assert_uint_eq(list_length(list), 3);
    head = list_front(list);
    tail = list_back(list);
    ck_assert(head == page5);
    ck_assert(tail == page1);

    list_pop_front(list);
    list_pop_front(list);
    ck_assert_uint_eq(list_length(list), 1);
    head = list_front(list);
    tail = list_back(list);
    ck_assert(head == page1);
    ck_assert(tail == page1);

    list_pop_back(list);
    ck_assert_uint_eq(list_length(list), 0);
    ck_assert(is_list_empty(list));
    head = list_front(list);
    tail = list_back(list);
    ck_assert_ptr_null(head);
    ck_assert_ptr_null(tail);

    delete_page(page1);
    delete_page(page2);
    delete_page(page3);
    delete_page(page4);
    delete_page(page5);
    delete_list(list);
}
END_TEST


START_TEST(test_list_push_pop_randomized)
{
    list_t* list = create_list();
    const size_t n_pages = RAND_MAX + 1;
    page_t** pages = malloc(sizeof(page_t*) * n_pages);
    for (size_t i = 0; i < n_pages; ++i) {
        char buf[1000];
        sprintf_s(buf, 1000, "page%zu", i);
        pages[i] = create_page(buf);
    }

    size_t count_pushes = 0;
    size_t count_pops = 0;

    for (size_t i = 0; i < 1000000; ++i) {
        int rand_num = rand() % 100;
        if (rand_num < 25) {
            list_push_front(list, pages[rand()]);
            ++count_pushes;
        } else if (rand_num < 60) {
            list_push_back(list, pages[rand()]);
            ++count_pushes;
        } else if (rand_num < 70) {
            page_t* head = list_front(list);
            if (!is_list_empty(list)) {
                ck_assert_ptr_nonnull(head);
                ck_assert(head->data[0] == 'p');
            }
        } else if (rand_num < 80) {
            page_t* tail = list_back(list);
            if (!is_list_empty(list)) {
                ck_assert_ptr_nonnull(tail);
                ck_assert(tail->data[0] == 'p');
            }
        } else if (rand_num < 90) {
            if (!is_list_empty(list)) {
                list_pop_front(list);
                ++count_pops;
            }
        } else {
            if (!is_list_empty(list)) {
                list_pop_back(list);
                ++count_pops;
            }
        }
    }
    ck_assert(list_length(list) == (count_pushes > count_pops ? count_pushes - count_pops : 0));
    while (!is_list_empty(list)) {
        list_pop_back(list);
    }

    for (size_t i = 0; i < n_pages; ++i) {
        delete_page(pages[i]);
    }
    free(pages);
    delete_list(list);
}
END_TEST


START_TEST(test_hashtable_create)
{
    {
        hashtable_t* htable = create_hashtable();
        delete_hashtable(htable);
    }
    {
        hashtable_t* htable = create_hashtable();
        ck_assert(hashtable_is_empty(htable));
        ck_assert_uint_eq(hashtable_length(htable), 0);
        delete_hashtable(htable);
    }
}
END_TEST


START_TEST(test_hashtable_put_get_delete)
{
    hashtable_t* htable = create_hashtable();
    
    key_t key1 = "key1";
    list_node_t* node1 = create_list_node();
    
    ck_assert(!hashtable_get(htable, &key1));
    
    hashtable_put(htable, &key1, node1);
    ck_assert(!hashtable_is_empty(htable));
    ck_assert_uint_eq(hashtable_length(htable), 1);
    ck_assert(hashtable_get(htable, &key1) == node1);

    hashtable_put(htable, &key1, node1);
    // nothing should change
    ck_assert_uint_eq(hashtable_length(htable), 1);
    ck_assert(hashtable_get(htable, &key1) == node1);

    hashtable_put(htable, &key1, NULL);
    // NULL should overwrite node1
    ck_assert_uint_eq(hashtable_length(htable), 1);
    ck_assert(hashtable_get(htable, &key1) == NULL);
    hashtable_put(htable, &key1, node1);

    key_t key2 = "key2";
    list_node_t* node2 = create_list_node();
    hashtable_put(htable, &key2, node2);

    hashtable_print(htable);    

    ck_assert_uint_eq(hashtable_length(htable), 2);
    ck_assert(hashtable_get(htable, &key1) == node1);
    ck_assert(hashtable_get(htable, &key2) == node2);

    key_t key3 = "key3";
    list_node_t* node3 = create_list_node();
    hashtable_put(htable, &key3, node3);

    hashtable_print(htable);

    ck_assert_uint_eq(hashtable_length(htable), 3);
    ck_assert(hashtable_get(htable, &key1) == node1);
    ck_assert(hashtable_get(htable, &key2) == node2);
    ck_assert(hashtable_get(htable, &key3) == node3);

    bool status = hashtable_delete_entry(htable, &key2);
    ck_assert(status);

    hashtable_print(htable);

    ck_assert_uint_eq(hashtable_length(htable), 2);
    ck_assert(hashtable_get(htable, &key1) == node1);
    ck_assert(hashtable_get(htable, &key3) == node3);

    status = hashtable_delete_entry(htable, &key2);
    // nothing should change
    ck_assert(!status);
    ck_assert_uint_eq(hashtable_length(htable), 2);
    ck_assert(hashtable_get(htable, &key1) == node1);
    ck_assert(hashtable_get(htable, &key3) == node3);

    status = hashtable_delete_entry(htable, &key1);
    ck_assert(status);
    ck_assert_uint_eq(hashtable_length(htable), 1);
    ck_assert(hashtable_get(htable, &key3) == node3);

    hashtable_print(htable);
    
    status = hashtable_delete_entry(htable, &key3);
    ck_assert(status);
    ck_assert_uint_eq(hashtable_length(htable), 0);
    ck_assert(hashtable_is_empty(htable));
    
    hashtable_print(htable);

    delete_hashtable(htable);
    delete_list_node(node1);
    delete_list_node(node2);
    delete_list_node(node3);
}
END_TEST


Suite* list_suite(void) {
    Suite *s = suite_create("lru_cache");

    // Page tests
    TCase *tc_page = tcase_create("Page");
    tcase_add_test(tc_page, test_page);

    // Key tests
    TCase *tc_key = tcase_create("Key");
    tcase_add_test(tc_key, test_key);

    // Clist tests
    TCase *tc_clist = tcase_create("Clist");
    tcase_add_test(tc_clist, test_list_create);
    tcase_add_test(tc_clist, test_list_push_pop);
    tcase_add_test(tc_clist, test_list_push_pop_randomized);

    // Chashtable tests
    TCase *tc_chashtable = tcase_create("Chashtable");
    tcase_add_test(tc_chashtable, test_hashtable_create);
    tcase_add_test(tc_chashtable, test_hashtable_put_get_delete);

    suite_add_tcase(s, tc_page);
    suite_add_tcase(s, tc_key);
    suite_add_tcase(s, tc_clist);
    suite_add_tcase(s, tc_chashtable);
    return s;
}


int main(void) {
    puts("Run tests");
    Suite *suite = list_suite();
    SRunner *runner = srunner_create(suite);
    srunner_run_all(runner, CK_NORMAL);
    int n_failed = srunner_ntests_failed(runner);
    srunner_free(runner);
    return (n_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
