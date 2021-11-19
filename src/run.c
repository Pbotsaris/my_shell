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

    token_t *token = program->lexer->get_next_token(program->lexer);

    if(token != NULL && token->type == EXIT)
      printf("was exit!\n");

    if(token != NULL && token->type == ECHO)
    {
      printf("was echo!\n");
      free(token);

      token = program->lexer->get_next_token(program->lexer);
      printf("type: %d, value: %s\n", token->type, token->value);
      free(token->value);
      free(token);


      token = program->lexer->get_next_token(program->lexer);
      printf("type: %d, value: %s\n", token->type, token->value);
      free(token);

      token = program->lexer->get_next_token(program->lexer);
      printf("type: %d, value: %s\n", token->type, token->value);
      free(token->value);
      free(token);

      token = program->lexer->get_next_token(program->lexer);
      printf("type: %d, value: %s\n", token->type, token->value);
    }

    if(token != NULL && token->type == ENV)
      printf("was ENV!\n");

    if(token != NULL && token->type == SETENV)
      printf("was setenv!\n");

    if(token != NULL && token->type == UNSETENV)
      printf("was unsetenv!\n");

    if(token != NULL && token->type == WHICH)
      printf("was was which!\n");

    if(token != NULL && token->type == PWD)
      printf("was was pwd!\n");

    if(token != NULL && token->type == VARIABLE_ASSIGN)
    {
      printf("was VAR ASSIGN -> %s!\n", token->value);
      free(token->value);
      free(token);

      token = program->lexer->get_next_token(program->lexer);

      printf("type: %d, value: %s\n", token->type, token->value);
      free(token);

      token = program->lexer->get_next_token(program->lexer);
      printf("type: %d, value: %s\n", token->type, token->value);
      free(token->value);

    }

    if(token != NULL && token->type == PASS_THROUGH)
    {
      printf("was WAS PASS THROUGH:!\n");
      free(token->value);
    }




    if(token)
      free(token);

  }

  program->free(program);

}


