# Minimum cmake version
cmake_minimum_required(VERSION 3.13.4)

# Project properties: C project
project(ListeChainee
  VERSION 1.0.0
  LANGUAGES C
)

# Add executables of projects
add_executable(listechainee src/listechainee_main.c)
include_directories(BEFORE "${PROJECT_SOURCE_DIR}")
add_library(list list/list.c)
add_executable(testlistechainee tests/test-main.c tests/list-tests.c)
set_target_properties(testlistechainee
  PROPERTIES LD_FLAGS "-llist -lcunit"
)
add_test(testlistechainee testlistechainee)

