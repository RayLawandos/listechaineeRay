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

// Global env vars pointer
char** global_envp = (char**)NULL;

#include <CUnit/Basic.h>

#include "src/log.h"					// Include the logging header
#include "list/list.h"					// Include the list header
#include "tests/list-tests.h"			// Include tests decl for list library
#include "tests/cmd-tests.h"			// Include tests decl for command line behaviour

/* Wait for the childprocess to terminate before the parent process ends */
#define NUMBER_SECONDS_TO_WAIT 		0
#define NUMBER_NANOSECONDS_TO_WAIT 	200000000L

/* List chainee courante */
listechainee_ptr curlist = (listechainee_ptr)NULL;

/*
 * Forward decl for our functions
 */
int main(int, char**, char**);
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
  // Erase all expected_|effective log files
  system("/bin/rm -f expected_*.log effective_*.log");
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
main(int argc, char** argv, char** envp)
{
  int testres = 0;  
  CU_pSuite pSuite = NULL;

  /* Save env ptr in global var */
  global_envp = envp;

  /* Extract the program name from the first argument */
  char* progname = (progname = rindex(argv[0], '/')) ? progname+1 : argv[0];

  /* Get current date */
  time_t current_time = time((time_t*)NULL);
  char* datestr = ctime(&current_time);  
  datestr[strlen(datestr)-1] = '\0';
  char* date_tmp = datestr;
  while(*date_tmp)
    {
      if (*date_tmp == ' ')
        *date_tmp = '_';
      date_tmp++;
    }
  
  /* Init logging */
  char logfilepath[1024];
  snprintf(logfilepath, 1024, "%s_%s.log", progname, datestr);
  loglvl = LOG_LVL_INFO;
#ifdef DO_LOG
  logfp = init_log(logfilepath, loglvl);
#endif /* DO_LOG */

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
  if ((NULL == CU_add_test(pSuite, "test #1.01: test_1_1_init_list", 
                           test_1_1_init_list)) ||
      (NULL == CU_add_test(pSuite, "test #1.02: test_1_2_reinit_list", 
                           test_1_2_reinit_list)) ||
      (NULL == CU_add_test(pSuite, "test #1.03: test_1_3_ix_in_list with null list", 
                           test_1_3_ix_in_list_with_list_null)) ||
      (NULL == CU_add_test(pSuite, "test #1.04: test_1_4_ix_in_list with list with ix", 
                           test_1_4_ix_in_list_with_list_with_ix)) ||
      (NULL == CU_add_test(pSuite, "test #1.05: test_1_5_ix_in_list with list without ix", 
                           test_1_5_ix_in_list_with_list_without_ix)) ||
      (NULL == CU_add_test(pSuite, "test #1.06: test_1_6_elem_in_list with null list", 
                           test_1_6_elem_in_list_with_null_list)) ||
      (NULL == CU_add_test(pSuite, "test #1.07: test_1_7_elem_in_list with non null list", 
                           test_1_7_elem_in_list_with_non_null_list1)) ||
      (NULL == CU_add_test(pSuite, "test #1.08: test_1_8_elem_in_list with non null list (int limit)", 
                           test_1_8_elem_in_list_with_non_null_list2)) ||
      (NULL == CU_add_test(pSuite, "test #1.09: test_1_9_elem_in_list with non null list (int limit) #2", 
                           test_1_9_elem_in_list_with_non_null_list3)) ||
      (NULL == CU_add_test(pSuite, "test #1.10: test_1_10_append_list_with_list_null", 
                           test_1_10_append_list_with_list_null)) ||
      (NULL == CU_add_test(pSuite, "test #1.11: test_1_11_append_list_with_list_non_null", 
                           test_1_11_append_list_with_list_non_null)) ||
      (NULL == CU_add_test(pSuite, "test #1.12: test_1_12_prepend_list_with_list_null", 
                           test_1_12_prepend_list_with_list_null)) ||
      (NULL == CU_add_test(pSuite, "test #1.13: test_1_13_prepend_list_with_list_non_null", 
                           test_1_13_prepend_list_with_list_non_null))
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
  if ((NULL == CU_add_test(pSuite, "test #2.01: basic command launch ",
                           test_2_1_command_basic_invocation_0)) ||
      (NULL == CU_add_test(pSuite, "test #2.02: basic command launch verbose (-v)",
                           test_2_2_command_basic_invocation_v)) ||
      (NULL == CU_add_test(pSuite, "test #2.03: command launch verbose+help (-v -h)",
                           test_2_3_command_basic_invocation_vh)) ||
      (NULL == CU_add_test(pSuite, "test #2.04: command launch long verbose (--verbose)", 
                           test_2_4_command_basic_invocation_verbose)) ||
      (NULL == CU_add_test(pSuite, "test #2.05: command launch long verbose+help (--verbose --help)", 
                           test_2_5_command_basic_invocation_verbose_help)) ||
      (NULL == CU_add_test(pSuite, "test #2.06: command launch append+display (-A 1 -d)", 
                           test_2_6_command_basic_invocation_A1d)) ||
      (NULL == CU_add_test(pSuite, "test #2.07: command launch prepend+display (-P 2 -d)", 
                           test_2_7_command_basic_invocation_P2d)) ||
      (NULL == CU_add_test(pSuite, "test #2.08: command launch long append+display (--append 3 --display)", 
                           test_2_8_command_basic_invocation_append_3_display)) ||
      (NULL == CU_add_test(pSuite, "test #2.09: command launch long prepend+display (--prepend 4 --display)", 
                           test_2_9_command_basic_invocation_prepend_4_display)) ||
      (NULL == CU_add_test(pSuite, "test #2.10: command launch append+prepend+display (-A 1 -P 2 -d)", 
                           test_2_10_command_basic_invocation_A1P2d)) ||
      (NULL == CU_add_test(pSuite, "test #2.11: command launch long append+prepend+display (--append 1 --prepend 2 --display)", 
                           test_2_11_command_basic_invocation_append_1_prepend_2_display)) ||
      (NULL == CU_add_test(pSuite, "test #2.12: command launch verbose+append+display (-v -A 1 -d)", 
                           test_2_12_command_basic_invocation_vA1d)) ||
      (NULL == CU_add_test(pSuite, "test #2.13: command launch verbose+prepend+display (-v -P 2 -d)", 
                           test_2_13_command_basic_invocation_vP2d)) ||
      (NULL == CU_add_test(pSuite, "test #2.14: command launch long verbose+append+display (--verbose --append 3 --display)", 
                           test_2_14_command_basic_invocation_verbose_append_3_display)) ||
      (NULL == CU_add_test(pSuite, "test #2.15: command launch long verbose+prepend+display (--verbose --prepend 4 --display)", 
                           test_2_15_command_basic_invocation_verbose_prepend_4_display)) ||
      (NULL == CU_add_test(pSuite, "test #2.16: command launch verbose+append+prepend+display (-v -A 1 -P 2 -d)", 
                           test_2_16_command_basic_invocation_vA1P2d)) ||
      (NULL == CU_add_test(pSuite, "test #2.17: command launch long verbose+append+prepend+display (--verbose --append 1 --prepend 2 --display)", 
                           test_2_17_command_basic_invocation_verbose_append_1_prepend_2_display)) ||
      (NULL == CU_add_test(pSuite, "test #2.18: command launch verbose+append+prepend+display+save (-v -A 1 -P 2 -d -s testlist1.l)", 
                           test_2_18_command_basic_invocation_vA1P2ds_testlist1_l)) ||
      (NULL == CU_add_test(pSuite, "test #2.19: command launch long verbose+append+prepend+display+save (--verbose --append 1 --prepend 2 --display --save testlist1.l)", 
                           test_2_19_command_basic_invocation_verbose_append_1_prepend_2_display_save_testlist1_l)) ||
      (NULL == CU_add_test(pSuite, "test #2.20: command launch verbose+load+display (-v -l testlist1.l -d)", 
                           test_2_20_command_basic_invocation_vl_testlist1_l_d)) ||
      (NULL == CU_add_test(pSuite, "test #2.21: command launch verbose+load+display (--verbose -load testlist1.l --display)", 
                           test_2_21_command_basic_invocation_verbose_load_testlist1_l_display)) ||
      (NULL == CU_add_test(pSuite, "test #2.22: command launch verbose+loaderr+display (-v -l testlist1-notfound.l -d)", 
                           test_2_22_command_basic_invocation_vl_testlist1_notfound_l_d)) ||
      (NULL == CU_add_test(pSuite, "test #2.23: command launch long verbose+loaderr+display (--verbose --load testlist1-notfound.l --display)", 
                           test_2_23_command_basic_invocation_verbose_load_testlist1_notfound_l_display)) ||
      (NULL == CU_add_test(pSuite, "test #2.24: command launch append+remove+prepend+remove (-A 1 -A 2 -A 3 -d -r 1 -d -P 4 -d -r 2 -d -r 4 -d -r 3 -d -r 3 -d)", 
                           test_2_24_command_basic_invocation_A1A2A3dr1dP4dr2dr4dr3dr3d)) ||
      (NULL == CU_add_test(pSuite, "test #2.25: command launch append+insert 4,1 (-A 1 -A 2 -A 3 -d -I4,1 -d)", 
                           test_2_25_command_basic_invocation_A1A2A3dI41d)) ||
      (NULL == CU_add_test(pSuite, "test #2.26: command launch append+insert 4,2 (-A 1 -A 2 -A 3 -d -I4,2 -d)", 
                           test_2_26_command_basic_invocation_A1A2A3dI42d)) ||
      (NULL == CU_add_test(pSuite, "test #2.27: command launch append+insert 4,3 (-A 1 -A 2 -A 3 -d -I4,3 -d)", 
                           test_2_27_command_basic_invocation_A1A2A3dI43d)) ||
      (NULL == CU_add_test(pSuite, "test #2.28: command launch append+insert 1,2 (-A 1 -A 2 -A 3 -d -I4,4 -d)", 
                           test_2_28_command_basic_invocation_A1A2A3dI44d)) ||
      (NULL == CU_add_test(pSuite, "test #2.29: command launch append+insert 1,2 (-A 1 -A 2 -A 3 -d -I1,2 -d)", 
                           test_2_29_command_basic_invocation_A1A2A3dI12d)) ||
      (NULL == CU_add_test(pSuite, "test #2.30: command launch append+insert 4,+0 (-A 1 -A 2 -A 3 -d -I4,+0 -d)", 
                           test_2_30_command_basic_invocation_A1A2A3dI4plus0d)) ||
      (NULL == CU_add_test(pSuite, "test #2.31: command launch append+insert 4,+1 (-A 1 -A 2 -A 3 -d -I4,+1 -d)", 
                           test_2_31_command_basic_invocation_A1A2A3dI4plus1d)) ||
      (NULL == CU_add_test(pSuite, "test #2.32: command launch append+insert 4,+2 (-A 1 -A 2 -A 3 -d -I4,+2 -d)", 
                           test_2_32_command_basic_invocation_A1A2A3dI4plus2d)) ||
      (NULL == CU_add_test(pSuite, "test #2.33: command launch append+insert 4,+3 (-A 1 -A 2 -A 3 -d -I4,+3 -d)", 
                           test_2_33_command_basic_invocation_A1A2A3dI4plus3d)) ||
      (NULL == CU_add_test(pSuite, "test #2.34: command launch append+insert 1,+0 (-A 1 -A 2 -A 3 -d -I1,+0 -d)", 
                           test_2_34_command_basic_invocation_A1A2A3dI1plus0d)) ||
      (NULL == CU_add_test(pSuite, "test #2.35: command launch load+display+test1+test2+test3 (-l testlist1.l -d -t 1 -t 2 -t 3)", 
                           test_2_35_command_basic_invocation_l_testlist1_l_dt1t2t3)) ||
      (NULL == CU_add_test(pSuite, "test #2.36: command launch verbose+load+display+test1+test2+test3 (-v -l testlist1.l -d -t 1 -t 2 -t 3)", 
                           test_2_36_command_basic_invocation_vl_testlist1_l_dt1t2t3)) ||
      (NULL == CU_add_test(pSuite, "test #2.37: command launch load+append+display+remove2 (-l testlist1.l -A 3 -d -r 2)", 
                           test_2_37_command_basic_invocation_l_testlist1_l_A1dr2d)) ||
      (NULL == CU_add_test(pSuite, "test #2.38: command launch load+append+display+remove1 (-l testlist1.l -A 3 -d -r 1)", 
                           test_2_38_command_basic_invocation_l_testlist1_l_A1dr1d)) ||
      (NULL == CU_add_test(pSuite, "test #2.39: command launch load+append+display+remove3 (-l testlist1.l -A 3 -d -r 3)", 
                           test_2_39_command_basic_invocation_l_testlist1_l_A1dr3d)) ||
      (NULL == CU_add_test(pSuite, "test #2.40: command launch load+append+display+remove4 (-l testlist1.l -A 3 -d -r 4)", 
                           test_2_40_command_basic_invocation_l_testlist1_l_A1dr4d)) ||
      (NULL == CU_add_test(pSuite, "test #2.41: menu launch (-N) and append+append+display+quit << 'f 1 f 2 a q'", 
                           test_2_41_command_menu_invocation_append_display)) ||
      (NULL == CU_add_test(pSuite, "test #2.42: menu launch (-N) and help+quit << 'm q'", 
                           test_2_42_command_menu_invocation_help_quit)) ||
      (NULL == CU_add_test(pSuite, "test #2.43: menu launch (-N) loadfile+display+insert+display+insert+display+quit << 'c testmenu.l a h 4 1 a h 5 3 a q'", 
                           test_2_43_command_menu_invocation_load_insert_x_2_display_quit))
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

  /* Initialize the timespec structure for nanosleep */
  struct timespec ts_req, ts_rem;
  // Wait 1s
  ts_req.tv_sec = NUMBER_SECONDS_TO_WAIT;
  // And wait half a second more
  ts_req.tv_nsec = NUMBER_NANOSECONDS_TO_WAIT;
  // Do wait ...
  if (nanosleep((const struct timespec*)&ts_req, &ts_rem) == -1)
    {
      // If nanosleep was interrupted because of an error
      if (errno == EFAULT)
        {
          // Report it
          perror("nanosleep");
          exit(1);
        }
      // Else nanosleep was interrupted because of a signal or anything else
      else if (errno == EINTR)
        // Let's loop in wait again
        do
          {
            // If error occured
            if (errno == EFAULT)
              {
                // Report it
                perror("nanosleep loop");
                exit(1);
              }
            // Set the remaining time to wait (from ts_rem) as time to wait in ts_req
            ts_req.tv_sec = ts_rem.tv_sec;
            ts_req.tv_nsec = ts_rem.tv_nsec;
          }
        // Then do wait again
        while(nanosleep((const struct timespec*)&ts_req, &ts_rem));
    }

#ifdef DO_LOG
  // Terminate logging
  end_log(logfp);
#endif /* DO_LOG */
  
  // Return the test main function
  return testres;
}

/**
 * vim: et:ts=4:sw=4:sts=4																																				   
 * -*- mode: C; coding: utf-8-unix; tab-width: 4; tab-always-indent: t; tab-first-completion: nil -*-
 */
