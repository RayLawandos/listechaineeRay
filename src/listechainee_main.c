/*
 * listechainee_main.c
 *
 * Fichier source principale du menu de gestion de liste chainee
 * Affiche le menu, saisie la commande a executer puis lance la commande
 */

#include <stdio.h>
#include <strings.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>

#include "list/list.h"

/* Forward decl main */
int main(int argc, char** argv);

/* List chainee courante */
listechainee_ptr curlist = (listechainee_ptr)NULL;

extern char* optarg;
static int v_opt = 0;
static int h_opt = 0;
static int d_opt = 0;
static int i_opt = 0; 
static int l_opt = 0;
static int s_opt = 0;
static int t_opt = 0;
static int A_opt = 0;
static int P_opt = 0;
static int I_opt = 0;
static int x_opt = 0;
static int X_opt = 0;
static int r_opt = 0;
static int q_opt = 0;
static int N_opt = 0;
char *progname = (char *)NULL;
char* arg_filename = (char *)NULL;
int arg_elem = 0;

void
print_help(void)
{
  fprintf(stdout, "%s: usage: %s [-hvdAPxXqN] [-l <filename>] [-s <filename>] [-t|-I|-r <elem>]\n", progname, progname);
}

int main(int argc, char** argv)
{
  int c;
  int digit_optind = 0;

  progname = (progname = rindex(argv[0], '/')) ? progname+1 : argv[0];
  init_list();
  
  while (1) {
    int this_option_optind = optind ? optind : 1;
    int option_index = 0;
    static struct option long_options[] = {
      {"help",    no_argument,       0,  'h'},
      {"verbose", no_argument,       0,  'v'},
      {"display", no_argument,       0,  'd'},
      {"load",    required_argument, 0,  'l' },
      {"save",    required_argument, 0,  's' },
      {"test",    required_argument, 0,  't' },
      {"append",  required_argument, 0,  'A'},
      {"prepend", required_argument, 0,  'P' },
      {"insert",  required_argument, 0,  'I' },
      {"del",     no_argument,       0,  'x' },
      {"delend",  no_argument,       0,  'X' },
      {"remove",  required_argument, 0,  'r' },
      {"quit",    no_argument,       0,  'q' },
      {"inter",   no_argument,       0,  'N' },
      {0,         0,                 0,  0 }
    };
    
    c = getopt_long(argc, argv, "hvdl:s:t:A:P:I:xXr:qN",
                    long_options, &option_index);
    if (c == -1)
      break;
               
    switch (c) {

    case 'h':
      h_opt = 1;
      if (v_opt)
        fprintf(stdout, "%s: info: option 'h' was set !\n", progname);
      break;

    case 'v':
      v_opt++;
      if (v_opt)
        fprintf(stdout, "%s: info: option 'v' was incremented for verbosity !\n", progname);
      break;

    case 'd':
      d_opt = 1;
      if (v_opt)
        fprintf(stdout, "%s: info: option 'd' was set !\n", progname);
      break;

    case 'l':
      l_opt = 1;
      arg_filename = optarg;
      if (v_opt)
        fprintf(stdout, "%s: info: option 'l' was called for file '%s' !\n", progname, arg_filename);
      break;

    case 's':
      s_opt = 1;
      arg_filename = optarg;
      if (v_opt)
        fprintf(stdout, "%s: info: option 's' was called for file '%s' !\n", progname, arg_filename);
      break;

    case 't':
      t_opt = 1;
      arg_elem = atoi(optarg);
      if (v_opt)
        fprintf(stdout, "%s: info: option 't' was called for elem '%d' !\n", progname, arg_elem);
      break;

    case 'A':
      A_opt = 1;
      arg_elem = atoi(optarg);
      if (v_opt)
        fprintf(stdout, "%s: info: option 'A' was called for appending '%d' !\n", progname, arg_elem);
      break;

    case 'P':
      P_opt = 1;
      arg_elem = atoi(optarg);
      if (v_opt)
        fprintf(stdout, "%s: info: option 'P' was called for prepending '%d' !\n", progname, arg_elem);
      break;

    case 'I':
      I_opt = 1;
      arg_elem = atoi(optarg);
      if (v_opt)
        fprintf(stdout, "%s: info: option 'I' was called with elem '%d' !\n", progname, arg_elem);
      break;

    case 'x':
      x_opt = 1;
      if (v_opt)
        fprintf(stdout, "%s: info: option 'x' was set !\n", progname);
      break;

    case 'X':
      X_opt = 1;
      if (v_opt)
        fprintf(stdout, "%s: info: option 'X' was set !\n", progname);
      break;

    case 'r':
      r_opt = 1;
      arg_elem = atoi(optarg);
      if (v_opt)
        fprintf(stdout, "%s: info: option 'r' was called with elem '%d' !\n", progname, arg_elem);
      break;

    case 'q':
      q_opt = 1;
      if (v_opt)
        fprintf(stdout, "%s: info: option 'q' was set !\n", progname);
      break;

    case 'N':
      N_opt = 1;
      if (v_opt)
        fprintf(stdout, "%s: info: option 'N' was set ! Entering interactive menu ...\n", progname);
      break;

    case '?':
      break;

    default:
      printf("?? getopt returned character code 0%o ??\n", c);
    }

    if (A_opt)
      {
        if (v_opt)
          fprintf(stdout, "%s: info: appending '%d'\n", progname, arg_elem);
        A_opt = 0;
        listechainee_ptr tmplist = append_list(curlist, arg_elem);
        if (tmplist == NULL)
          fprintf(stderr, "%s: error: element '%d' already in list\n", progname, arg_elem);
        else
          curlist = tmplist;
      }
    else if (P_opt)
      {
        if (v_opt)
          fprintf(stdout, "%s: info: prepending '%d'\n", progname, arg_elem);
        P_opt = 0;
        listechainee_ptr tmplist = prepend_list(curlist, arg_elem);
        if (tmplist == NULL)
          fprintf(stderr, "%s: error: element '%d' already in list\n", progname, arg_elem);
        else
          curlist = tmplist;
      }
    else if (d_opt)
      {
        if (v_opt)
          fprintf(stdout, "%s: info: Displaying list\n", progname);
        d_opt = 0;
        display_list(curlist);
      }
  }

  if (optind < argc)
    {
      printf("non-option ARGV-elements: ");
      while (optind < argc)
        printf("%s ", argv[optind++]);
      printf("\n");
    }

  if (h_opt)
    {
      print_help();
    }
  
  return 0;
}
