/*
 * cmd-tests.c
 *
 * Unitary tests for command behaviour
 * These tests test pre-condition, post-conditions and functional 
 */

#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <fcntl.h> 
#include <errno.h>

extern int errno;
extern int sys_nerr;
extern const char * const sys_errlist[];
extern char** global_envp;

#include <CUnit/Basic.h>

#include "list/list.h"
#include "tests/cmd-tests.h"
#include "tests/tests-results.h"

/* Buffers used for reading the command output */
char outbuf[BUFFER_MAX_SIZE];
char errbuf[BUFFER_MAX_SIZE];

/* Command to launch */
const char const* command_pathname = (const char const*)NULL;

/*
 * stat_listechainee
 *
 * Try all known command path in order to determine which to use
 */
const char const*
stat_listechainee(const char* pathname)
{
  struct stat statst;

  /* Test provided path */
  if (pathname && stat(pathname, &statst) == 0)
    return (const char const*)pathname;
  /* If no path provided, test default command to use */
  else if (!pathname && stat(USED_COMMAND_STRING, &statst) == 0)
    return USED_COMMAND_STRING;
  /* If no path provided, test local command */
  else if (!pathname && stat(LOCAL_COMMAND_STRING, &statst) == 0)
    return LOCAL_COMMAND_STRING;
  /* If no path provided, test Debug command */
  else if (!pathname && stat(DEBUG_COMMAND_STRING, &statst) == 0)
    return DEBUG_COMMAND_STRING;
  /* If no path provided, test RelWithDebInfo command */
  else if (!pathname && stat(RELWDEBINFO_COMMAND_STRING, &statst) == 0)
    return RELWDEBINFO_COMMAND_STRING;

  return (const char const*)NULL;
}

/*
 * popen_listechainee
 *
 * Opens a pipe for reading from launched command listechainee.
 * This function uses only standard input for reading from
 * the child process.
 * cf. open_dup_listechainee for reading from standard output and
 * standard error and also for writing to standard input.
 */
FILE *
popen_listechainee(char* args)
{
  /* Compute the size of the string to allocate for command */
  char *cmd = (char*)malloc(strlen(command_pathname) + strlen(args) + 2);

  /* Format the command  */
  sprintf(cmd, "%s %s", command_pathname, args);

  /* Open the pipe for launching the command with a shell: 'sh -c "<command>"' */
  /* The returned file pointer will allow us to read ("r") the command standard output/error  */
  FILE *fp = popen(cmd, "r");

  /* Free the command string allocated */
  free((void*)cmd);

  /* Return the file pointer */
  return fp;
}

/*
 * popen_dup_listechainee
 *
 * Opens a pipe for reading from standard output and error and for
 * writing to standard input of the child process.
 */
pid_t
popen_dup_listechainee(int* fd_in, int* fd_out, int* fd_err, int nargs, ...)
{
  va_list ap;			/* arg ptr: variable argument list */
  pid_t childpid;		/* childpid: child process ID */
  int n = nargs;		/* n: number of arguments for command */
  
  int use_input_fd = (fd_in != (int*)NULL);		/* If an input fd is provided use it */
  int use_output_fd = (fd_out != (int*)NULL);	/* If an output fd is provided use it */
  int use_error_fd = (fd_err != (int*)NULL);	/* If an error fd is provided use it */

  int fdin[2];			/* Input pipe */
  int fdout[2];			/* Output pipe */
  int fderr[2];			/* Error pipe */

  /* Allocate memory for storing command arguments */
  char** args = (char**)malloc(nargs * sizeof(char*));
  if (args == NULL)
    {
      perror("allocate memory for command arguments");
      return -1;
    }

  /* Get args for the launched command and store them in args table */
  va_start(ap, nargs);
  do
    args[nargs - n] = va_arg(ap, char *);
  while(--n);
  va_end(ap);

  /*
   * Create our three pipes: input, output and error
   */
  if (use_input_fd)
    if (pipe2(fdin, O_CLOEXEC|O_NONBLOCK|O_DIRECT) == -1) 	/* One for standard input */
      {
        perror("create input pipe");
        return -1;          
      }
  if (use_output_fd)
    if (pipe2(fdout, O_CLOEXEC|O_DIRECT) == -1)  		/* One for standard output */ 
      {
        perror("create output pipe");
        return -1;          
      }
  if (use_error_fd)
    if (pipe2(fderr, O_CLOEXEC|O_DIRECT) == -1)  /* One for standard error */
      {
        perror("create error pipe");
        return -1;          
      }
  
  /*
   * Fork our process for launching the command
   */
  if ((childpid = fork()) == -1)
    {
      perror("fork");
      return -1;
    }

  /* If we are in child process */
  if (childpid == 0)
    {
      /*
       * Setup our pipes ends as input/output for the child process
       */

      /* If we use input pipe */
      if (use_input_fd)
        {
          close(fdin[PIPE_WRITE_END]);  			/* close write end of input pipe */
          close(STDIN_FILENO);          		  	/* Close the standard input fd */
          if (dup(fdin[PIPE_READ_END]) == -1)     	/* And set as std input the read end fd from the input pipe */
            {
              perror("dup read end fd of input pipe");
              return -1;
            }
        }
      /* If we use output pipe */
      if (use_output_fd)
        {
          close(fdout[PIPE_READ_END]);  			/* close read end of output pipe */
          close(STDOUT_FILENO);                   	/* Close the standard output fd */
          if (dup(fdout[PIPE_WRITE_END]) == -1)   	/* And set as std out the write end fd from the output pipe */          
            {
              perror("dup write end fd of output pipe");
              return -1;
            }
        }
      /* If we use error pipe */
      if (use_error_fd)
        {
          close(fderr[PIPE_READ_END]);  			/* close read end of error pipe */
          close(STDERR_FILENO);               	    /* Finally close the standard error fd */
          if (dup(fderr[PIPE_WRITE_END]) == -1)   	/* And set as standard error the write end fd from the error pipe */          
            {
              perror("dup write end fd of error pipe");
              return -1;
            }
        }
      
      /* Exec the child command */
      if (execve(args[0], args, global_envp) == -1)
        {
          perror("execv child command");
          return -1;
        }
      
      /* Never reached */
      while(1);
    }
  /*
   * And in parent process (the childpid is provided hence positive integer)
   */
  else if (childpid > 0)
    {
      /*
       * We want to use the input pipe for the parent process to send
       * data to the child process (it read on std in)
       */
      if (use_input_fd)
        {
          /* Close unused read end of input pipe */
          close(fdin[PIPE_READ_END]);
          /* Copy as std input the in fd of the input pipe  */
          *fd_in = fdin[PIPE_WRITE_END];          
        }

      /*
       * We want to use the output pipe for the parent process to receive
       * data from the child process (it send on std out)
       */
      if (use_output_fd)
        {
          /* Close unused write end of output pipe */
          close(fdout[PIPE_WRITE_END]);
          /* Copy as std output the out fd of the out pipe */
          *fd_out = fdout[PIPE_READ_END];
        }

      /*
       * We want to use the error pipe for the parent process to receive
       * data from the child process (it send on std err)
       */
      if (use_error_fd)
        {
          /* Close unused write end of error pipe */
          close(fderr[PIPE_WRITE_END]);
          /* Copy as std error the out fd of the err pipe */
          *fd_err = fderr[PIPE_READ_END];          
        }
    }
  
  /* Returns our FDs */
  return childpid;
}

#define TEST_COMMAND_ARGS_VS_RESULT_NB(args, nb)                     \
  /* Initialize the buffer to \x00 */                                \
  bzero((void*)outbuf, BUFFER_MAX_SIZE);                             \
  /* Open the command with a pipe */                                 \
  FILE *fp = popen_listechainee(args);                               \
  /* Read the command output with the pipe */                        \
  int sz = fread(outbuf, BUFFER_MAX_SIZE-1, 1, fp);                  \
  /* Test assertion */                                               \
  CU_ASSERT(strncmp(outbuf,                                          \
                    results[nb],                                     \
                    strlen(results[nb])) == 0);                      \
  /* Close the pipe */                                               \
  pclose(fp);

#define TEST_COMMAND_ARGS_VS_IO_RES_NB_FULL(nbin, nbout, nberr, fdin, fdout, fderr, nbargs, ...)    \
  pid_t childpid;                   /* Child process PID returned from fork */                      \
  int wstatus;                      /* Status returned by the child process */                      \
  int fds[3];                       /* File descr table used for getting in/out from child */       \
  char* total_out = (char*)NULL;    /* Buffer for all output from child standard output */          \
  char* total_err = (char*)NULL;    /* Buffer for all output from child standard error */           \
  ssize_t out_sz, total_out_sz = 0; /* Size of data read from child output and total size */        \
  ssize_t err_sz, total_err_sz = 0; /* Size of data read from child error and total size */         \
  /* Logging file name for storing effective result strings */                                      \
  char effect_testname[MESSAGE_MAX_SIZE];                                                           \
  sprintf(effect_testname, "effective_%s.log", __PRETTY_FUNCTION__);                                \
  /* Logging file name for storing expected result strings */                                       \
  char expect_testname[MESSAGE_MAX_SIZE];                                                           \
  sprintf(expect_testname, "expected__%s.log", __PRETTY_FUNCTION__);                                \
                                                                                                    \
  /* Init IOs fds */                                                                                \
  fds[STDIN_FILENO] = fds[STDOUT_FILENO] = fds[STDERR_FILENO] = -1;                                 \
                                                                                                    \
  /* Open the command with a pipe */                                                                \
  childpid = popen_dup_listechainee(fdin, fdout, fderr, nbargs, __VA_ARGS__);                       \
                                                                                                    \
  /* Wait for child to terminate */                                                                 \
  (void)waitpid(childpid, &wstatus, WNOHANG);                                                       \
                                                                                                    \
  /* Open files with piped fds */                                                                   \
  FILE* fpin =  (FILE*)(fds[STDIN_FILENO]  == -1 ? NULL : fdopen(fds[STDIN_FILENO],  "w"));         \
  FILE* fpout = (FILE*)(fds[STDOUT_FILENO] == -1 ? NULL : fdopen(fds[STDOUT_FILENO], "r"));         \
  FILE* fperr = (FILE*)(fds[STDERR_FILENO] == -1 ? NULL : fdopen(fds[STDERR_FILENO], "r"));         \
                                                                                                    \
  /* Check if we need to inject some input to the child process */                                  \
  if (fpin && instrs[nbin] != (char*)NULL)                                                          \
    {                                                                                               \
      fwrite(instrs[nbin], strlen(instrs[nbin]), 1, fpin);                                          \
      /* flush the standard input of the child */                                                   \
      fflush(fpin);                                                                                 \
    }                                                                                               \
                                                                                                    \
  /* Read the command output with the pipe */                                                       \
  if (fpout)                                                                                        \
    do                                                                                              \
      {                                                                                             \
        /* Initialize the buffers to \x00 */                                                        \
        bzero((void*)outbuf, BUFFER_MAX_SIZE);                                                      \
                                                                                                    \
        /* Read child command standard output */                                                    \
        if ((out_sz = read(fileno(fpout), (void*)outbuf, BUFFER_MAX_SIZE-1)) == -1)                 \
          {                                                                                         \
            perror("Reading child process standard output");                                        \
            return;                                                                                 \
          }                                                                                         \
        /* Read an amount of bytes */                                                               \
        else if (out_sz > 0)                                                                        \
          {                                                                                         \
            /* If these are the first bytes read */                                                 \
            if (!total_out_sz)                                                                      \
              {                                                                                     \
                total_out = (char*)malloc(out_sz+1);                                                \
                total_out[out_sz] = 0;                                                              \
                memcpy(total_out, outbuf, out_sz);                                                  \
              }                                                                                     \
            /* Else accumulate the bytes */                                                         \
            else                                                                                    \
              {                                                                                     \
                total_out = (char*)realloc(total_out, total_out_sz+out_sz);                         \
                memcpy(total_out+total_out_sz, outbuf, out_sz);                                     \
              }                                                                                     \
            /* Compute the total size of read bytes */                                              \
            total_out_sz += out_sz;                                                                 \
          }                                                                                         \
      }                                                                                             \
    while(out_sz > 0);                                                                              \
                                                                                                    \
  /* Read the command error with the pipe */                                                        \
  if (fperr)                                                                                        \
    do                                                                                              \
      {                                                                                             \
        /* Initialize the buffers to \x00 */                                                        \
        bzero((void*)errbuf, BUFFER_MAX_SIZE);                                                      \
                                                                                                    \
        /* Read child command standard error */                                                     \
        if ((err_sz = read(fileno(fperr), (void*)errbuf, BUFFER_MAX_SIZE-1)) == -1)                 \
          {                                                                                         \
            perror("Reading child process standard error");                                         \
            return;                                                                                 \
          }                                                                                         \
        /* Read an amount of bytes */                                                               \
        else if (err_sz > 0)                                                                        \
          {                                                                                         \
            /* If these are the first bytes read */                                                 \
            if (!total_err_sz)                                                                      \
              {                                                                                     \
                total_err = (char*)malloc(err_sz+1);                                                \
                total_err[err_sz] = 0;                                                              \
                memcpy(total_err, errbuf, err_sz);                                                  \
              }                                                                                     \
            /* Else accumulate the bytes */                                                         \
            else                                                                                    \
              {                                                                                     \
                total_err = (char*)realloc(total_err, total_err_sz+err_sz);                         \
                memcpy(total_err+total_err_sz, errbuf, err_sz);                                     \
              }                                                                                     \
            /* Compute the total size of read bytes */                                              \
            total_err_sz += err_sz;                                                                 \
          }                                                                                         \
      }                                                                                             \
    while(err_sz > 0);                                                                              \
                                                                                                    \
  /* Test assertions */                                                                             \
  if (fpout)                                                                                        \
    {                                                                                               \
      if (strncmp(total_out, results[nbout], strlen(results[nbout])) != 0)                          \
        {                                                                                           \
          FILE* dbgtestfp = fopen(expect_testname, "w");                                            \
          fprintf(dbgtestfp, "%s", results[nbout]);                                                 \
          fclose(dbgtestfp);                                                                        \
          dbgtestfp = fopen(effect_testname, "w");                                                  \
          fprintf(dbgtestfp, "%s", total_out);                                                      \
          fclose(dbgtestfp);                                                                        \
        }                                                                                           \
      CU_ASSERT(strncmp(total_out, results[nbout], strlen(results[nbout])) == 0);                   \
      free((void*)total_out);                                                                       \
    }                                                                                               \
  if (fperr)                                                                                        \
    {                                                                                               \
      if (strncmp(total_err, errres[nberr], strlen(errres[nberr])) != 0)                            \
        {                                                                                           \
          FILE* dbgtestfp = fopen(expect_testname, "w");                                            \
          fprintf(dbgtestfp, "%s", errres[nberr]);                                                  \
          fclose(dbgtestfp);                                                                        \
          dbgtestfp = fopen(effect_testname, "w");                                                  \
          fprintf(dbgtestfp, "%s", total_err);                                                      \
          fclose(dbgtestfp);                                                                        \
        }                                                                                           \
      CU_ASSERT(strncmp(total_err, errres[nberr],  strlen(errres[nberr]))  == 0);                   \
      free((void*)total_err);                                                                       \
    }                                                                                               \
                                                                                                    \
  /* Close the pipes */                                                                             \
  if (fpin)  fclose(fpin);                                                                          \
  if (fpout) fclose(fpout);                                                                         \
  if (fperr) fclose(fperr);                                                                         \
                                                                                                    \
  /* Wait for child to terminate */                                                                 \
  (void)waitpid(childpid, &wstatus, 0);

/*
 * test_2_1_command_basic_invocation_0
 *
 * Basic tests for the command invocation without args
 */
void
test_2_1_command_basic_invocation_0(void)
{
  TEST_COMMAND_ARGS_VS_IO_RES_NB_FULL(-1, 0, 0,
                                      (int*)NULL, &fds[STDOUT_FILENO], &fds[STDERR_FILENO],
                                      2, command_pathname, (char*)NULL);
}

/*
 * test_2_2_command_basic_invocation_v
 *
 * Basic tests for the command invocation with '-v' arg
 */
void
test_2_2_command_basic_invocation_v(void)
{
  TEST_COMMAND_ARGS_VS_IO_RES_NB_FULL(-1, 1, 0,
                                      (int*)NULL, &fds[STDOUT_FILENO], &fds[STDERR_FILENO],
                                      3, command_pathname, "-v", (char*)NULL);
}

/*
 * test_2_3_command_basic_invocation_vh
 *
 * Basic tests for the command invocation with '-v -h' args
 */
void
test_2_3_command_basic_invocation_vh(void)
{
  TEST_COMMAND_ARGS_VS_IO_RES_NB_FULL(-1, 2, 0,
                                      (int*)NULL, &fds[STDOUT_FILENO], &fds[STDERR_FILENO],
                                      4, command_pathname, "-v", "-h", (char*)NULL);
}

/*
 * test_2_4_command_basic_invocation_verbose
 *
 * Basic tests for the command invocation with '--verbose' arg
 */
void
test_2_4_command_basic_invocation_verbose(void)
{
  TEST_COMMAND_ARGS_VS_IO_RES_NB_FULL(-1, 3, 0,
                                      (int*)NULL, &fds[STDOUT_FILENO], &fds[STDERR_FILENO],
                                      3, command_pathname, "--verbose", (char*)NULL);
}

/*
 * test_2_5_command_basic_invocation_verbose_help
 *
 * Basic tests for the command invocation with '--verbose --help' args
 */
void
test_2_5_command_basic_invocation_verbose_help(void)
{
  TEST_COMMAND_ARGS_VS_IO_RES_NB_FULL(-1, 4, 0,
                                      (int*)NULL, &fds[STDOUT_FILENO], &fds[STDERR_FILENO],
                                      4, command_pathname, "--verbose", "--help", (char*)NULL);
}

/*
 * test_2_6_command_basic_invocation_A1d
 *
 * Basic tests for the command invocation with '-A 1 -d' args
 */
void
test_2_6_command_basic_invocation_A1d(void)
{
  TEST_COMMAND_ARGS_VS_RESULT_NB("-A 1 -d", 5);
}

/*
 * test_2_7_command_basic_invocation_P2d
 *
 * Basic tests for the command invocation with '-P 2 -d' args
 */
void
test_2_7_command_basic_invocation_P2d(void)
{
  TEST_COMMAND_ARGS_VS_RESULT_NB("-P 2 -d", 6);
}

/*
 * test_2_8_command_basic_invocation_append_3_display
 *
 * Basic tests for the command invocation with '--append 3 --display' args
 */
void
test_2_8_command_basic_invocation_append_3_display(void)
{
  TEST_COMMAND_ARGS_VS_RESULT_NB("--append 3 --display", 7);  
}

/*
 * test_2_9_command_basic_invocation_prepend_4_display
 *
 * Basic tests for the command invocation with '--prepend 4 --display' args
 */
void
test_2_9_command_basic_invocation_prepend_4_display(void)
{
  TEST_COMMAND_ARGS_VS_RESULT_NB("--prepend 4 --display", 8);  
}

/*
 * test_2_10_command_basic_invocation_A1P2d
 *
 * Basic tests for the command invocation with '-A 1 -P 2 -d' args
 */
void
test_2_10_command_basic_invocation_A1P2d(void)
{
  TEST_COMMAND_ARGS_VS_RESULT_NB("-A 1 -P 2 -d", 9);  
}

/*
 * test_2_11_command_basic_invocation_append_1_prepend_2_display
 *
 * Basic tests for the command invocation with '--append 1 --prepend 2 --display' args
 */
void
test_2_11_command_basic_invocation_append_1_prepend_2_display(void)
{
  TEST_COMMAND_ARGS_VS_RESULT_NB("--append 1 --prepend 2 --display", 10);  
}

/*
 * test_2_12_command_basic_invocation_vA1d
 *
 * Basic tests for the command invocation with '-v -A 1 -d' args
 */
void
test_2_12_command_basic_invocation_vA1d(void)
{
  TEST_COMMAND_ARGS_VS_RESULT_NB("-v -A 1 -d", 11);
}

/*
 * test_2_13_command_basic_invocation_vP2d
 *
 * Basic tests for the command invocation with '-v -P 2 -d' args
 */
void
test_2_13_command_basic_invocation_vP2d(void)
{
  TEST_COMMAND_ARGS_VS_RESULT_NB("-v -P 2 -d", 12);
}

/*
 * test_2_14_command_basic_invocation_verbose_append_3_display
 *
 * Basic tests for the command invocation with '--verbose --append 3 --display' args
 */
void
test_2_14_command_basic_invocation_verbose_append_3_display(void)
{
  TEST_COMMAND_ARGS_VS_RESULT_NB("--verbose --append 3 --display", 13);  
}

/*
 * test_2_15_command_basic_invocation_verbose_prepend_4_display
 *
 * Basic tests for the command invocation with '--verbose --prepend 4 --display' args
 */
void
test_2_15_command_basic_invocation_verbose_prepend_4_display(void)
{
  TEST_COMMAND_ARGS_VS_RESULT_NB("--verbose --prepend 4 --display", 14);  
}

/*
 * test_2_16_command_basic_invocation_vA1P2d
 *
 * Basic tests for the command invocation with '-v -A 1 -P 2 -d' args
 */
void
test_2_16_command_basic_invocation_vA1P2d(void)
{
  TEST_COMMAND_ARGS_VS_RESULT_NB("-v -A 1 -P 2 -d", 15);  
}

/*
 * test_2_17_command_basic_invocation_verbose_append_1_prepend_2_display
 *
 * Basic tests for the command invocation with '--verbose --append 1 --prepend 2 --display' args
 */
void
test_2_17_command_basic_invocation_verbose_append_1_prepend_2_display(void)
{
  TEST_COMMAND_ARGS_VS_RESULT_NB("--verbose --append 1 --prepend 2 --display", 16);  
}

/*
 * test_2_18_command_basic_invocation_vA1P2ds_testlist1_l
 *
 * Basic tests for the command invocation with '-v -A 1 -P 2 -d -s testlist1.l' args
 */
void
test_2_18_command_basic_invocation_vA1P2ds_testlist1_l(void)
{
  TEST_COMMAND_ARGS_VS_RESULT_NB("-v -A 1 -P 2 -d -s testlist1.l", 17);
  /* Open the saved list file */ 
  FILE* datafp = fopen("testlist1.l", "r");
  /* Open failed */
  if (datafp == (FILE*)NULL)
    {
      /* Couldn't read the list file, format error msg  */
      char errmesg[ERROR_BUFFER_SIZE];
      snprintf(errmesg, ERROR_BUFFER_SIZE-1, "error: ");
      perror(errmesg);
      /* Test failed */
      CU_ASSERT(0 == 1);
    }
  else
    {
      /* Buffer for reading file content */
      char buflist[LIST_BUFFER_SIZE];
      /* Read list file content */
      size_t listlen = read(fileno(datafp), (void*)buflist, LIST_BUFFER_SIZE-1);
      /* Assert the list content */
      CU_ASSERT(strncmp("2,1\n", buflist, listlen) == 0);
    }
  return;
}

/*
 * test_2_19_command_basic_invocation_verbose_append_1_prepend_2_display_save_testlist1_l
 *
 * Basic tests for the command invocation with '--verbose --append 1 --prepend 2 --display --save testlist1.l' args
 */
void
test_2_19_command_basic_invocation_verbose_append_1_prepend_2_display_save_testlist1_l(void)
{
  TEST_COMMAND_ARGS_VS_RESULT_NB("--verbose --append 1 --prepend 2 --display --save testlist1.l", 18); 
  /* Open the saved list file */ 
  FILE* datafp = fopen("testlist1.l", "r");
  /* Open failed */
  if (datafp == (FILE*)NULL)
    {
      /* Couldn't read the list file, format error msg  */
      char errmesg[ERROR_BUFFER_SIZE];
      snprintf(errmesg, ERROR_BUFFER_SIZE-1, "error: ");
      perror(errmesg);
      /* Test failed */
      CU_ASSERT(0 == 1);
    }
  else
    {
      /* Buffer for reading file content */
      char buflist[LIST_BUFFER_SIZE];
      /* Read list file content */
      size_t listlen = read(fileno(datafp), (void*)buflist, LIST_BUFFER_SIZE-1);
      /* Assert the list content */
      CU_ASSERT(strncmp("2,1\n", buflist, listlen) == 0);
    }
  return;
}

/*
 * test_2_20_command_basic_invocation_vl_testlist1_l_d
 *
 * Basic tests for the command invocation with '-v -l testlist1.l -d' args
 */
void
test_2_20_command_basic_invocation_vl_testlist1_l_d(void)
{
  TEST_COMMAND_ARGS_VS_RESULT_NB("-v -l testlist1.l -d", 19); 
}

/*
 * test_2_21_command_basic_invocation_verbose_load_testlist1_l_display
 *
 * Basic tests for the command invocation with '--verbose --load testlist1.l --display' args
 */
void
test_2_21_command_basic_invocation_verbose_load_testlist1_l_display(void)
{
  TEST_COMMAND_ARGS_VS_RESULT_NB("--verbose --load testlist1.l --display", 20); 
}

/*
 * test_2_22_command_basic_invocation_vl_testlist1_notfound_l_d
 *
 * Basic tests for the command invocation with '-v -l testlist1-notfound.l -d' args
 */
void
test_2_22_command_basic_invocation_vl_testlist1_notfound_l_d(void)
{
  TEST_COMMAND_ARGS_VS_IO_RES_NB_FULL(-1, 21, 1,
                                      (int*)NULL, &fds[STDOUT_FILENO], &fds[STDERR_FILENO],
                                      6, command_pathname, "-v", "-l", "testlist1-notfound.l", "-d", (char*)NULL);
}

/*
 * test_2_23_command_basic_invocation_verbose_load_testlist1_notfound_l_display
 *
 * Basic tests for the command invocation with '--verbose --load testlist1-notfound.l --display' args
 */
void
test_2_23_command_basic_invocation_verbose_load_testlist1_notfound_l_display(void)
{
  TEST_COMMAND_ARGS_VS_IO_RES_NB_FULL(-1, 22, 2,
                                      (int*)NULL, &fds[STDOUT_FILENO], &fds[STDERR_FILENO],
                                      6, command_pathname, "--verbose", "--load", "testlist1-notfound.l", "--display", (char*)NULL);
}

/*
 * test_2_24_command_basic_invocation_A1A2A3dr1dP4dr2dr4dr3dr3d
 *
 * Basic tests for the command invocation with '-A 1 -A 2 -A 3 -d -r 1 -d -P 4 -d -r 2 -d -r 4 -d -r 3 -d -r 3 -d' args
 */
void
test_2_24_command_basic_invocation_A1A2A3dr1dP4dr2dr4dr3dr3d(void)
{
  TEST_COMMAND_ARGS_VS_RESULT_NB("-A 1 -A 2 -A 3 -d -r 1 -d -P 4 -d -r 2 -d -r 4 -d -r 3 -d -r 3 -d", 23); 
}

/*
 * test_2_25_command_basic_invocation_A1A2A3dI41d
 *
 * Basic tests for the command invocation with '-A 1 -A 2 -A 3 -d -I 4,1 -d' args
 */
void
test_2_25_command_basic_invocation_A1A2A3dI41d(void)
{
  TEST_COMMAND_ARGS_VS_RESULT_NB("-A 1 -A 2 -A 3 -d -I4,1 -d", 24); 
}

/*
 * test_2_26_command_basic_invocation_A1A2A3dI42d
 *
 * Basic tests for the command invocation with '-A 1 -A 2 -A 3 -d -I 4,2 -d' args
 */
void
test_2_26_command_basic_invocation_A1A2A3dI42d(void)
{
  TEST_COMMAND_ARGS_VS_RESULT_NB("-A 1 -A 2 -A 3 -d -I4,2 -d", 25); 
}

/*
 * test_2_27_command_basic_invocation_A1A2A3dI43d
 *
 * Basic tests for the command invocation with '-A 1 -A 2 -A 3 -d -I 4,3 -d' args
 */
void
test_2_27_command_basic_invocation_A1A2A3dI43d(void)
{
  TEST_COMMAND_ARGS_VS_RESULT_NB("-A 1 -A 2 -A 3 -d -I4,3 -d", 26); 
}

/*
 * test_2_28_command_basic_invocation_A1A2A3dI44d
 *
 * Basic tests for the command invocation with '-A 1 -A 2 -A 3 -d -I 4,4 -d' args
 */
void
test_2_28_command_basic_invocation_A1A2A3dI44d(void)
{
  TEST_COMMAND_ARGS_VS_RESULT_NB("-A 1 -A 2 -A 3 -d -I4,4 -d", 27); 
}

/*
 * test_2_29_command_basic_invocation_A1A2A3dI12d
 *
 * Basic tests for the command invocation with '-A 1 -A 2 -A 3 -d -I 1,2 -d' args
 */
void
test_2_29_command_basic_invocation_A1A2A3dI12d(void)
{
  TEST_COMMAND_ARGS_VS_RESULT_NB("-A 1 -A 2 -A 3 -d -I1,2 -d", 28); 
}

/*
 * test_2_30_command_basic_invocation_A1A2A3dI4plus0d
 *
 * Basic tests for the command invocation with '-A 1 -A 2 -A 3 -d -I 4,+0 -d' args
 */
void
test_2_30_command_basic_invocation_A1A2A3dI4plus0d(void)
{
  TEST_COMMAND_ARGS_VS_RESULT_NB("-A 1 -A 2 -A 3 -d -I4,+0 -d", 29); 
}

/*
 * test_2_31_command_basic_invocation_A1A2A3dI4plus1d
 *
 * Basic tests for the command invocation with '-A 1 -A 2 -A 3 -d -I 4,+1 -d' args
 */
void
test_2_31_command_basic_invocation_A1A2A3dI4plus1d(void)
{
  TEST_COMMAND_ARGS_VS_RESULT_NB("-A 1 -A 2 -A 3 -d -I4,+1 -d", 30); 
}

/*
 * test_2_32_command_basic_invocation_A1A2A3dI4plus2d
 *
 * Basic tests for the command invocation with '-A 1 -A 2 -A 3 -d -I 4,+2 -d' args
 */
void
test_2_32_command_basic_invocation_A1A2A3dI4plus2d(void)
{
  TEST_COMMAND_ARGS_VS_RESULT_NB("-A 1 -A 2 -A 3 -d -I4,+2 -d", 31); 
}

/*
 * test_2_33_command_basic_invocation_A1A2A3dI4plus3d
 *
 * Basic tests for the command invocation with '-A 1 -A 2 -A 3 -d -I 4,+3 -d' args
 */
void
test_2_33_command_basic_invocation_A1A2A3dI4plus3d(void)
{
  TEST_COMMAND_ARGS_VS_RESULT_NB("-A 1 -A 2 -A 3 -d -I4,+3 -d", 32); 
}
/*
 * test_2_34_command_basic_invocation_A1A2A3dI1plus0d
 *
 * Basic tests for the command invocation with '-A 1 -A 2 -A 3 -d -I 1,+0 -d' args
 */
void
test_2_34_command_basic_invocation_A1A2A3dI1plus0d(void)
{
  TEST_COMMAND_ARGS_VS_RESULT_NB("-A 1 -A 2 -A 3 -d -I1,+0 -d", 33); 
}

/*
 * test_2_35_command_basic_invocation_l_testlist1_l_dt1t2t3
 *
 * Basic tests for the command invocation with '-l testlist1.l -d -t 1 -t 2 -t 3' args
 */
void
test_2_35_command_basic_invocation_l_testlist1_l_dt1t2t3(void)
{
  TEST_COMMAND_ARGS_VS_RESULT_NB("-l testlist1.l -d -t 1 -t 2 -t 3", 34); 
}

/*
 * test_2_36_command_basic_invocation_vl_testlist1_l_dt1t2t3
 *
 * Basic tests for the command invocation with '-v -l testlist1.l -d -t 1 -t 2 -t 3' args
 */
void
test_2_36_command_basic_invocation_vl_testlist1_l_dt1t2t3(void)
{
  TEST_COMMAND_ARGS_VS_RESULT_NB("-v -l testlist1.l -d -t 1 -t 2 -t 3", 35); 
}

/*
 * test_2_37_command_basic_invocation_l_testlist1_l_A1dr2d
 *
 * Basic tests for the command invocation with '-l testlist1.l -A 3 -d -r 2 -d' args
 */
void
test_2_37_command_basic_invocation_l_testlist1_l_A1dr2d(void)
{
  TEST_COMMAND_ARGS_VS_RESULT_NB("-l testlist1.l -A 3 -d -r 2 -d", 36);
}

/*
 * test_2_38_command_basic_invocation_l_testlist1_l_A1dr1d
 *
 * Basic tests for the command invocation with '-l testlist1.l -A 3 -d -r 1 -d' args
 */
void
test_2_38_command_basic_invocation_l_testlist1_l_A1dr1d(void)
{
  TEST_COMMAND_ARGS_VS_RESULT_NB("-l testlist1.l -A 3 -d -r 1 -d", 37);
}

/*
 * test_2_39_command_basic_invocation_l_testlist1_l_A1dr3d
 *
 * Basic tests for the command invocation with '-l testlist1.l -A 3 -d -r 3 -d' args
 */
void
test_2_39_command_basic_invocation_l_testlist1_l_A1dr3d(void)
{
  TEST_COMMAND_ARGS_VS_RESULT_NB("-l testlist1.l -A 3 -d -r 3 -d", 38);
}

/*
 * test_2_40_command_basic_invocation_l_testlist1_l_A1dr4d
 *
 * Basic tests for the command invocation with '-l testlist1.l -A 3 -d -r 4 -d' args
 */
void
test_2_40_command_basic_invocation_l_testlist1_l_A1dr4d(void)
{
  TEST_COMMAND_ARGS_VS_RESULT_NB("-l testlist1.l -A 3 -d -r 4 -d", 39);
}

/*
 * test_2_41_command_menu_invocation_append_display
 *
 * Basic tests for the command menu invocation with '-N' args
 * then send 'f \n 1 \n f \n 2 \n a \n q \n' on standard input
 */
void
test_2_41_command_menu_invocation_append_display(void)
{
  TEST_COMMAND_ARGS_VS_IO_RES_NB_FULL(0, 40, -1,
                                      &fds[STDIN_FILENO], &fds[STDOUT_FILENO], (int*)NULL,
                                      3, command_pathname, "-N", (char*)NULL);
}

/*
 * test_2_42_command_menu_invocation_help_quit
 *
 * Basic tests for the command menu invocation with '-N' args
 * then send 'm \n q \n' on standard input
 */
void
test_2_42_command_menu_invocation_help_quit(void)
{
  TEST_COMMAND_ARGS_VS_IO_RES_NB_FULL(1, 41, -1,
                                      &fds[STDIN_FILENO], &fds[STDOUT_FILENO], (int*)NULL,
                                      3, command_pathname, "-N", (char*)NULL);
}

/*
 * test_2_43_command_menu_invocation_load_insert_x_2_display_quit
 *
 * Basic tests for the command menu invocation with '-N' args
 * then send 'c \n testmenu.l \n a \n h \n 4 \n 1 \n a \n h \n 5 \n 3 \n a \n q \n' on standard input
 */
void
test_2_43_command_menu_invocation_load_insert_x_2_display_quit(void)
{
  TEST_COMMAND_ARGS_VS_IO_RES_NB_FULL(1, 41, -1,
                                      &fds[STDIN_FILENO], &fds[STDOUT_FILENO], (int*)NULL,
                                      3, command_pathname, "-N", (char*)NULL);
}

/**
 * vim: et:ts=4:sw=4:sts=4																																				   
 * -*- mode: C; coding: utf-8-unix; tab-width: 4; tab-always-indent: t; tab-first-completion: nil -*-
 */
