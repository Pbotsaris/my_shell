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

  while (!program->is_exit)
  {
    program->print_prompt(program);
    program->readline(program);
  }

    program->free(program);
}





//   token_t *token = program->lexer->get_next_token(program->lexer);
//    printf("%d, %s\n", token->type, token->value); 
//    if(token->value)
//        free(token->value);
//
//    free(token);
//
//    token = program->lexer->get_next_token(program->lexer);
//    printf("%d, %s\n", token->type, token->value); 
//    if(token->value)
//        free(token->value);
//
//    free(token);
//  
//    token = program->lexer->get_next_token(program->lexer);
//    if(token == NULL)
//      printf("token was null!");
// 
