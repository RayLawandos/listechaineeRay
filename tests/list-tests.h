/*
 * list-tests.h
 *
 * Declarations for unitary tests for list functions
 */

#pragma once

void test_1_1_init_list(void);
void test_1_2_reinit_list(void);
void test_1_3_ix_in_list_with_list_null(void);
void test_1_4_ix_in_list_with_list_with_ix(void);
void test_1_5_ix_in_list_with_list_without_ix(void);
void test_1_6_elem_in_list_with_null_list(void);
void test_1_7_elem_in_list_with_non_null_list1(void);
void test_1_8_elem_in_list_with_non_null_list2(void);
void test_1_9_elem_in_list_with_non_null_list3(void);
void test_1_10_append_list_with_list_null(void);
void test_1_11_append_list_with_list_non_null(void);
void test_1_12_prepend_list_with_list_null(void);
void test_1_13_prepend_list_with_list_non_null(void);

/**
 * vim: et:ts=4:sw=4:sts=4																																				   
 * -*- mode: C; coding: utf-8-unix; tab-width: 4; tab-always-indent: t; tab-first-completion: nil -*-
 */
