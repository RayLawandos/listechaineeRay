/*
 * listechainee_main.c
 *
 * Fichier source principale du menu de gestion de liste chainee
 * Affiche le menu, saisie la commande a executer puis lance la commande
 */

#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <time.h>

#include "src/log.h"					// Include logging header
#include "list/list.h"					// Include list header

// Message maximum size
#define ERR_MESSAGE_MAX_SIZE    1024

// Global env vars pointer
char** global_envp = (char**)NULL;

/* Forward decl main */
int main(int, char**, char**);

/* Long options specification: see 'man 3 getopt' */
static struct option long_options[] =
  {
    {"help",          no_argument,       0,  'h' }, /* --help|-h */
    {"verbose",       no_argument,       0,  'v' }, /* --verbose|-v */
    {"display",       no_argument,       0,  'd' }, /* --display|-d */
    {"init",          no_argument,       0,  'i' }, /* --init|-i */
    {"load",          required_argument, 0,  'l' }, /* --load|-l */
    {"save",          required_argument, 0,  's' }, /* --save|-s */
    {"test",          required_argument, 0,  't' }, /* --test|-t */
    {"append",        required_argument, 0,  'A' }, /* --append|-A */
    {"prepend",       required_argument, 0,  'P' }, /* --prepend|-P */
    {"insert",        required_argument, 0,  'I' }, /* --insert|-I */
    {"delhead",       no_argument,       0,  'x' }, /* --delhead|-x */
    {"deltail",       no_argument,       0,  'X' }, /* --deltail|-X */
    {"remove",        required_argument, 0,  'r' }, /* --remove|-r */
    {"quit",          no_argument,       0,  'q' }, /* --quit|-q */
    {"interactive",   no_argument,       0,  'N' }, /* --interactive|-N */
    {0,               0,                 0,  0 }
  };
    
/* List chainee courante */
listechainee_ptr curlist = (listechainee_ptr)NULL;

/*
 * Options variables
 */
extern char* optarg;            // Option argument pointer
static int one_opt = 0;         // Option set if no command were requested (error)
static int v_opt = 0;           // Option for verbosity
static int h_opt = 0;           // Option for help
static int d_opt = 0;           // Option for displaying list 
static int l_opt = 0;           // Option for loading file
static int s_opt = 0;           // Option for saving file
static int i_opt = 0;           // Option for initializing the list
static int t_opt = 0;           // Option for testing element
static int A_opt = 0;           // Option for appending element
static int P_opt = 0;           // Option for prepending element
static int I_opt = 0;           // Option for inserting element
static int x_opt = 0;           // Option for deleting last element
static int X_opt = 0;           // Option for deleting first element
static int r_opt = 0;           // Option for removing specific element
static int q_opt = 0;           // Option for quitting
static int N_opt = 0;           // Option for entering interactive menu

/*
 * Values used for processing options argument
 */
char *progname = (char *)NULL;          // Program name
char *blankname = (char *)NULL;         // Blanked program name (removinf all chars before '/')
char* arg_filename = (char *)NULL;      // Argument handling filename
int arg_elem = 0;                       // Argument handling element
int arg_index = -1;                     // Argument handling index

/*
 * print_help
 *
 * The help message to be printed when -h option is requested
 */
void
print_help(void)
{
  fprintf(stdout, "%s: usage: %s [-hvdxXqNi] [-l|-s <filename>] [-t|-A|-P <elem>] [-I <elem,[+]pos>] [-r <elem>]            \n", progname, progname);
  fprintf(stdout, "%s         %sOptions:                                                                                    \n", blankname, blankname);
  fprintf(stdout, "%s         %s   [-h|--help]                  display this help message                                   \n", blankname, blankname);
  fprintf(stdout, "%s         %s   [-v|--verbose]               increase verbosity                                          \n", blankname, blankname);
  fprintf(stdout, "%s         %sCommands:                                                                                   \n", blankname, blankname);
  fprintf(stdout, "%s         %s   [-i|--init]                  init a linked list                                          \n", blankname, blankname);
  fprintf(stdout, "%s         %s   [-l|--load <filename>]       load a linked list from a file <filename>                   \n", blankname, blankname);
  fprintf(stdout, "%s         %s   [-s|--save <filename>]       save a linked list in a file <filename>                     \n", blankname, blankname);
  fprintf(stdout, "%s         %s   [-t|--test <elem>]           test if an element <elem> is contained in the list          \n", blankname, blankname);
  fprintf(stdout, "%s         %s   [-A|--append <elem>]         append the element <elem> in the list                       \n", blankname, blankname);
  fprintf(stdout, "%s         %s   [-P|--prepend <elem>]        prepend the element <elem> in the list                      \n", blankname, blankname);
  fprintf(stdout, "%s         %s   [-I|--insert <elem,[+]pos>]  insert the element <elem> in the list at position <pos>.    \n", blankname, blankname);
  fprintf(stdout, "%s         %s                                <pos> is the index of the element at which the insertion    \n", blankname, blankname);
  fprintf(stdout, "%s         %s                                will occcur after, if <pos> starts with '+'. Otherwise <pos>\n", blankname, blankname);
  fprintf(stdout, "%s         %s                                is the value of the element after which the insertion will  \n", blankname, blankname);
  fprintf(stdout, "%s         %s                                occur.                                                      \n", blankname, blankname);
  fprintf(stdout, "%s         %s   [-x|--delhead]               remove head of the list.                                    \n", blankname, blankname);
  fprintf(stdout, "%s         %s   [-X|--deltail]               remove tail of the list.                                    \n", blankname, blankname);
  fprintf(stdout, "%s         %s   [-r|--remove <elem>]         remove the element <elem> in the list at position <pos>.    \n", blankname, blankname);
  fprintf(stdout, "%s         %s                                <pos> is the index of the element at which the insertion    \n", blankname, blankname);
  fprintf(stdout, "%s         %s                                will occcur after, if <pos> starts with '+'. Otherwise <pos>\n", blankname, blankname);
  fprintf(stdout, "%s         %s                                is the value of the element after which the insertion will  \n", blankname, blankname);
  fprintf(stdout, "%s         %s                                occur.                                                      \n", blankname, blankname);
  fprintf(stdout, "%s         %s   [-q|--quit]                  quit.                                                       \n", blankname, blankname);
  fprintf(stdout, "%s         %s   [-N|--interactive]           enter interactive menu                                      \n", blankname, blankname);
  fprintf(stdout, "%s         %s                                                                                            \n", blankname, blankname);
  fprintf(stdout, "Copyright ©2024 EFREI - Rémi COHEN SCALI %s%s                                                            \n", blankname, blankname);
}

/*
 * print_help_menu
 */
void
print_help_menu(void)
{
  fprintf(stdout, "\nHelp for interactive menu:\n");
  fprintf(stdout, "-------------------------\n");
  fprintf(stdout, "Enter the letter for selecting a command. Accepted commands are:\n");
  fprintf(stdout, "'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'H', 'i', 'j', 'k', 'm', 'q'\n");
  fprintf(stdout, "If a command request an argument, this argument value is asked.\n");
  fprintf(stdout, "The following commands request an argument value:\n");
  fprintf(stdout, " 'c': Load the list file which name is provided.\n");
  fprintf(stdout, " 'd': Save the list file which name is provided.\n");
  fprintf(stdout, " 'e': Test if provided element is in the list.\n");
  fprintf(stdout, " 'f': Append the provided element at end of list.\n");
  fprintf(stdout, " 'g': Prepend the provided element at start of list.\n");
  fprintf(stdout, " 'h': Insert the provided element after another provided element.\n");
  fprintf(stdout, " 'H': Insert the provided element after the provided index.\n");
  fprintf(stdout, " 'k': Remove from list the provided element.\n");
  fprintf(stdout, "Others commands 'a', 'b', 'i', 'j', 'm', 'q' use no extra argument.\n");
}

/*
 * enter_interactive_mode
 *
 * Display the menu, select a command and execute it 
 */
listechainee_ptr
enter_interactive_mode(listechainee_ptr list)
{
  char entry = 0;       /* Menu entry selected */
  int element;          /* Value for an element */
  int elementat;        /* Value of an element after which to insert */
  int index;            /* Index at which inserting an element */
  char filename[50];    /* Filename to save/load */
  int menu = 1;         /* Menu loop control */
  
  while(menu)
    {
      fprintf(stdout, "\nMenu:\n");
      fprintf(stdout, "----:\n");
      fprintf(stdout, "     a) Display current list\n");
      fprintf(stdout, "     b) Reset/Init current list\n");
      fprintf(stdout, "     c) Load current list from file\n");
      fprintf(stdout, "     d) Save current list to file\n");
      fprintf(stdout, "     e) Test if an element is in current list\n");
      fprintf(stdout, "     f) Append an element to current list\n");
      fprintf(stdout, "     g) Prepend an element to current list\n");
      fprintf(stdout, "     h) Insert element after an element in current list\n");
      fprintf(stdout, "     H) Insert element after index in current list\n");
      fprintf(stdout, "     i) Delete at end of current list\n");
      fprintf(stdout, "     j) Delete at start of current list\n");
      fprintf(stdout, "     k) Remove element from current list\n");
      fprintf(stdout, "     m) Display help for the menu\n");
      fprintf(stdout, "     q) Exit the menu\n");
      fprintf(stdout, "Select an entry by its letter ? ");
      fflush(stdin);
      fscanf(stdin, "\n%c", &entry);
      fprintf(stdout, "\n");

      /* According to the menu selected */
      switch (entry)
        {
          /*
           * Display list
           */
        case 'a':
          display_list(curlist);
          break;

          /*
           * Init list
           */
        case 'b':
          init_list();
          break;

          /*
           * Load list from file
           */
        case 'c':
          fprintf(stdout, "Filename to load ? ");
          fflush(stdin);
          fscanf(stdin, "%s", (char*)&filename);
          curlist = load_list(filename);
          display_list(curlist);
          break;

          /*
           * Save list to file
           */
        case 'd':
          fprintf(stdout, "Filename to save ? ");
          fflush(stdin);
          fscanf(stdin, "%s", (char*)&filename);
          curlist = save_list(curlist, filename);
          break;

          /*
           * Test if element is in list
           */
        case 'e':
          fprintf(stdout, "Element to find ? ");
          fflush(stdin);
          fscanf(stdin, "%d", &element);
          fprintf(stdout, "List contains element '%d' : %s\n", element, test_elem_in_list(curlist, element) ? "yes" : "no");
          break;

          /*
           * Append element to the list
           */
        case 'f':
          fprintf(stdout, "Element to append ? ");
          fflush(stdin);
          fscanf(stdin, "%d", &element);
          curlist = append_list(curlist, element);
          display_list(curlist);
          break;

          /*
           * Prepend element to the list
           */
        case 'g':
          fprintf(stdout, "Element to prepend ? ");
          fflush(stdin);
          fscanf(stdin, "%d", &element);
          curlist = prepend_list(curlist, element);
          display_list(curlist);
          break;

          /*
           * Insert element in the list after an element
           */
        case 'h':
          {
            fprintf(stdout, "Element to insert ? ");
            fflush(stdin);
            fscanf(stdin, "%d", &element);
            fprintf(stdout, "Insert after element ? ");
            fflush(stdin);
            fscanf(stdin, "%d", &elementat);
            int atindex = find_elem_ix(curlist, elementat);
            curlist = insert_elem_in_list(curlist, atindex, element);
          }
          break;

          /*
           * Insert element in the list at a position
           */
        case 'H':
          fprintf(stdout, "Element to insert ? ");
          fflush(stdin);
          fscanf(stdin, "%d", &element);
          fprintf(stdout, "Insert after index ? ");
          fflush(stdin);
          fscanf(stdin, "%d", &index);
          curlist = insert_elem_in_list(curlist, index, element);            
          break;

          /*
           * Delete end of the list
           */
        case 'i':
          curlist = delete_end_of_list(curlist);
          display_list(curlist);
          break;

          /*
           * Delete head of the list
           */
        case 'j':
          curlist = delete_start_of_list(curlist);
          display_list(curlist);
          break;

          /*
           * Remove an element from the list
           */
        case 'k':
          fprintf(stdout, "Element to delete ? ");
          fflush(stdin);
          fscanf(stdin, "%d", &element);
          curlist = delete_elem_in_list(curlist, element);
          display_list(curlist);
          break;

          /*
           * Exit interactive menu
           */
        case 'q':
          menu = 0;
          break;

          /*
           * Print interactive menu help
           */
        case 'm':
          print_help_menu();
          break;

          /*
           * Default
           */
        default:
          fprintf(stdout, "\n!! Unknown option !!\n");
        }
    }

  return curlist;
}

/*
 * main
 *
 * The main function being the program entry point
 */
int
main(int argc, char** argv, char** envp)
{
  /* The variable holding the command line argument */
  int c;
  /* Option index digit */
  int digit_optind = 0;

  /* Save env vars ptr in global var */
  global_envp = envp;

  /* Extract the program name from the first argument */
  progname = (progname = rindex(argv[0], '/')) ? progname+1 : argv[0];
  /* Allocate memory for converting program name into blank string */
  blankname = strndup(progname, strlen(progname));
  /* Actually convert the program name into blank string */
  blankname = memset(blankname, (int)' ', strlen(progname));
  
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

  /* Initialize the linked list */ 
  init_list();

  /* Look */
  while (1) {
    int this_option_optind = optind ? optind : 1;
    int option_index = 0;

    /* Parse options arguments */
    if ((c = getopt_long(argc, argv,
                         "hvdl:s:i:t:A:P:I:xXr:qN",
                         long_options, &option_index)) == -1)
      break;

    /*
     * According to options parsed,
     * this first switch store state in variables ...
     */
    switch (c)
      {

        /**
         * Help
         */
      case 'h':
        h_opt = 1;
        if (v_opt)
          fprintf(stdout, "%s: info: option 'h' was set !\n", progname);
        LOG(LOG_LVL_INFO, "%s: info: option 'h' was set !\n", progname);
        break;

        /**
         * Verbose
         */
      case 'v':
        v_opt++;
        if (v_opt)
          fprintf(stdout, "%s: info: option 'v' was incremented for verbosity !\n", progname);
        LOG(LOG_LVL_INFO, "%s: info: option 'v' was incremented for verbosity !\n", progname);
        break;

        /**
         * Display
         */
      case 'd':
        d_opt = 1;
        if (v_opt)
          fprintf(stdout, "%s: info: option 'd' was set !\n", progname);
        LOG(LOG_LVL_INFO, "%s: info: option 'd' was set !\n", progname);
        break;

        /**
         * Init list
         */
      case 'i':
        i_opt = 1;
        if (v_opt)
          fprintf(stdout, "%s: info: option 'i' was set !\n", progname);
        LOG(LOG_LVL_INFO, "%s: info: option 'i' was set !\n", progname);
        break;

        /**
         * Load a list file
         */
      case 'l':
        l_opt = 1;
        arg_filename = optarg;
        if (v_opt)
          fprintf(stdout, "%s: info: option 'l' was called for file '%s' !\n", progname, arg_filename);
        LOG(LOG_LVL_INFO, "%s: info: option 'l' was called for file '%s' !\n", progname, arg_filename);
        break;

        /**
         * Save a list file
         */       
      case 's':
        s_opt = 1;
        arg_filename = optarg;
        if (v_opt)
          fprintf(stdout, "%s: info: option 's' was called for file '%s' !\n", progname, arg_filename);
        LOG(LOG_LVL_INFO, "%s: info: option 's' was called for file '%s' !\n", progname, arg_filename);
        break;

        /**
         * Test if an element is in the list
         */
      case 't':
        t_opt = 1;
        arg_elem = atoi(optarg);
        if (v_opt)
          fprintf(stdout, "%s: info: option 't' was called for elem '%d' !\n", progname, arg_elem);
        LOG(LOG_LVL_INFO, "%s: info: option 't' was called for elem '%d' !\n", progname, arg_elem);
        break;

        /**
         * Append element
         */
      case 'A':
        A_opt = 1;
        arg_elem = atoi(optarg);
        if (v_opt)
          fprintf(stdout, "%s: info: option 'A' was called for appending '%d' !\n", progname, arg_elem);
        LOG(LOG_LVL_INFO, "%s: info: option 'A' was called for appending '%d' !\n", progname, arg_elem);
        break;

        /**
         * Prepend element
         */
      case 'P':
        P_opt = 1;
        arg_elem = atoi(optarg);
        if (v_opt)
          fprintf(stdout, "%s: info: option 'P' was called for prepending '%d' !\n", progname, arg_elem);
        LOG(LOG_LVL_INFO, "%s: info: option 'P' was called for prepending '%d' !\n", progname, arg_elem);
        break;

        /**
         * Insert element
         */
      case 'I':
        {
          int refelem;
          int is_index = (index((const char*)optarg, (int)'+') != (char*)NULL);
          I_opt = 1;
          if (is_index)
            sscanf(optarg, "%d,%d", &arg_elem, &arg_index);

          else
            {
              sscanf(optarg, "%d,%d", &arg_elem, &refelem);
              arg_index = find_elem_ix(curlist, refelem);
            }
          if (v_opt)
            fprintf(stdout, "%s: info: option 'I' was called with elem '%d' and index '%d' !\n", progname, arg_elem, arg_index);
          LOG(LOG_LVL_INFO, "%s: info: option 'I' was called with elem '%d' and index '%d' !\n", progname, arg_elem, arg_index);
        }
        break;

        /**
         * Delete head of list
         */
      case 'x':
        x_opt = 1;
        if (v_opt)
          fprintf(stdout, "%s: info: option 'x' was set !\n", progname);
        LOG(LOG_LVL_INFO, "%s: info: option 'x' was set !\n", progname);
        break;

        /**
         * Delete end of list
         */
      case 'X':
        X_opt = 1;
        if (v_opt)
          fprintf(stdout, "%s: info: option 'X' was set !\n", progname);
        LOG(LOG_LVL_INFO, "%s: info: option 'X' was set !\n", progname);
        break;

        /**
         * Remove element
         */
      case 'r':
        r_opt = 1;
        arg_elem = atoi(optarg);
        if (v_opt)
          fprintf(stdout, "%s: info: option 'r' was called with elem '%d' !\n", progname, arg_elem);
        LOG(LOG_LVL_INFO, "%s: info: option 'r' was called with elem '%d' !\n", progname, arg_elem);
        break;

      case 'q':
        q_opt = 1;
        if (v_opt)
          fprintf(stdout, "%s: info: option 'q' was set !\n", progname);
        LOG(LOG_LVL_INFO, "%s: info: option 'q' was set !\n", progname);
        break;

      case 'N':
        N_opt = 1;
        if (v_opt)
          fprintf(stdout, "%s: info: option 'N' was set ! Entering interactive menu ...\n", progname);
        LOG(LOG_LVL_INFO, "%s: info: option 'N' was set ! Entering interactive menu ...\n", progname);
        break;

      case '?':
        break;

      default:
        fprintf(stderr, "?? getopt returned character code 0%o ??\n", c);
        LOG(LOG_LVL_ERR, "?? getopt returned character code 0%o ??\n", c);

      }
    
    if (v_opt > 2)
      fprintf(stdout,
              "%s: info: curlist = %p [%d, %p]\n",
              progname, curlist,
              curlist ? curlist->N: 0,
              (void*)(curlist ? curlist->next : NULL));
    LOG(LOG_LVL_INFO,
        "%s: info: curlist = %p [%d, %p]\n",
        progname, curlist,
        curlist ? curlist->N: 0,
        (void*)(curlist ? curlist->next : NULL));

    /*
     * Now,
     * according to the request stored in options variables
     * take appropriate actions ...
     */
    if (A_opt) /* Append element to list */
      {
        /* Set 'not missing command option' to false */
        one_opt = 1;
        if (v_opt)
          fprintf(stdout, "%s: info: appending '%d'\n", progname, arg_elem);
        LOG(LOG_LVL_INFO, "%s: info: appending '%d'\n", progname, arg_elem);
        A_opt = 0;
        listechainee_ptr tmplist = append_list(curlist, arg_elem);
        if (tmplist == NULL)
          fprintf(stdout, "%s: error: element '%d' already in list\n", progname, arg_elem);
        else
          curlist = tmplist;
      }
    else if (P_opt) /* Prepend element to list */
      {
        /* Set 'not missing command option' to false */
        one_opt = 1;
        if (v_opt)
          fprintf(stdout, "%s: info: prepending '%d'\n", progname, arg_elem);
        LOG(LOG_LVL_INFO, "%s: info: prepending '%d'\n", progname, arg_elem);
        P_opt = 0;
        listechainee_ptr tmplist = prepend_list(curlist, arg_elem);
        if (tmplist == NULL)
          fprintf(stdout, "%s: error: element '%d' already in list\n", progname, arg_elem);
        else
          curlist = tmplist;
      }
    else if (d_opt) /* Display list */
      {
        /* Set 'not missing command option' to false */
        one_opt = 1;
        if (v_opt)
          fprintf(stdout, "%s: info: Displaying list\n", progname);
        LOG(LOG_LVL_INFO, "%s: info: Displaying list\n", progname);
        d_opt = 0;
        display_list(curlist);
      }
    else if (x_opt || X_opt) /* Delete element at start/end of list */
      {
        /* Set 'not missing command option' to false */
        one_opt = 1;
        if (v_opt)
          fprintf(stdout, "%s: info: Deleting element at %s of list\n", progname, x_opt ? "start" : "end");
        LOG(LOG_LVL_INFO, "%s: info: Deleting element at %s of list\n", progname, x_opt ? "start" : "end");
        curlist = x_opt ? delete_start_of_list(curlist) : delete_end_of_list(curlist);
        x_opt = X_opt = 0;
      }
    else if (i_opt) /* Initialiaze current list */
      {
        one_opt = 1;        
        if (v_opt)
          fprintf(stdout, "%s: info: Initializing current list !\n", progname);
        LOG(LOG_LVL_INFO, "%s: info: Initializing current list !\n", progname);
        init_list();
      }
    else if (l_opt || s_opt)  /* Load/Save list from/to file */
      {
        /* Set 'not missing command option' to false */
        one_opt = 1;
        if (v_opt)
          fprintf(stdout, "%s: info: Loading/Saving from/to filename '%s'\n", progname, arg_filename);
        LOG(LOG_LVL_INFO, "%s: info: Loading/Saving from/to filename '%s'\n", progname, arg_filename);
        /* Load command */
        if (l_opt)
          {
            /* load the file */
            curlist = load_list(arg_filename);
            /* If an error occured */
            if (curlist == NULL)
              {
                /* Allocate the string for error message on the stack */
                char msg[ERR_MESSAGE_MAX_SIZE];
                /* Format the error message */
                snprintf(msg, ERR_MESSAGE_MAX_SIZE-1, "%s: error: cannot load list from file '%s'", progname, arg_filename);
                /* Display it */
                perror(msg);
              }
            else
              if (v_opt)
                fprintf(stdout, "%s: info: list was successfully loaded from file '%s'\n", progname, arg_filename);
            LOG(LOG_LVL_INFO, "%s: info: list was successfully loaded from file '%s'\n", progname, arg_filename);
          }
        /* Save command */
        else if (s_opt)
          {
            /* Save the file */
            if (save_list(curlist, arg_filename) == NULL)
              {
                /* If an error occured, allocate error message on the stack */
                char msg[ERR_MESSAGE_MAX_SIZE];
                /* Format it ... */
                snprintf(msg, ERR_MESSAGE_MAX_SIZE-1, "%s: error: cannot save list to file '%s'", progname, arg_filename);
                /* ... and display */
                perror(msg);
              }
            else
              if (v_opt)
                fprintf(stdout, "%s: info: list was successfully saved to file '%s'\n", progname, arg_filename);
            LOG(LOG_LVL_INFO, "%s: info: list was successfully saved to file '%s'\n", progname, arg_filename);
          }
        l_opt = s_opt = 0;
      }
    else if (t_opt) /* Test is element is in the list */
      {
        /* Set 'not missing command option' to false */
        one_opt = 1;
        /* Read element to test from command line arg */
        arg_elem = atoi(optarg);
        if (v_opt)
          fprintf(stdout, "%s: info: testing if element '%d' is in the list\n", progname, arg_elem);
        LOG(LOG_LVL_INFO, "%s: info: testing if element '%d' is in the list\n", progname, arg_elem);
        fprintf(stdout, "Element '%d' is%s in the list.\n", arg_elem, test_elem_in_list(curlist, arg_elem) ? "" : " not");
        LOG(LOG_LVL_INFO, "Element '%d' is%s in the list.\n", arg_elem, test_elem_in_list(curlist, arg_elem) ? "" : " not");
        t_opt = 0;
      }
    else if (I_opt) /* Insert element in list */
      {
        if (v_opt)
          fprintf(stdout, "%s: info: inserting element '%d' in the list at index '%d'\n", progname, arg_elem, arg_index);
        LOG(LOG_LVL_INFO, "%s: info: inserting element '%d' in the list at index '%d'\n", progname, arg_elem, arg_index);
        curlist = insert_elem_in_list(curlist, arg_index, arg_elem);        
        I_opt = 0;
      }
    else if (r_opt) /* Remove element from list */
      {
        /* Set 'not missing command option' to false */
        one_opt = 1;        
        if (v_opt)
          fprintf(stdout, "%s: info: removing element '%d' from list\n", progname, arg_elem);
        LOG(LOG_LVL_INFO, "%s: info: removing element '%d' from list\n", progname, arg_elem);
        curlist = delete_elem_in_list(curlist, arg_elem);
        r_opt = 0;
      }
    else if (N_opt) /* Enter interactive menu */
      {
        /* Set 'not missing command option' to false */
        one_opt = 1;        
      }
  }

  /*
   * Options have been processed ...
   * let's take other actions needed
   */
  if (one_opt == 0)         /* If no command option was specified ... */
    {
      /* Error ... */
      fprintf(stderr, "%s: error: no option was specified!\n", progname);
      LOG(LOG_LVL_ERR, "%s: error: no option was specified!\n", progname);
      print_help();
    }
  else
    {
      if (optind < argc)
        {
          fprintf(stdout, "non-option ARGV-elements: ");
          while (optind < argc)
            fprintf(stdout, "%s ", argv[optind++]);
          fprintf(stdout, "\n");
        }
      else if (h_opt)
        print_help();

      else if (N_opt)
        {
          curlist = enter_interactive_mode(curlist);
          display_list(curlist);
        }
    }

#ifdef DO_LOG
  end_log(logfp);
#endif /* DO_LOG */
  
  return 0;
}

/**
 * vim: et:ts=4:sw=4:sts=4																																				   
 * -*- mode: C; coding: utf-8-unix; tab-width: 4; tab-always-indent: t; tab-first-completion: nil -*-
 */
