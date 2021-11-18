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

    entry_t *e = program->env->bin->get(program->env->bin, "ls");

    if(e != NULL)
     {
      printf("ls was found. printing -> %s\n", e->pair);
     }

    token_t *token = program->lexer->get_next_token(program->lexer);

    if(token != NULL && token->type == EXIT)
        printf("was exit!\n");

    if(token != NULL && token->type == ECHO)
        printf("was echo!\n");

    if(token != NULL && token->type == ENV)
        printf("was ENV!\n");

    if(token != NULL && token->type == VARIABLE)
    {
        printf("was VAR!\n");
        free(token->value);
    }

    if(token != NULL && token->type == PASS_THROUGH)
    {
        printf("was WAS PASS THROUGH: %s!\n", token->value);
        free(token->value);
    }

    free(token);

  }

  program->free(program);

}


