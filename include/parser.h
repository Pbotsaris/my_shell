/*
 * =====================================================================================
 *
 *       Filename:  parser.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/19/21 09:26:06
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef PARSER_H
#define PARSER_H

#include "../include/lexer.h"

typedef struct node
{
  type_t type;
  char *value;
  struct node *left;
  struct node *right;

}node_t;

typedef struct parser
{
  bool valid;
  token_t *lookahead;
  node_t *(*parse) (struct parser*, lexer_t*);

}parser_t;

parser_t *init_parser(void);

#endif
