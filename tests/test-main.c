/*
 * test-main.c
 *
 * main routine for tests
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>

extern int errno;
extern int sys_nerr;
extern const char * const sys_errlist[];

#include <CUnit/Basic.h>

#include "list/list.h"
#include "list-tests.h"			// Include tests decl for list library
#include "cmd-tests.h"			// Include tests decl for command line behaviour

/* List chainee courante */
listechainee_ptr curlist = (listechainee_ptr)NULL;

/*
 * Forward decl for our functions
 */
int main(int, char**);
int init_suite_tests4list(void);
int clean_suite_tests4list(void);
int init_suite_tests4cmd(void);
int clean_suite_tests4cmd(void);


/*
 * ==============================================================================
 *                                            Suite for testing the list library
 *
 */

/* The suite initialization function.
 * Returns zero on success, non-zero otherwise.
 */
int init_suite_tests4list(void)
{
  return 0;
}

/* The suite cleanup function.
 * Returns zero on success, non-zero otherwise.
 */
int clean_suite_tests4list(void)
{
  return 0;
}

/*
 * ==============================================================================
 *                                        Suite for testing the command behavior
 *
 */

/* The suite initialization function.
 * Returns zero on success, non-zero otherwise.
 */
int init_suite_tests4cmd(void)
{
  return 0;
}

/* The suite cleanup function.
 * Returns zero on success, non-zero otherwise.
 */
int clean_suite_tests4cmd(void)
{
  return 0;
}

/*
 * ==============================================================================
 *                                                                Main for tests
 *
 */

/*
 * main
 *
 * The main function for the tests entry point. It creates tghe tests suites,
 * provision the tests for each test suite and finally executes all tests.
 */
int
main(int argc, char** argv)
{
  int testres = 0;  
  CU_pSuite pSuite = NULL;

  /* Extract the program name from the first argument */
  char* progname = (progname = rindex(argv[0], '/')) ? progname+1 : argv[0];

  /* If a pathname was provided for a command */
  if (argc == 1)
    command_pathname = stat_listechainee(NULL);
  else
    for (int n = 1; n < argc; n++)
      if ((command_pathname = stat_listechainee(NULL)) != NULL)
        break;

  if (command_pathname == NULL)
    {
      fprintf(stderr, "%s: error: cannot find listechainee command to test\n", progname);
      exit(1);
    }

  /* initialize the CUnit test registry */
  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  /* add a suite for testing the list library to the registry */
  pSuite = CU_add_suite("Suite_4_list", init_suite_tests4list, clean_suite_tests4list);
  if (NULL == pSuite) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  /* add the tests to the suite */
  /* 6 tests */
  if ((NULL == CU_add_test(pSuite, "test of test_init_list", 
                           test_init_list)) ||
      (NULL == CU_add_test(pSuite, "test of test_reinit_list", 
                           test_reinit_list)) ||
      (NULL == CU_add_test(pSuite, "test of test_ix_in_list with null list", 
                           test_ix_in_list_with_list_null)) ||
      (NULL == CU_add_test(pSuite, "test of test_ix_in_list with list with ix", 
                           test_ix_in_list_with_list_with_ix)) ||
      (NULL == CU_add_test(pSuite, "test of test_ix_in_list with list without ix", 
                           test_ix_in_list_with_list_without_ix)) ||
      (NULL == CU_add_test(pSuite, "test of test_elem_in_list with null list", 
                           test_elem_in_list_with_null_list)) ||
      (NULL == CU_add_test(pSuite, "test of test_elem_in_list with non null list", 
                           test_elem_in_list_with_non_null_list1)) ||
      (NULL == CU_add_test(pSuite, "test of test_elem_in_list with non null list (int limit)", 
                           test_elem_in_list_with_non_null_list2)) ||
      (NULL == CU_add_test(pSuite, "test of test_elem_in_list with non null list (int limit) #2", 
                           test_elem_in_list_with_non_null_list3)) ||
      (NULL == CU_add_test(pSuite, "test of test_append_list_with_list_null", 
                           test_append_list_with_list_null)) ||
      (NULL == CU_add_test(pSuite, "test of test_append_list_with_list_non_null", 
                           test_append_list_with_list_non_null)) ||
      (NULL == CU_add_test(pSuite, "test of test_prepend_list_with_list_null", 
                           test_prepend_list_with_list_null)) ||
      (NULL == CU_add_test(pSuite, "test of test_prepend_list_with_list_non_null", 
                           test_prepend_list_with_list_non_null))
      )
    {
      CU_cleanup_registry();
      return CU_get_error();
    }

  /* add a suite to the registry */
  /* 1 test */
  pSuite = CU_add_suite("Suite_4_command", init_suite_tests4cmd, clean_suite_tests4cmd);
  if (NULL == pSuite) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  /* add the tests for the command to the suite */
  if ((NULL == CU_add_test(pSuite, "test basic command launch ",
                           test_command_basic_invocation_0)) ||
      (NULL == CU_add_test(pSuite, "test basic command launch verbose (-v)",
                           test_command_basic_invocation_v)) ||
      (NULL == CU_add_test(pSuite, "test command launch verbose+help (-v -h)",
                           test_command_basic_invocation_vh)) ||
      (NULL == CU_add_test(pSuite, "test command launch long verbose (--verbose)", 
                           test_command_basic_invocation_verbose)) ||
      (NULL == CU_add_test(pSuite, "test command launch long verbose+help (--verbose --help)", 
                           test_command_basic_invocation_verbose_help)) ||
      (NULL == CU_add_test(pSuite, "test command launch append+display (-A 1 -d)", 
                           test_command_basic_invocation_A1d)) ||
      (NULL == CU_add_test(pSuite, "test command launch prepend+display (-P 2 -d)", 
                           test_command_basic_invocation_P2d)) ||
      (NULL == CU_add_test(pSuite, "test command launch long append+display (--append 3 --display)", 
                           test_command_basic_invocation_append_3_display)) ||
      (NULL == CU_add_test(pSuite, "test command launch long prepend+display (--prepend 4 --display)", 
                           test_command_basic_invocation_prepend_4_display)) ||
      (NULL == CU_add_test(pSuite, "test command launch append+prepend+display (-A 1 -P 2 -d)", 
                           test_command_basic_invocation_A1P2d)) ||
      (NULL == CU_add_test(pSuite, "test command launch long append+prepend+display (--append 1 --prepend 2 --display)", 
                           test_command_basic_invocation_append_1_prepend_2_display)) ||
      (NULL == CU_add_test(pSuite, "test command launch verbose+append+display (-v -A 1 -d)", 
                           test_command_basic_invocation_vA1d)) ||
      (NULL == CU_add_test(pSuite, "test command launch verbose+prepend+display (-v -P 2 -d)", 
                           test_command_basic_invocation_vP2d)) ||
      (NULL == CU_add_test(pSuite, "test command launch long verbose+append+display (--verbose --append 3 --display)", 
                           test_command_basic_invocation_verbose_append_3_display)) ||
      (NULL == CU_add_test(pSuite, "test command launch long verbose+prepend+display (--verbose --prepend 4 --display)", 
                           test_command_basic_invocation_verbose_prepend_4_display)) ||
      (NULL == CU_add_test(pSuite, "test command launch verbose+append+prepend+display (-v -A 1 -P 2 -d)", 
                           test_command_basic_invocation_vA1P2d)) ||
      (NULL == CU_add_test(pSuite, "test command launch long verbose+append+prepend+display (--verbose --append 1 --prepend 2 --display)", 
                           test_command_basic_invocation_verbose_append_1_prepend_2_display)) ||
      (NULL == CU_add_test(pSuite, "test command launch verbose+append+prepend+display+save (-v -A 1 -P 2 -d -s testlist1.l)", 
                           test_command_basic_invocation_vA1P2ds_testlist1_l)) ||
      (NULL == CU_add_test(pSuite, "test command launch long verbose+append+prepend+display+save (--verbose --append 1 --prepend 2 --display --save testlist1.l)", 
                           test_command_basic_invocation_verbose_append_1_prepend_2_display_save_testlist1_l)) ||
      (NULL == CU_add_test(pSuite, "test command launch verbose+load+display (-v -l testlist1.l -d)", 
                           test_command_basic_invocation_vl_testlist1_l_d)) ||
      (NULL == CU_add_test(pSuite, "test command launch long verbose+loaderr+display (--verbose --load testlist1-notfound.l --display)", 
                           test_command_basic_invocation_verbose_load_testlist1_notfound_l_display)) ||
      (NULL == CU_add_test(pSuite, "test command launch verbose+loaderr+display (-v -l testlist1-notfound.l -d)", 
                           test_command_basic_invocation_vl_testlist1_notfound_l_d)) ||
      (NULL == CU_add_test(pSuite, "test command launch append+remove+prepend+remove (-A 1 -A 2 -A 3 -d -r 1 -d -P 4 -d -r 2 -d -r 4 -d -r 3 -d -r 3 -d)", 
                           test_command_basic_invocation_A1A2A3dr1dP4dr2dr4dr3dr3d)) ||
      (NULL == CU_add_test(pSuite, "test command launch append+insert 4,1 (-A 1 -A 2 -A 3 -d -I4,1 -d)", 
                           test_command_basic_invocation_A1A2A3dI41d)) ||
      (NULL == CU_add_test(pSuite, "test command launch append+insert 4,2 (-A 1 -A 2 -A 3 -d -I4,2 -d)", 
                           test_command_basic_invocation_A1A2A3dI42d)) ||
      (NULL == CU_add_test(pSuite, "test command launch append+insert 4,3 (-A 1 -A 2 -A 3 -d -I4,3 -d)", 
                           test_command_basic_invocation_A1A2A3dI43d)) ||
      (NULL == CU_add_test(pSuite, "test command launch append+insert 4,4 (-A 1 -A 2 -A 3 -d -I4,4 -d)", 
                           test_command_basic_invocation_A1A2A3dI44d)) ||
      (NULL == CU_add_test(pSuite, "test command launch append+insert 1,2 (-A 1 -A 2 -A 3 -d -I1,2 -d)", 
                           test_command_basic_invocation_A1A2A3dI44d)) ||
      (NULL == CU_add_test(pSuite, "test command launch append+insert 4,+0 (-A 1 -A 2 -A 3 -d -I4,+0 -d)", 
                           test_command_basic_invocation_A1A2A3dI4plus0d)) ||
      (NULL == CU_add_test(pSuite, "test command launch append+insert 4,+1 (-A 1 -A 2 -A 3 -d -I4,+1 -d)", 
                           test_command_basic_invocation_A1A2A3dI4plus1d)) ||
      (NULL == CU_add_test(pSuite, "test command launch append+insert 4,+2 (-A 1 -A 2 -A 3 -d -I4,+2 -d)", 
                           test_command_basic_invocation_A1A2A3dI4plus2d)) ||
      (NULL == CU_add_test(pSuite, "test command launch append+insert 4,+3 (-A 1 -A 2 -A 3 -d -I4,+3 -d)", 
                           test_command_basic_invocation_A1A2A3dI4plus3d)) ||
      (NULL == CU_add_test(pSuite, "test command launch append+insert 1,+0 (-A 1 -A 2 -A 3 -d -I1,+0 -d)", 
                           test_command_basic_invocation_A1A2A3dI1plus0d)) ||
      (NULL == CU_add_test(pSuite, "test command launch load+display+test1+test2+test3 (-l testlist1.l -d -t 1 -t 2 -t 3)", 
                           test_command_basic_invocation_l_testlist1_l_dt1t2t3)) ||
      (NULL == CU_add_test(pSuite, "test command launch verbose+load+display+test1+test2+test3 (-v -l testlist1.l -d -t 1 -t 2 -t 3)", 
                           test_command_basic_invocation_vl_testlist1_l_dt1t2t3)) ||
      (NULL == CU_add_test(pSuite, "test command launch load+append+display+remove2 (-l testlist1.l -A 3 -d -r 2)", 
                           test_command_basic_invocation_l_testlist1_l_A1dr2d)) ||
      (NULL == CU_add_test(pSuite, "test command launch load+append+display+remove1 (-l testlist1.l -A 3 -d -r 1)", 
                           test_command_basic_invocation_l_testlist1_l_A1dr1d)) ||
      (NULL == CU_add_test(pSuite, "test command launch load+append+display+remove3 (-l testlist1.l -A 3 -d -r 3)", 
                           test_command_basic_invocation_l_testlist1_l_A1dr3d)) ||
      (NULL == CU_add_test(pSuite, "test command launch load+append+display+remove4 (-l testlist1.l -A 3 -d -r 4)", 
                           test_command_basic_invocation_l_testlist1_l_A1dr4d)) ||
      (NULL == CU_add_test(pSuite, "test menu launch (-N) << 'f 1 f 2 a q'", 
                           test_command_menu_invocation_append_display)) ||
      (NULL == CU_add_test(pSuite, "test menu launch (-N) << 'm q'", 
                           test_command_menu_invocation_help_quit)) ||
      (NULL == CU_add_test(pSuite, "test menu launch (-N) << 'c testmenu.l a h 4 1 a h 5 3 a q'", 
                           test_command_menu_invocation_load_insert_x_2_display_quit))
      )
    {
      CU_cleanup_registry();
      return CU_get_error();
    }
   
  /* Run all tests using the CUnit Basic interface */
  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  testres = CU_get_error();

  struct timespec ts_req, ts_rem;
  ts_req.tv_sec = 5;
  ts_req.tv_nsec = 0L;
  if (nanosleep((const struct timespec*)&ts_req, &ts_rem) == -1)
    {
      if (errno == EFAULT)
        {
          perror("nanosleep");
          exit(1);
        }
      else if (errno == EINTR)
        do
          {
            if (errno == EFAULT)
              {
                perror("nanosleep loop");
                exit(1);
              }
            ts_req.tv_sec = ts_rem.tv_sec;
            ts_req.tv_nsec = ts_rem.tv_nsec;
          }
        while(nanosleep((const struct timespec*)&ts_req, &ts_rem));
    }
  
  return testres;
}

/**
 * vim: et:ts=4:sw=4:sts=4																																				   
 * -*- mode: C; coding: utf-8-unix; tab-width: 4; tab-always-indent: t; tab-first-completion: nil -*-
 */
