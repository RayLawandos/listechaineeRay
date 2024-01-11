/*
 * test-main.c
 *
 * main routine for tests
 */

#include <stdio.h>
#include <CUnit/Basic.h>

#include "list/list.h"
#include "list-tests.h"

/* List chainee courante */
listechainee_ptr curlist = (listechainee_ptr)NULL;

/*
 * Forward decl for our functions
 */
int main(int, char**);
int init_suite1(void);
int clean_suite1(void);

/* The suite initialization function.
 * Opens the temporary file used by the tests.
 * Returns zero on success, non-zero otherwise.
 */
int init_suite1(void)
{
}

/* The suite cleanup function.
 * Closes the temporary file used by the tests.
 * Returns zero on success, non-zero otherwise.
 */
int clean_suite1(void)
{
}

int
main(int argc, char** argv)
{
   CU_pSuite pSuite = NULL;

   /* initialize the CUnit test registry */
   if (CUE_SUCCESS != CU_initialize_registry())
      return CU_get_error();

   /* add a suite to the registry */
   pSuite = CU_add_suite("Suite_1", init_suite1, clean_suite1);
   if (NULL == pSuite) {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* add the tests to the suite */
   /* NOTE - ORDER IS IMPORTANT - MUST TEST fread() AFTER fprintf() */
   if ((NULL == CU_add_test(pSuite, "test of test_ix_in_list_with_list_null", test_ix_in_list_with_list_null)) ||
       (NULL == CU_add_test(pSuite, "test of test_ix_in_list_with_list_with_ix", test_ix_in_list_with_list_with_ix)) ||
       (NULL == CU_add_test(pSuite, "test of test_ix_in_list_with_list_without_ix", test_ix_in_list_with_list_without_ix)))
   {
      CU_cleanup_registry();
      return CU_get_error();
   }

   /* Run all tests using the CUnit Basic interface */
   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   CU_cleanup_registry();
   return CU_get_error();  
}
