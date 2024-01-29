#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include "../page.h"
#include "../clist.h"
#include "../chashtable.h"


enum { 
    HASH_TEST_ARR_SIZE=1000,
    HASH_TEST_N_VALS=100000,
    RNG_TEST_SIZE=100000,
    RNG_TEST_N_ITER=1000000,
};


START_TEST(test_page)
{
    {
        page_t* page = create_page("", "");
        ck_assert_str_eq(page->key, "");
        ck_assert_str_eq(page->data, "");
        delete_page(page);
    }
    {
        page_t* page = create_page("Hello", "World");
        ck_assert_str_eq(page->key, "Hello");
        ck_assert_str_eq(page->data, "World");
        delete_page(page);
    }
    {
        page_t* page = create_page("key", "Ref_page");
        page_t* page_copy = copy_page(page);
        delete_page(page);
        ck_assert_str_eq(page_copy->key, "key");
        ck_assert_str_eq(page_copy->data, "Ref_page");
        delete_page(page_copy);
    }
}
END_TEST


START_TEST(test_key)
{
    char* key1 = {"Hello, key"};
    char* key2 = key1;
    char* key3 = {"Hello, keyw"};
    ck_assert_str_eq(key1, "Hello, key");
    ck_assert(key_equal(key1, key2));
    ck_assert(!key_equal(key1, key3));
}
END_TEST


static float calc_mean(int* arr, size_t size) {
    float res = 0.0;
    for (size_t i = 0; i < size; ++i) {
        res += (float)arr[i];
    }
    return res / (float)size;
}


START_TEST(test_hash)
{
    int freqs[HASH_TEST_ARR_SIZE] = {0};
    char buf[100];
    char* buf_ptr = (char*) buf;
    for (size_t i = 0; i < HASH_TEST_N_VALS; ++i) {
        sprintf(buf, "key%zu", i);
        ++freqs[key_hash(buf_ptr) % HASH_TEST_ARR_SIZE];
    }
    float mean = calc_mean(freqs, HASH_TEST_ARR_SIZE);
    float expected_mean = (float)HASH_TEST_N_VALS / HASH_TEST_ARR_SIZE;
    ck_assert_float_eq_tol(mean, expected_mean, 0.1);
    printf("String hash stat test: mean = %f (expected %f)\n", mean, expected_mean);
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

    page_t* page1 = create_page("key1", "page1");
    page_t* page2 = create_page("key2", "page2");
    page_t* page3 = create_page("key3", "page3");
    page_t* page4 = create_page("key4", "page4");
    page_t* page5 = create_page("key5", "page5");

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


START_TEST(test_list_move_upfront)
{
    list_t* list = create_list();

    page_t* page1 = create_page("key1", "page1");
    page_t* page2 = create_page("key2", "page2");
    page_t* page3 = create_page("key3", "page3");

    list_node_t* node1 = list_push_front(list, page1);
    list_move_upfront(list, node1);
    ck_assert_ptr_eq(list_front(list), page1);

    list_node_t* node2 = list_push_back(list, page2);
    list_move_upfront(list, node2);
    ck_assert_ptr_eq(list_front(list), page2);
    ck_assert_ptr_eq(list_back(list), page1);

    list_node_t* node3 = list_push_front(list, page3);
    list_move_upfront(list, node3);
    ck_assert_ptr_eq(list_front(list), page3);

    list_move_upfront(list, node1);
    ck_assert_ptr_eq(list_front(list), page1);

    list_move_upfront(list, node2);
    ck_assert_ptr_eq(list_front(list), page2);
    ck_assert_ptr_eq(list_back(list), page3);
    ck_assert_uint_eq(list_length(list), 3);
    
    list_move_upfront(list, node3);
    ck_assert_ptr_eq(list_front(list), page3);
    ck_assert_ptr_eq(list_back(list), page1);
    ck_assert_uint_eq(list_length(list), 3);

    delete_page(page1);
    delete_page(page2);
    delete_page(page3);

    delete_list(list);
}
END_TEST


START_TEST(test_list_push_pop_randomized)
{
    list_t* list = create_list();
    const size_t n_pages = RNG_TEST_SIZE;
    page_t** pages = malloc(sizeof(page_t*) * n_pages);
    for (size_t i = 0; i < n_pages; ++i) {
        char buf[1000];
        sprintf(buf, "page%zu", i);
        pages[i] = create_page("", buf);
    }

    size_t count_pushes = 0;
    size_t count_pops = 0;

    for (size_t i = 0; i < RNG_TEST_N_ITER; ++i) {
        int rand_num = rand() % 100;
        if (rand_num < 25) {
            list_push_front(list, pages[rand() % n_pages]);
            ++count_pushes;
        } else if (rand_num < 60) {
            list_node_t* push_node = list_push_back(list, pages[rand() % n_pages]);
            ++count_pushes;

            if (rand() % 200) {
                list_move_upfront(list, push_node);
            }
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
    
    const char* key1 = "key1";
    list_node_t* node1 = create_list_node();
    
    ck_assert(!hashtable_get(htable, key1));
    
    hashtable_put(htable, key1, node1);
    ck_assert(!hashtable_is_empty(htable));
    ck_assert_uint_eq(hashtable_length(htable), 1);
    ck_assert(hashtable_get(htable, key1) == node1);

    hashtable_put(htable, key1, node1);
    // nothing should change
    ck_assert_uint_eq(hashtable_length(htable), 1);
    ck_assert(hashtable_get(htable, key1) == node1);

    hashtable_put(htable, key1, NULL);
    // NULL should overwrite node1
    ck_assert_uint_eq(hashtable_length(htable), 1);
    ck_assert(hashtable_get(htable, key1) == NULL);
    hashtable_put(htable, key1, node1);

    const char* key2 = "key2";
    list_node_t* node2 = create_list_node();
    hashtable_put(htable, key2, node2);
    ck_assert_uint_eq(hashtable_length(htable), 2);
    ck_assert(hashtable_get(htable, key1) == node1);
    ck_assert(hashtable_get(htable, key2) == node2);

    const char* key3 = "key3";
    list_node_t* node3 = create_list_node();
    hashtable_put(htable, key3, node3);
    ck_assert_uint_eq(hashtable_length(htable), 3);
    ck_assert(hashtable_get(htable, key1) == node1);
    ck_assert(hashtable_get(htable, key2) == node2);
    ck_assert(hashtable_get(htable, key3) == node3);

    bool status = hashtable_delete_entry(htable, key2);
    ck_assert(status);
    ck_assert_uint_eq(hashtable_length(htable), 2);
    ck_assert(hashtable_get(htable, key1) == node1);
    ck_assert(hashtable_get(htable, key3) == node3);

    status = hashtable_delete_entry(htable, key2);
    // nothing should change
    ck_assert(!status);
    ck_assert_uint_eq(hashtable_length(htable), 2);
    ck_assert(hashtable_get(htable, key1) == node1);
    ck_assert(hashtable_get(htable, key3) == node3);

    status = hashtable_delete_entry(htable, key1);
    ck_assert(status);
    ck_assert_uint_eq(hashtable_length(htable), 1);
    ck_assert(hashtable_get(htable, key3) == node3);
    
    status = hashtable_delete_entry(htable, key3);
    ck_assert(status);
    ck_assert_uint_eq(hashtable_length(htable), 0);
    ck_assert(hashtable_is_empty(htable));
    
    {
        char arr[20] = "key4";
        char* key4 = arr;
        hashtable_put(htable, key4, node3);
    }
    char* key4 = "key4";
    ck_assert(hashtable_get(htable, key4) == node3);

    delete_hashtable(htable);
    delete_list_node(node1);
    delete_list_node(node2);
    delete_list_node(node3);
}
END_TEST


START_TEST(test_hashtable_randomized)
{
    hashtable_t* htable = create_hashtable();
    const size_t n = RNG_TEST_SIZE;
    list_node_t** nodes = malloc(sizeof(list_node_t*) * n);
    char** keys = malloc(sizeof(char*) * n);
    for (size_t i = 0; i < n; ++i) {
        nodes[i] = create_list_node();
        keys[i] = malloc(20);
        sprintf(keys[i], "%zu", rand() % n);
    }

    const size_t n_iter = RNG_TEST_N_ITER;
    size_t count_puts = 0;
    size_t count_gets = 0;
    size_t count_dels = 0;
    size_t count_successful_dels = 0;
    for (size_t i = 0; i < n_iter; ++i) {
        size_t op_num = rand() % 100;
        char* key = keys[rand() % n];
        list_node_t* node = nodes[rand() % n];

        if (op_num < 50) {
            ++count_puts;
            hashtable_put(htable, key, node);
        } else if (op_num < 80) {
            ++count_gets;
            hashtable_get(htable, key);
        } else {
            ++count_dels;
            bool del_res = hashtable_delete_entry(htable, key);
            if (del_res) ++count_successful_dels;
        }
    }
    printf("n_iter = %zu, n_put = %zu, n_get = %zu, n_del = %zu (n successful dels = %zu)\n",
           n_iter, count_puts, count_gets, count_dels, count_successful_dels);

    for (size_t i = 0; i < n; ++i) {
        free(keys[i]);
        delete_list_node(nodes[i]);
    }
    free(keys);
    free(nodes);
    delete_hashtable(htable);
}
END_TEST


Suite* make_suite(void) {
    Suite *s = suite_create("lru_cache");

    // Page tests
    TCase *tc_page = tcase_create("Page");
    tcase_add_test(tc_page, test_page);

    // Key tests
    TCase *tc_key = tcase_create("Key");
    tcase_add_test(tc_key, test_key);
    tcase_add_test(tc_key, test_hash);

    // Clist tests
    TCase *tc_clist = tcase_create("Clist");
    tcase_add_test(tc_clist, test_list_create);
    tcase_add_test(tc_clist, test_list_push_pop);
    tcase_add_test(tc_clist, test_list_move_upfront);
    tcase_add_test(tc_clist, test_list_push_pop_randomized);

    // Chashtable tests
    TCase *tc_chashtable = tcase_create("Chashtable");
    tcase_add_test(tc_chashtable, test_hashtable_create);
    tcase_add_test(tc_chashtable, test_hashtable_put_get_delete);
    tcase_add_test(tc_chashtable, test_hashtable_randomized);

    suite_add_tcase(s, tc_page);
    suite_add_tcase(s, tc_key);
    suite_add_tcase(s, tc_clist);
    suite_add_tcase(s, tc_chashtable);
    return s;
}


int main(void) {
    puts("Run tests");
    Suite *suite = make_suite();
    SRunner *runner = srunner_create(suite);
    srunner_run_all(runner, CK_NORMAL);
    int n_failed = srunner_ntests_failed(runner);
    srunner_free(runner);
    return (n_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
