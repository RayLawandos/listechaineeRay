/*
 * cmd-tests.h
 *
 * Declarations for unitary tests for command 
 */

#pragma once

/*
 * Command pathnames
 */
#define COMMAND_STRING              "listechainee"
#define LOCAL_COMMAND_STRING        "./" COMMAND_STRING 
#define DEBUG_COMMAND_STRING        "./Debug/" COMMAND_STRING 
#define RELEASE_COMMAND_STRING      "./Release/" COMMAND_STRING
#define RELWDEBINFO_COMMAND_STRING  "./RelWithDebInfo/" COMMAND_STRING

#define USED_COMMAND_STRING         DEBUG_COMMAND_STRING

extern const char const* command_pathname;

/*
 * Declarations for command test functions
 */

// Stat command pathname to find it
const char const* stat_listechainee(const char*);

// Basic command tests
void test_command_basic_invocation_0(void);
void test_command_basic_invocation_v(void);
void test_command_basic_invocation_vh(void);
void test_command_basic_invocation_verbose(void);
void test_command_basic_invocation_verbose_help(void);
void test_command_basic_invocation_A1d(void);
void test_command_basic_invocation_P2d(void);
void test_command_basic_invocation_append_3_display(void);
void test_command_basic_invocation_prepend_4_display(void);
void test_command_basic_invocation_A1P2d(void);
void test_command_basic_invocation_append_1_prepend_2_display(void);
void test_command_basic_invocation_vA1d(void);
void test_command_basic_invocation_vP2d(void);
void test_command_basic_invocation_verbose_append_3_display(void);
void test_command_basic_invocation_verbose_prepend_4_display(void);
void test_command_basic_invocation_vA1P2d(void);
void test_command_basic_invocation_verbose_append_1_prepend_2_display(void);
void test_command_basic_invocation_vA1P2ds_testlist1_l(void);
void test_command_basic_invocation_verbose_append_1_prepend_2_display_save_testlist1_l(void);
void test_command_basic_invocation_vl_testlist1_l_d(void);
void test_command_basic_invocation_verbose_load_testlist1_l_display(void);
void test_command_basic_invocation_vl_testlist1_notfound_l_d(void);
void test_command_basic_invocation_verbose_load_testlist1_notfound_l_display(void);
void test_command_basic_invocation_A1A2A3dr1dP4dr2dr4dr3dr3d(void);
void test_command_basic_invocation_A1A2A3dI41d(void);
void test_command_basic_invocation_A1A2A3dI42d(void);
void test_command_basic_invocation_A1A2A3dI43d(void);
void test_command_basic_invocation_A1A2A3dI44d(void);
void test_command_basic_invocation_A1A2A3dI12d(void);
void test_command_basic_invocation_A1A2A3dI4plus0d(void);
void test_command_basic_invocation_A1A2A3dI4plus1d(void);
void test_command_basic_invocation_A1A2A3dI4plus2d(void);
void test_command_basic_invocation_A1A2A3dI4plus3d(void);
void test_command_basic_invocation_A1A2A3dI1plus0d(void);
void test_command_basic_invocation_l_testlist1_l_dt1t2t3(void);
void test_command_basic_invocation_vl_testlist1_l_dt1t2t3(void);
void test_command_basic_invocation_l_testlist1_l_A1dr2d(void);
void test_command_basic_invocation_l_testlist1_l_A1dr1d(void);
void test_command_basic_invocation_l_testlist1_l_A1dr3d(void);
void test_command_basic_invocation_l_testlist1_l_A1dr4d(void);
// Interactive menu tests
void test_command_menu_invocation_append_display(void);
void test_command_menu_invocation_help_quit(void);
void test_command_menu_invocation_load_insert_x_2_display_quit(void);


/**
 * vim: et:ts=4:sw=4:sts=4																																				   
 * -*- mode: C; coding: utf-8-unix; tab-width: 4; tab-always-indent: t; tab-first-completion: nil -*-
 */
