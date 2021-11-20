/*
 * =====================================================================================
 *
 *       Filename:  ast.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/20/21 13:51:28
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "../include/ast.h"


ast_t *init_ast()
{
  ast_t *ast       = (ast_t*)malloc(sizeof(ast));
  ast->root        = NULL;
  return ast;

}


void evaluate(ast_t *ast)
{

  if(ast->root->type == PASS_THROUGH)
  {
    printf("fork and execute!\n");
    return;
  }

  if(ast->root->type == VARIABLE_ASSIGN)
  {
    printf("fork and execute!\n");
  }



}
