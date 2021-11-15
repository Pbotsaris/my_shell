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
static void print_prompt(prgm_t *program);
static void read_line(prgm_t *program);

prgm_t *init_program(char **envs)
{
  prgm_t *program           = (prgm_t*)malloc(sizeof(prgm_t));
  program->cmd              = init_command();
  program->env              = init_env();
  program->free             = free_program;
  program->print_prompt     = print_prompt;
  program->readline         = read_line;

  program->env->load(program->env, envs);

  return program;

}

/**/

static void free_program(prgm_t *program)
{
  program->env->free(program->env);

  free(program->cmd->line);
  free(program->cmd);
  free(program);

}

/**/

static void read_line(prgm_t *program)
{
  char *temp                = program->cmd->line;
  program->cmd->line        = readline("");
  program->cmd->len         = strlen(program->cmd->line);

  add_history(program->cmd->line);
  free(temp);
}

/**/

static void print_prompt(prgm_t *program) 
{
  /* update pwd at every command in case of cd */
  entry_t *pwd          = program->env->map->get(program->env->map, PWD);
  program->env->pwd     = pwd->pair;

  printf("%s %s %s %s", program->env->user, PSEP, program->env->pwd, PSUFIX);

}

