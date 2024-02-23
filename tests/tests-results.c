/**
 * tests-results.c
 *
 * Files containing the strings for comparing command invocations output
 */

#include <stddef.h>

#include "tests-results.h"

/*
 * The help message of the command
 */
#define HELP_MESSAGE        \
    "listechainee: usage: listechainee [-hvdxXqNi] [-l|-s <filename>] [-t|-A|-P <elem>] [-I <elem,[+]pos>] [-r <elem>]            \n" \
    "                                 Options:                                                                                    \n" \
    "                                    [-h|--help]                  display this help message                                   \n" \
    "                                    [-v|--verbose]               increase verbosity                                          \n" \
    "                                 Commands:                                                                                   \n" \
    "                                    [-i|--init]                  init a linked list                                          \n" \
    "                                    [-l|--load <filename>]       load a linked list from a file <filename>                   \n" \
    "                                    [-s|--save <filename>]       save a linked list in a file <filename>                     \n" \
    "                                    [-t|--test <elem>]           test if an element <elem> is contained in the list          \n" \
    "                                    [-A|--append <elem>]         append the element <elem> in the list                       \n" \
    "                                    [-P|--prepend <elem>]        prepend the element <elem> in the list                      \n" \
    "                                    [-I|--insert <elem,[+]pos>]  insert the element <elem> in the list at position <pos>.    \n" \
    "                                                                 <pos> is the index of the element at which the insertion    \n" \
    "                                                                 will occcur after, if <pos> starts with '+'. Otherwise <pos>\n" \
    "                                                                 is the value of the element after which the insertion will  \n" \
    "                                                                 occur.                                                      \n" \
    "                                    [-x|--delhead]               remove head of the list.                                    \n" \
    "                                    [-X|--deltail]               remove tail of the list.                                    \n" \
    "                                    [-r|--remove <elem>]         remove the element <elem> in the list at position <pos>.    \n" \
    "                                                                 <pos> is the index of the element at which the insertion    \n" \
    "                                                                 will occcur after, if <pos> starts with '+'. Otherwise <pos>\n" \
    "                                                                 is the value of the element after which the insertion will  \n" \
    "                                                                 occur.                                                      \n" \
    "                                    [-q|--quit]                  quit.                                                       \n" \
    "                                    [-N|--interactive]           enter interactive menu                                      \n" \
    "                                                                                                                             \n" \
    "Copyright ©2024 EFREI - Rémi COHEN SCALI                                                                                     \n"

/*
 * The menu of the interactive command
 */
#define MENU_MESSAGE                                                \
    "Menu:\n"                                                       \
    "----:\n"                                                       \
    "     a) Display current list\n"                                \
    "     b) Reset/Init current list\n"                             \
    "     c) Load current list from file\n"                         \
    "     d) Save current list to file\n"                           \
    "     e) Test if an element is in current list\n"               \
    "     f) Append an element to current list\n"                   \
    "     g) Prepend an element to current list\n"                  \
    "     h) Insert element after an element in current list\n"     \
    "     H) Insert element after index in current list\n"          \
    "     i) Delete at end of current list\n"                       \
    "     j) Delete at start of current list\n"                     \
    "     k) Remove element from current list\n"                    \
    "     m) Display help for the menu\n"                           \
    "     q) Exit the menu\n"                                       \
    "Select an entry by its letter ? \n"

/*
 * The help of the interactive command menu
 */
#define MENU_HELP_MESSAGE                                                       \
    "Help for interactive menu:\n"                                              \
    "-------------------------\n"                                               \
    "Enter the letter for selecting a command. Accepted commands are:\n"        \
    "'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'H', 'i', 'j', 'k', 'm', 'q'\n"    \
    "If a command request an argument, this argument value is asked.\n"         \
    "The following commands request an argument value:\n"                       \
    " 'c': Load the list file which name is provided.\n"                        \
    " 'd': Save the list file which name is provided.\n"                        \
    " 'e': Test if provided element is in the list.\n"                          \
    " 'f': Append the provided element at end of list.\n"                       \
    " 'g': Prepend the provided element at start of list.\n"                    \
    " 'h': Insert the provided element after another provided element.\n"       \
    " 'H': Insert the provided element after the provided index.\n"             \
    " 'k': Remove from list the provided element.\n"                            \
    "Others commands 'a', 'b', 'i', 'j', 'm', 'q' use no extra argument.\n"

/*
 * These are the strings to provide as input for child process
 */
char* instrs[NB_STDIN_ENTRIES] =
  {
    /*                                                                                   maintain the value of the counter ->    */
    /* ====================================================================================================================  0 = */
    /*  == $ listechainee -N ...                                                                                               = */
    "f\n1\nf\n2\na\nq\n",
    /* ====================================================================================================================  1 = */
    /*  == $ listechainee -N ...                                                                                               = */
    "m\nq\n",
    /* ====================================================================================================================  2 = */
    /*  == $ listechainee ...                                                                                                  = */
    "c\ntestmenu.l\na\nh\n4\n1\na\nh\n5\n3\na\nq\n",
    /* ====================================================================================================================  3 = */
    /*  == $ listechainee ...                                                                                                  = */
    (char*)NULL,
    /*  ==                                                       ==> Check this value with the define NB_STDIN_ENTRIES ==  4 <== */
  };

/*
 * These are the expected results as strings
 */
char* results[NB_STDOUT_RESULTS] =
  {
    /*                                                                                   maintain the value of the counter ->    */
    /* ===================================================================================================================== 0 = */
    /*  == $ listechainee                                                                                                      = */
    HELP_MESSAGE,
    /* ===================================================================================================================== 1 = */
    /*  == $ listechainee -v                                                                                                   = */
    "listechainee: info: option 'v' was incremented for verbosity !\n"
    HELP_MESSAGE,
    /* ===================================================================================================================== 2 = */
    /*  == $ listechainee -v -h                                                                                                = */
    "listechainee: info: option 'v' was incremented for verbosity !\n"
    "listechainee: info: option 'h' was set !\n"
    HELP_MESSAGE,
    /* ===================================================================================================================== 3 = */
    /*  == $ listechainee --verbose                                                                                            = */
    "listechainee: info: option 'v' was incremented for verbosity !\n"
    HELP_MESSAGE,
    /* ===================================================================================================================== 4 = */
    /*  == $ listechainee --verbose --help                                                                                     = */
    "listechainee: info: option 'v' was incremented for verbosity !\n"
    "listechainee: info: option 'h' was set !\n"
    HELP_MESSAGE,
    /* ===================================================================================================================== 5 = */
    /*  == $ listechainee -A 1 -d                                                                                              = */
    "List: [1]\n",
    /* ===================================================================================================================== 6 = */
    /*  == $ listechainee -P 2 -d                                                                                              = */
    "List: [2]\n",
    /* ===================================================================================================================== 7 = */
    /*  == $ listechainee --append 3 --display                                                                                 = */
    "List: [3]\n",
    /* ===================================================================================================================== 8 = */
    /*  == $ listechainee --prepend 4 --display                                                                                = */
    "List: [4]\n",
    /* ===================================================================================================================== 9 = */
    /*  == $ listechainee -A 1 -P 2 -d                                                                                         = */
    "List: [2,1]\n",
    /* ==================================================================================================================== 10 = */
    /*  == $ listechainee --append 1 --prepend 2 --display                                                                     = */
    "List: [2,1]\n",
    /* ==================================================================================================================== 11 = */
    /*  == $ listechainee -v -A 1 -d                                                                                           = */
    "listechainee: info: option 'v' was incremented for verbosity !\n"
    "listechainee: info: option 'A' was called for appending '1' !\n"
    "listechainee: info: appending '1'\n"
    "listechainee: info: option 'd' was set !\n"
    "listechainee: info: Displaying list\n"
    "List: [1]\n",
    /* ==================================================================================================================== 12 = */
    /*  == $ listechainee -v -P 2 -d                                                                                           = */
    "listechainee: info: option 'v' was incremented for verbosity !\n"
    "listechainee: info: option 'P' was called for prepending '2' !\n"
    "listechainee: info: prepending '2'\n"
    "listechainee: info: option 'd' was set !\n"
    "listechainee: info: Displaying list\n"
    "List: [2]\n",
    /* ==================================================================================================================== 13 = */
    /*  == $ listechainee --verbose --append 3 --display                                                                       = */
    "listechainee: info: option 'v' was incremented for verbosity !\n"
    "listechainee: info: option 'A' was called for appending '3' !\n"
    "listechainee: info: appending '3'\n"
    "listechainee: info: option 'd' was set !\n"
    "listechainee: info: Displaying list\n"
    "List: [3]\n",
    /* ==================================================================================================================== 14 = */
    /*  == $ listechainee --verbose --prepend 4 --display                                                                      = */
    "listechainee: info: option 'v' was incremented for verbosity !\n"
    "listechainee: info: option 'P' was called for prepending '4' !\n"
    "listechainee: info: prepending '4'\n"
    "listechainee: info: option 'd' was set !\n"
    "listechainee: info: Displaying list\n"
    "List: [4]\n",
    /* ==================================================================================================================== 15 = */
    /*  == $ listechainee -v -A 1 -P 2 -d                                                                                      = */
    "listechainee: info: option 'v' was incremented for verbosity !\n"
    "listechainee: info: option 'A' was called for appending '1' !\n"
    "listechainee: info: appending '1'\n"
    "listechainee: info: option 'P' was called for prepending '2' !\n"
    "listechainee: info: prepending '2'\n"
    "listechainee: info: option 'd' was set !\n"
    "listechainee: info: Displaying list\n"
    "List: [2,1]\n",
    /* ==================================================================================================================== 16 = */
    /*  == $ listechainee --verbose --append 1 --prepend 2 --display                                                           = */
    "listechainee: info: option 'v' was incremented for verbosity !\n"
    "listechainee: info: option 'A' was called for appending '1' !\n"
    "listechainee: info: appending '1'\n"
    "listechainee: info: option 'P' was called for prepending '2' !\n"
    "listechainee: info: prepending '2'\n"
    "listechainee: info: option 'd' was set !\n"
    "listechainee: info: Displaying list\n"
    "List: [2,1]\n",
    /* ==================================================================================================================== 17 = */
    /*  == $ listechainee -v -A 1 -P 2 -d -s testlist1.l                                                                       = */
    "listechainee: info: option 'v' was incremented for verbosity !\n"
    "listechainee: info: option 'A' was called for appending '1' !\n"
    "listechainee: info: appending '1'\n"
    "listechainee: info: option 'P' was called for prepending '2' !\n"
    "listechainee: info: prepending '2'\n"
    "listechainee: info: option 'd' was set !\n"
    "listechainee: info: Displaying list\n"
    "List: [2,1]\n"
    "listechainee: info: option 's' was called for file 'testlist1.l' !\n"
    "listechainee: info: Loading/Saving from/to filename 'testlist1.l'\n"
    "listechainee: info: list was successfully saved to file 'testlist1.l'\n",
    /* ==================================================================================================================== 18 = */
    /*  == $ listechainee --verbose --append 1 --prepend 2 --display --save testlist1.l                                        = */
    "listechainee: info: option 'v' was incremented for verbosity !\n"
    "listechainee: info: option 'A' was called for appending '1' !\n"
    "listechainee: info: appending '1'\n"
    "listechainee: info: option 'P' was called for prepending '2' !\n"
    "listechainee: info: prepending '2'\n"
    "listechainee: info: option 'd' was set !\n"
    "listechainee: info: Displaying list\n"
    "List: [2,1]\n"
    "listechainee: info: option 's' was called for file 'testlist1.l' !\n"
    "listechainee: info: Loading/Saving from/to filename 'testlist1.l'\n"
    "listechainee: info: list was successfully saved to file 'testlist1.l'\n",
    /* ==================================================================================================================== 19 = */
    /*  == $ listechainee -v -l testlist1.l -d                                                                                 = */
    "listechainee: info: option 'v' was incremented for verbosity !\n"
    "listechainee: info: option 'l' was called for file 'testlist1.l' !\n"
    "listechainee: info: Loading/Saving from/to filename 'testlist1.l'\n"
    "listechainee: info: list was successfully loaded from file 'testlist1.l'\n"
    "listechainee: info: option 'd' was set !\n"
    "listechainee: info: Displaying list\n"
    "List: [2,1]\n",
    /* ==================================================================================================================== 20 = */
    /*  == $ listechainee --verbose --load testlist1.l --display                                                               = */
    "listechainee: info: option 'v' was incremented for verbosity !\n"
    "listechainee: info: option 'l' was called for file 'testlist1.l' !\n"
    "listechainee: info: Loading/Saving from/to filename 'testlist1.l'\n"
    "listechainee: info: list was successfully loaded from file 'testlist1.l'\n"
    "listechainee: info: option 'd' was set !\n"
    "listechainee: info: Displaying list\n"
    "List: [2,1]\n",
    /* ==================================================================================================================== 21 = */
    /*  == $ listechainee -v -l testlist1-notfound.l -d                                                                        = */
    "listechainee: info: option 'v' was incremented for verbosity !\n"
    "listechainee: info: option 'l' was called for file 'testlist1-notfound.l' !\n"
    "listechainee: info: Loading/Saving from/to filename 'testlist1-notfound.l'\n"
    "listechainee: info: option 'd' was set !\n"
    "listechainee: info: Displaying list\n"
    "Empty list\n",
    /* ==================================================================================================================== 22 = */
    /*  == $ listechainee --verbose --load testlist1-notfound.l --display                                                      = */
    "listechainee: info: option 'v' was incremented for verbosity !\n"
    "listechainee: info: option 'l' was called for file 'testlist1-notfound.l' !\n"
    "listechainee: info: Loading/Saving from/to filename 'testlist1-notfound.l'\n"
    "listechainee: info: option 'd' was set !\n"
    "listechainee: info: Displaying list\n"
    "Empty list\n",
    /* ==================================================================================================================== 23 = */
    /*  == $ listechainee -A 1 -A 2 -A 3 -d -r 1 -d -P 4 -d -r 2 -d -r 4 -d -r 3 -d -r 3 -d                                    = */
    "List: [1,2,3]\n"
    "List: [2,3]\n"
    "List: [4,2,3]\n"
    "List: [4,3]\n"
    "List: [3]\n"
    "Empty list\n"
    "Empty list\n",
    /* ==================================================================================================================== 24 = */
    /*  == $ listechainee -A 1 -A 2 -A 3 -d -I 4,1 -d                                                                          = */
    "List: [1,2,3]\n"
    "List: [1,4,2,3]\n",
    /* ==================================================================================================================== 25 = */
    /*  == $ listechainee -A 1 -A 2 -A 3 -d -I 4,2 -d                                                                          = */
    "List: [1,2,3]\n"
    "List: [1,2,4,3]\n",
    /* ==================================================================================================================== 26 = */
    /*  == $ listechainee -A 1 -A 2 -A 3 -d -I 4,3 -d                                                                          = */
    "List: [1,2,3]\n"
    "List: [1,2,3,4]\n",
    /* ==================================================================================================================== 27 = */
    /*  == $ listechainee -A 1 -A 2 -A 3 -d -I 4,4 -d                                                                          = */
    "List: [1,2,3]\n"
    "List: [1,2,3]\n",
    /* ==================================================================================================================== 28 = */
    /*  == $ listechainee -A 1 -A 2 -A 3 -d -I 1,2 -d                                                                          = */
    "List: [1,2,3]\n"
    "List: [1,2,3]\n",
    /* ==================================================================================================================== 29 = */
    /*  == $ listechainee -A 1 -A 2 -A 3 -d -I 4,1 -d                                                                          = */
    "List: [1,2,3]\n"
    "List: [1,4,2,3]\n",
    /* ==================================================================================================================== 30 = */
    /*  == $ listechainee -A 1 -A 2 -A 3 -d -I 4,2 -d                                                                          = */
    "List: [1,2,3]\n"
    "List: [1,2,4,3]\n",
    /* ==================================================================================================================== 31 = */
    /*  == $ listechainee -A 1 -A 2 -A 3 -d -I 4,3 -d                                                                          = */
    "List: [1,2,3]\n"
    "List: [1,2,3,4]\n",
    /* ==================================================================================================================== 32 = */
    /*  == $ listechainee -A 1 -A 2 -A 3 -d -I 4,4 -d                                                                          = */
    "List: [1,2,3]\n"
    "List: [1,2,3]\n",
    /* ==================================================================================================================== 33 = */
    /*  == $ listechainee -A 1 -A 2 -A 3 -d -I 1,+0 -d                                                                         = */
    "List: [1,2,3]\n"
    "List: [1,2,3]\n",
    /* ==================================================================================================================== 34 = */
    /*  == $ listechainee -l testlist1.l -d -t 1 -t 2 -t 3                                                                     = */
    "List: [2,1]\n"
    "Element '1' is in the list.\n"
    "Element '2' is in the list.\n"
    "Element '3' is not in the list.\n",
    /* ==================================================================================================================== 35 = */
    /*  == $ listechainee -v -l testlist1.l -d -t 1 -t 2 -t 3                                                                  = */
    "listechainee: info: option 'v' was incremented for verbosity !\n"
    "listechainee: info: option 'l' was called for file 'testlist1.l' !\n"
    "listechainee: info: Loading/Saving from/to filename 'testlist1.l'\n"
    "listechainee: info: list was successfully loaded from file 'testlist1.l'\n"
    "listechainee: info: option 'd' was set !\n"
    "listechainee: info: Displaying list\n"
    "List: [2,1]\n"
    "listechainee: info: option 't' was called for elem '1' !\n"
    "listechainee: info: testing if element '1' is in the list\n"
    "Element '1' is in the list.\n"
    "listechainee: info: option 't' was called for elem '2' !\n"
    "listechainee: info: testing if element '2' is in the list\n"
    "Element '2' is in the list.\n"
    "listechainee: info: option 't' was called for elem '3' !\n"
    "listechainee: info: testing if element '3' is in the list\n"
    "Element '3' is not in the list.\n",
    /* ==================================================================================================================== 36 = */
    /*  == $ listechainee -l testlist1.l -d -A 3 -d -r 2 -d                                                                    = */
    "List: [2,1,3]\n"
    "List: [1,3]\n",
    /* ==================================================================================================================== 37 = */
    /*  == $ listechainee -l testlist1.l -d -A 3 -d -r 1 -d                                                                    = */
    "List: [2,1,3]\n"
    "List: [2,3]\n",
    /* ==================================================================================================================== 38 = */
    /*  == $ listechainee -l testlist1.l -d -A 3 -d -r 3 -d                                                                    = */
    "List: [2,1,3]\n"
    "List: [2,1]\n",
    /* ==================================================================================================================== 39 = */
    /*  == $ listechainee -l testlist1.l -d -A 3 -d -r 4 -d                                                                    = */
    "List: [2,1,3]\n"
    "List: [2,1,3]\n",
    /* ==================================================================================================================== 40 = */
    /*  == $ listechainee -N << 'f 1 f 2 a q'                                                                                  = */
    "\n"
    MENU_MESSAGE
    "Element to append ? "
    "List: [1]\n"
    "\n"
    MENU_MESSAGE
    "Element to append ? "
    "List: [1,2]\n"
    "\n"
    MENU_MESSAGE
    "List: [1,2]\n"
    "\n"
    MENU_MESSAGE
    "List: [1,2]\n",
    /* ==================================================================================================================== 41 = */
    /*  == $ listechainee -N << 'm q'                                                                                          = */
    "\n"
    MENU_MESSAGE
    "\n"
    MENU_HELP_MESSAGE
    "\n"
    MENU_MESSAGE
    "Empty list\n",
    /* ==================================================================================================================== 42 = */
    /*  == $ listechainee -N << 'c testmenu.l a h 4 1 a h 5 3 a q '                                                            = */
    "\n"
    MENU_MESSAGE
    "Filename to load ? "
    "List: [1,2,3]\n"
    "\n"
    MENU_MESSAGE
    "List: [1,2,3]\n"
    "\n"
    MENU_MESSAGE
    "Element to insert ? "
    "Insert after element ? \n"
    MENU_MESSAGE
    "List: [1,4,2,3]\n"
    "\n"
    MENU_MESSAGE
    "Element to insert ? "
    "Insert after element ? \n"
    MENU_MESSAGE
    "List: [1,4,2,3,5]\n"
    "\n"
    MENU_MESSAGE
    "List: [1,4,2,3,5]\n",
    /* ==================================================================================================================== 43 = */
    /*  == The End                                                                                                             = */
    (char*)NULL
    /*  ==                                                      ==> Check this value with the define NB_STDOUT_RESULTS == 44 <== */
  };

/*
 * These are the expected results from stderr as strings
 */
char* errres[NB_STDERR_RESULTS] =
  {
    /*                                                                                   maintain the value of the counter ->    */
    /* ====================================================================================================================  0 = */
    /*  == $ listechainee                                                                                                      = */
    "listechainee: error: no option was specified!\n",
    /* ====================================================================================================================  1 = */
    /*  == $ listechainee -v -l testlist1-notfound.l -d                                                                        = */
    "listechainee: error: cannot load list from file 'testlist1-notfound.l': No such file or directory",
    /* ====================================================================================================================  2 = */
    /*  == $ listechainee --verbose --load testlist1-notfound.l --display                                                      = */
    "listechainee: error: cannot load list from file 'testlist1-notfound.l': No such file or directory\n",
    /* ====================================================================================================================  3 = */
    /*  == The End                                                                                                             = */
    (char*)NULL
    /*  ==                                                      ==> Check this value with the define NB_STDERR_RESULTS ==  4 <== */
  };

/**
 * vim: et:ts=4:sw=4:sts=4																																				   
 * -*- mode: C; coding: utf-8-unix; tab-width: 4; tab-always-indent: t; tab-first-completion: nil -*-
 */
