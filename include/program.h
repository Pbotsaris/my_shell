/*
 * =====================================================================================
 *
 *       Filename:  program.h
 *
 *    Description:  The program module holds all the functionality and data for
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

#include <editline/readline.h>
#include "../include/env.h"
#include "../include/hash.h"
#include "../include/parser.h"
#include "../include/path.h"
#include "../include/exec.h"

/*
 *          change PSUFIX and PSEP to customize the prompt
 */

#define PSUFIX "%>"
#define PSEP  "-"

/*
 *           the cmd struct stores temporarely the
 *           readline input as well as its length
 */

typedef struct cmd{

  char *line;
  ssize_t len;

}cmd_t;

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
  node_t   *ast;                                             /* the ast root produced by the parser module */
  cmd_t    *cmd;                                             /* cmd struct. see above */
  exec_t   *exec;                                            /* exec struct. see above */
  bool     is_exit;                                          /* tracks if whether the user issued a exit command  */
  void     (*print_prompt) (struct program*);                /* prints a new prompt whenever a new line is being read  */
  void     (*free) (struct program*);                        /* frees all memory related to this program */
  void     (*readline) (struct program*);                    /* read and executes a line */

}prgm_t;


/* builtins in builtins.c */
void echo(prgm_t *program);
void cd(prgm_t *program);
void env(prgm_t *program);
void exit_program(prgm_t *program);
void set_env(prgm_t *program);
void unset_env(prgm_t *program);
void which(prgm_t *program);

/* initialization */
prgm_t *init_program(char **envs);

#endif
