/*
 * =====================================================================================
 *
 *       Filename:  run.c
 *
 *    Description:  main program entrypoint
 *
 *        Version:  1.0
 *        Created:  11/13/21 22:07:50
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pedro Botsaris
 *   Organization:  
 *
 * =====================================================================================
 */

#include "../include/run.h"

void main_loop(char **envs)
{

  prgm_t *program = init_program(envs);

  while (!program->is_exit)
  {
    program->print_prompt(program);

    /* reads line and executes */
    program->readline(program);
  }

    program->free(program);
}

