/*
 * listechainee_main.c
 *
 * Fichier source principale du menu de gestion de liste chainee
 * Affiche le menu, saisie la commande a executer puis lance la commande
 */

#include <stdio.h>
#include <strings.h>
#include <unistd.h>
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
char* arg_filename;

int main(int argc, char** argv)
{
  int c;
  int digit_optind = 0;

  progname = (progname = rindex(argv[0], '/')) ? argv[0] : progname+1;
  
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
      {"append",  no_argument,       0,  'A'},
      {"prepend", no_argument,       0,  'P' },
      {"insert",  required_argument, 0,  'I' },
      {"del",     no_argument,       0,  'x' },
      {"delend",  no_argument,       0,  'X' },
      {"remove",  required_argument, 0,  'r' },
      {"quit",    no_argument,       0,  'q' },
      {"inter",   no_argument,       0,  'N' },
      {0,         0,                 0,  0 }
    };
    
    c = getopt_long(argc, argv, "abc:d:012",
                    long_options, &option_index);
    if (c == -1)
      break;
               
    switch (c) {
    case 'd':
      d_opt = 1;
      if (v_opt)
        fprintf(stdout, "%s: Info: option 'd' was set !\n", progname);
      break;

    case 'i':
      break;

    case 'c':
      break;

    case 'h':
      printf("option d with value '%s'\n", optarg);
      break;

    case '?':
      break;

    default:
      printf("?? getopt returned character code 0%o ??\n", c);
    }
  }

  if (optind < argc) {
    printf("non-option ARGV-elements: ");
    while (optind < argc)
      printf("%s ", argv[optind++]);
    printf("\n");
  }
  
  
  return 0;
}
