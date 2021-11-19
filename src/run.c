/*
 * =====================================================================================
 *
 *       Filename:  run.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/13/21 22:07:50
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "../include/run.h"

void main_loop(char **envs)
{

  prgm_t *program = init_program(envs);

  while (program->cmd->not_exit(program->cmd))
  {
    program->print_prompt(program);
    program->readline(program);

  }

  program->free(program);

}


