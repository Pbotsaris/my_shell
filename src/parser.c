/*
 * =====================================================================================
 *
 *       Filename:  parser.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/19/21 09:25:51
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "../include/parser.h"

static node_t *parse(parser_t *parser, lexer_t *lexer);
static node_t *command(parser_t *parser, lexer_t *lexer);
static node_t *builtins(parser_t *parser, lexer_t *lexer);
static node_t *variable_assign(parser_t *parser, lexer_t *lexer);
static node_t *arguments(parser_t *parse, lexer_t *lexer);
static node_t *literal(parser_t *parser, lexer_t *lexer);
static node_t *options(parser_t *parser, lexer_t *lexer);

static node_t *eat(parser_t *parser, lexer_t *lexer, type_t type);
static node_t *create_node(token_t *token);
static void cpy_value(node_t *node, char *value);
static bool is_argument(type_t type);

parser_t *init_parser()
{
  parser_t *parser        = (parser_t*)malloc(sizeof(parser_t));
  parser->lookahead       = NULL;
  parser->parse           = parse;

  return parser;

}

static node_t *parse(parser_t *parser, lexer_t *lexer)
{

  parser->lookahead = lexer->get_next_token(lexer);

  if(!parser->lookahead)
  {
    parser->valid = false;
    return NULL;
  }

  return command(parser, lexer);

}

static node_t *command(parser_t *parser, lexer_t *lexer)
{
  if(parser->lookahead->type == PASS_THROUGH)
    return eat(parser, lexer, PASS_THROUGH);

  if(parser->lookahead->type == VARIABLE_ASSIGN)
    return variable_assign(parser, lexer);

  else return builtins(parser, lexer);

}

static node_t *builtins(parser_t *parser, lexer_t *lexer)
{
  node_t *root = eat(parser, lexer, parser->lookahead->type);
  /* not using right leaf for commands as they are not binary operations */
  if(parser->lookahead != NULL)
      root->left = arguments(parser, lexer);

  return  root;
}

static node_t *variable_assign(parser_t *parser, lexer_t *lexer)
{
  node_t *left = eat(parser, lexer, VARIABLE_ASSIGN);
  node_t *root = eat(parser, lexer, ASSIGN_OPERATOR);
  node_t *right = literal(parser, lexer);

  root->left = left;
  root->right = right;

  return root;

}

static node_t *arguments(parser_t *parser, lexer_t *lexer)
{
  node_t *left = options(parser, lexer);

  while(parser->lookahead && is_argument(parser->lookahead->type))
  {

    /* always to the left */
    node_t *arg = options(parser, lexer);
    arg->left = left;
    left = arg;
  }

  return left;
}


static node_t *options(parser_t *parser, lexer_t *lexer)
{

  if(parser->lookahead->type == ARGUMENT)
    return eat(parser, lexer, ARGUMENT);

  if(parser->lookahead->type == FLAG)
    return eat(parser, lexer, FLAG);

  if(parser->lookahead->type == DOUBLE_FLAG)
    return eat(parser, lexer, DOUBLE_FLAG);

  if(parser->lookahead->type == QUOTE)
    return literal(parser, lexer);

  return NULL;
}


static node_t *literal(parser_t *parser, lexer_t *lexer)
{

  if(parser->lookahead && parser->lookahead->type == LITERAL)
    return eat(parser, lexer, LITERAL);

  eat(parser, lexer, QUOTE);
  node_t *literal = eat(parser, lexer, LITERAL);
  eat(parser, lexer, QUOTE);

  return literal;
}


static node_t *eat(parser_t *parser, lexer_t *lexer, type_t type)
{
  token_t *token = parser->lookahead;

  /* unexpected end of input */
  if(!token)
  {
    parser->valid = false;
    printf("unexpected end of input\n");
    return NULL;
  }

  /* unexpected token */
  if(token->type != type)
  {
    printf("unexpected token: was: %d, should be: %d \n", token->type, type);
    parser->valid = false;
    return NULL;
  }

  parser->lookahead = lexer->get_next_token(lexer);

  /* do not return a tree node for quote */
  if(type == QUOTE)
  {
    free(token);
    return NULL;
  }

  return create_node(token);
}


static node_t *create_node(token_t *token)
{  

  node_t *node        = (node_t *)malloc(sizeof(node_t));
  node->left          = NULL;
  node->right         = NULL;
  node->type          = token->type;

  if(token->value)
  {
    cpy_value(node, token->value);
    free(token->value);
  }
  else
    node->value = NULL;

  free(token);
  return node;
}


static void cpy_value(node_t *node, char *value)
{
  size_t len          = strlen(value);
  node->value         = (char*)malloc((len + 1) * sizeof(char));

  strncpy(node->value, value, len);
  node->value[len]    = '\0';

}


static bool is_argument(type_t type)
{
  return type == ARGUMENT || type == FLAG || type == DOUBLE_FLAG || type == QUOTE || type == LITERAL;

}