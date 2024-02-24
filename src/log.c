/*
 * log.c
 *
 * Implementation des fonctions de logging
 */

#include <stdlib.h>

#include "src/log.h"

/* File pointer for log file */
FILE* logfp = (FILE*)NULL;
static FILE* local_log_fp = (FILE*)NULL;

/* Level of logging */
loglvl_t loglvl = LOG_LVL_INFO;

/*
 * init_log
 *
 * Initialize the logging in a provided file path. The sdefault logging level is also
 * provided as argument.
 *
 */
FILE*
init_log(char* log_filepath, loglvl_t log_level)
{
  if (log_filepath == (char*)NULL)
    {
      fprintf(stderr, "Error: cannot initialize logging without a file pathname\n");
      return (FILE*)NULL;
    }

  local_log_fp = fopen(log_filepath, "w");
  if (local_log_fp == (FILE*)NULL)
    {
      const char* logfmt = "cannot open logging file '%s'";
      char logmsg[MAX_LOG_MSG_LENGTH];
      snprintf(logmsg, MAX_LOG_MSG_LENGTH, logfmt, log_filepath);
      perror(logmsg);
    }

  // Return the logging file pointer
  return local_log_fp;
}

/*
 * do_log
 *
 * Implementation of the logging function. It takes an unknown number of arguments and
 * log according to the format string provided and the level expected.
 * It returns the number of arguments printed in the provided format.
 */
int
do_log(FILE* logfp, loglvl_t loglvl, char* logfmt, ...)
{
  unsigned int n_printed_args = 0;
  va_list ap;
  char* my_logfmt = strndup(logfmt, strnlen(logfmt, MAX_LOGFMT_LENGTH));
  char* logptr = strtok(my_logfmt, "%");

  // If a '%' was found in the logfmt
  if (logptr)
    {
      // Continue parsing logfmt with current sep
      while(logptr = strtok((char*)NULL, "! \"'|()[],:;\n\t"))
        {
          // According to the '%' char requested
          switch (logptr[strlen(logptr)-1])
            {
              // Requesting an integer
            case 'd':
              {
                // Requested field is a long
                int is_long = 0;
                // If modifiers are provided
                if (strlen(logptr) > 1)
                  // If an 'l' is provided in the request string
                  if (logptr[0] == 'l')
                    // A long is requested
                    is_long = 1;
                // Increment number of processed fields
                n_printed_args++;
              }
              break;
              
              // Requesting an unsigned
            case 'u':
              {
                // Requested field is a long
                int is_long = 0;
                // If modifiers are provided
                if (strlen(logptr) > 1)
                  // If an 'l' is provided in the request string
                  if (logptr[0] == 'l')
                    // A long is requested
                    is_long = 1;
                // Increment number of processed fields
                n_printed_args++;
              }
              break;
              
              // Requesting a float
            case 'f': 
            case 'F':
              // Increment number of processed fields
              n_printed_args++;
              break;
              
              // Requesting a double
            case 'g': 
            case 'G': 
              // Increment number of processed fields
              n_printed_args++;
              break;
              
              // Requesting a string (char*)
            case 's':
              {
                // Requested string length (by default, no limit)
                int len = -1;
                // If a length is provided
                if (strlen(logptr) > 1)
                  {
                    // Read the string length
                    len = atoi(logptr);
                  }
                // Increment number of processed fields
                n_printed_args++;
              }
              break;
              
              // Requesting a pointer
            case 'p':
              // Increment number of processed fields
              n_printed_args++;
              break;

              // We do not know the requested field
            default:
              
            }
        }
    }

  // Initialize the argument pointer list
  va_start(ap, logfmt);

  // Call printf with the argument pointer list
  vfprintf(logfp, my_logfmt, ap);

  // Return the number of fields processed
  return(n_printed_args);
}

/*
 * end_log
 *
 * Terminate logging in the file, hence close it
 * Returns nothing.
 */
void
end_log(FILE* log_fp)
{
  /*
   * If provided param is NULL or is local_log_fp, use
   * the local log_fp and zero it
   */
  if (log_fp == (FILE*)NULL || log_fp == local_log_fp)
    {
      // Transfer local_log_fp in log_fp
      log_fp = local_log_fp;
      // Zero local_log_fp
      local_log_fp = (FILE*)NULL;
    }

  /*
   * If log_fp not null
   */
  if (log_fp != (FILE*)NULL)
    {
      // Close the associated file pointer
      fclose(log_fp);
      // Zero it
      log_fp = (FILE*)NULL;
    }
}
