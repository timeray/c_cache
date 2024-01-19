#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include "../page.h"
#include "../clist.h"


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

}


START_TEST(test_list_create)
{
    list_t* list = create_list();

    ck_assert_msg(list_length(list) == 0, "failure");
    delete_list(list);
}
END_TEST


START_TEST(test_list_push_pop)
{
    list_t* list = create_list();

    page_t* page1 = create_page("page1");
    page_t* page2 = create_page("page2");

    list_push_front(list, page1);
    ck_assert_uint_eq(list_length(list), 1);
    page_t* head = list_front(list);
    page_t* tail = list_back(list);
    ck_assert_ptr_nonnull(head);
    ck_assert_ptr_nonnull(tail);
    ck_assert(head == tail);
    ck_assert(head == page1);

    list_push_back(list, page2);

    delete_page(page1);
    delete_page(page2);
    delete_list(list);
}


Suite* list_suite(void) {
    Suite *s = suite_create("lru_cache");

    // Page tests
    TCase *tc_page = tcase_create("Page");
    tcase_add_test(tc_page, test_page);

    // Clist tests
    TCase *tc_clist = tcase_create("Clist");
    tcase_add_test(tc_clist, test_list_create);
    tcase_add_test(tc_clist, test_list_push_pop);

    suite_add_tcase(s, tc_page);
    suite_add_tcase(s, tc_clist);
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
