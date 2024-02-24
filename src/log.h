/*
 * log.h
 *
 * Header des fonctions de logging
 */

#pragma once

// Standard includes
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <strings.h>
#include <errno.h>

extern int errno;
extern int sys_nerr;
extern const char * const sys_errlist[];

// Maximum logging format string/msg size
#define MAX_LOGFMT_LENGTH  4096
#define MAX_LOG_MSG_LENGTH 1024

// Logging macro
#ifdef    DEBUG
#define LOG(loglvl, logfmt, ...)	{if (loglvl >= log_lvl) do_log(logfp, loglvl, logfmt, ...);}
#else  /* !DEBUG */
#define LOG(logfmt, ...)	/* Do nothing ... */
#endif /* !DEBUG */

/* Enum for logging levels */
enum loglvl_e {
  LOG_LVL_NONE = 0,
  LOG_LVL_INFO,
  LOG_LVL_WARN,
  LOG_LVL_ERR,
  LOG_LVL_DBG
};
typedef enum loglvl_e loglvl_t;

/* File pointer for log file */
extern FILE* logfp;

/* Level of logging */
extern loglvl_t loglvl;

/*
 * Forward decl for logging functions
 */
FILE* init_log(char* /*log_filepath*/, loglvl_t /*log_level*/);
int do_log(FILE* /*log_fp*/, loglvl_t /*log_level*/, char* /*log_format*/, ... /*args*/);
void end_log(FILE* /*log_fp*/);
