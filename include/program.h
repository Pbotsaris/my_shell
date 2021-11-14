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

#include "../include/command.h"
#include "../include/env.h"

typedef struct program
{
  env_t *env;
  cmd_t *cmd;
  void (*free) (struct program*);

}prgm_t;


prgm_t *init_program(char **envs);

#endif
