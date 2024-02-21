/*
 * list-tests.h
 *
 * Declarations for unitary tests for list functions
 */

#pragma once

void test_init_list(void);
void test_reinit_list(void);
void test_ix_in_list_with_list_null(void);
void test_ix_in_list_with_list_with_ix(void);
void test_ix_in_list_with_list_without_ix(void);
void test_elem_in_list_with_null_list(void);
void test_elem_in_list_with_non_null_list1(void);
void test_elem_in_list_with_non_null_list2(void);
void test_elem_in_list_with_non_null_list3(void);
void test_append_list_with_list_null(void);
void test_append_list_with_list_non_null(void);
void test_prepend_list_with_list_null(void);
void test_prepend_list_with_list_non_null(void);

/**
 * vim: et:ts=4:sw=4:sts=4																																				   
 * -*- mode: C; coding: utf-8-unix; tab-width: 4; tab-always-indent: t; tab-first-completion: nil -*-
 */
