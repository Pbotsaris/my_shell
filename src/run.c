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

static bool is_not_exit(cmd_t *command);
static void prompt(void);

void main_loop(char **envs)
{

  prgm_t *program = init_program(envs);

  while (is_not_exit(program->cmd))
  {
    prompt();
    program->cmd->len = getline(&program->cmd->input, &program->cmd->num_bytes, stdin);
  }

  program->free(program);

 }

static bool is_not_exit(cmd_t *command) {
  if(command->input == NULL)
    return true;

  if(command->len == -1)
    return false;

  return !(strncmp(command->input, EXIT, command->len + 1) == 0);
}

static void prompt(void) {printf("%%> ");}

