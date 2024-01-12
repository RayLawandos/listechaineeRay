# Minimum cmake version
cmake_minimum_required(VERSION 3.13.4)

# Project properties: C project
project(ListeChainee
  VERSION 1.0.0
  LANGUAGES C
)

# Add executables of projects
add_library(list list/list.c)
add_executable(listechainee src/listechainee_main.c)
include_directories(BEFORE "${PROJECT_SOURCE_DIR}")
target_link_libraries(listechainee list)
add_executable(testlistechainee tests/test-main.c tests/list-tests.c tests/cmd-tests.c)
target_link_libraries(testlistechainee list -lcunit)
add_test(testlistechainee testlistechainee)

