/*
 * =====================================================================================
 *
 *       Filename:  program.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/14/21 12:54:27
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */



#ifndef PROGRAM_H
#define PROGRAM_H

#define PSUFIX "%>"
#define PSEP  "-"

#include <editline/readline.h>
//#include "../include/command.h"
#include "../include/env.h"
#include "../include/hash.h"
#include "../include/parser.h"


typedef struct cmd{

 char *line;
 ssize_t len;

}cmd_t;


typedef struct program
{
  env_t    *env;
  lexer_t  *lexer;
  parser_t *parser;
  cmd_t    *cmd;
  node_t   *ast;
  bool     is_exit;
  void     (*print_prompt) (struct program*);
  void     (*free) (struct program*);
  void     (*readline) (struct program*);

}prgm_t;


prgm_t *init_program(char **envs);

#endif
