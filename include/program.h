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
#include "../include/command.h"
#include "../include/env.h"
#include "../include/hash.h"


typedef struct program
{
  env_t *env;
  cmd_t *cmd;
  void (*print_prompt) (struct program*);
  void (*free) (struct program*);
  void (*readline) (struct program*);

}prgm_t;


prgm_t *init_program(char **envs);

#endif
