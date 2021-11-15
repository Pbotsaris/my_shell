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

//static void print_envs(prgm_t *program);

void main_loop(char **envs)
{

  prgm_t *program = init_program(envs);
 
 //  print_envs(program);
  while (program->cmd->not_exit(program->cmd))
  {
    program->print_prompt(program);
    program->readline(program);
  }

  entry_t *e = program->env->map->get(program->env->map, "PATH");

  printf("%s=%s\n", e->key, e->pair);



  program->free(program);

}


