/*
 * =====================================================================================
 *
 *       Filename:  program.h
 *
 *    Description:  The program modules holds all the functionality and data for
 *                  this program.
 *
 *        Version:  1.0
 *        Created:  11/14/21 12:54:27
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pedro Botsaris
 *   Organization:  
 *
 * =====================================================================================
 */



#ifndef PROGRAM_H
#define PROGRAM_H


/*
 *          change PSUFIX and PSEP to customize the prompt
 */

#define PSUFIX "%>"
#define PSEP  "-"

#include <editline/readline.h>
#include "../include/env.h"
#include "../include/hash.h"
#include "../include/parser.h"
#include "../include/path.h"

#define IGNORE_SINGLE  "i"
#define IGNORE_DOUBLE  "ignore-environment"
#define UNSET_SINGLE   "u"
#define UNSET_DOUBLE   "unset"
#define NILL_SINGLE    "0"
#define NILL_DOUBLE    "null"
#define CDIR_SINGLE    "C"
#define CDIR_DOUBLE    "chdir"
#define MAX_ARGV_LEN   100

typedef enum envflags{
  INIT,                /* 0 */
  IGNORE,              /* 1 */
  UNSET,               /* 2 */
  NILL,                /* 3 */
  CDIR,                /* 4 */
  ERR,                 /* 5 */
}envflag_t;

/*
 *           the cmd struct stores temporarely the
 *           readline input as well as its length
 */

typedef struct cmd{

  char *line;
  ssize_t len;

}cmd_t;


/* 
 *           the exec struct stores the necessary paramenters
 *           to execute an external binary with execve 
 */

typedef struct exec {
  node_t *root;                                                /* root of the ast branch to extract command to be executed by execve */
  char *bin;                                                   /*  bin to execute */
  char *argv[MAX_ARGV_LEN];                                    /*  arguments for execve. MAX of 99 arguments */
  char **envp;                                                 /*  envirioment for execve */


}exec_t;


/*
 *           the program struct holds all the data and functionality 
 *           necessary to run this program.
 *       
 */

typedef struct program
{
  env_t    *env;                                             /* the envirioment module. see env.c & env.h */
  lexer_t  *lexer;                                           /* the lexer module. see lexer.c & lexer.h */
  parser_t *parser;                                          /* the parser module. see parser.c & parser.h  */
  node_t   *ast;                                             /* the ast root outputed by the parser module */
  cmd_t    *cmd;                                             /* cmd struct. see above */
  exec_t   *exec;                                            /* exec struct. see above */
  bool     is_exit;                                          /* tracks if whether the user issued a exit command  */
  void     (*print_prompt) (struct program*);
  void     (*free) (struct program*);
  void     (*readline) (struct program*);

}prgm_t;


prgm_t *init_program(char **envs);

#endif
