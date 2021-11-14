/*
 * =====================================================================================
 *
 *       Filename:  program.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/14/21 12:54:16
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "../include/program.h"

static void free_program(prgm_t *program);

prgm_t *init_program(char **envs)
{
  prgm_t *program    = (prgm_t*)malloc(sizeof(prgm_t));
  program->cmd       = init_command();
  program->env       = init_env();
  program->free      = free_program;

  /* load up envs */
  program->env->read_envs(program->env, envs);

  return program;

}

static void free_program(prgm_t *program)
{

  program->env->free(program->env);

  free(program->cmd->input);
  free(program->cmd);
  free(program);

}
