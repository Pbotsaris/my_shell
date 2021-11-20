/*
 * =====================================================================================
 *
 *       Filename:  ast.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/20/21 13:49:32
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "../include/parser.h"

#ifndef AST_H
#define AST_H
typedef struct ast
{
  node_t *root;

}ast_t;


ast_t *init_ast(void);

#endif
