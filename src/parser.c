/*
 * =====================================================================================
 *
 *       Filename:  parser.c
 *
 *    Description:  The parser module consumes tokens from the lexer module producing 
 *                  an ast.
 *
 *        Version:  1.0
 *        Created:  11/19/21 09:25:51
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pedro Botsaris, 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "../include/parser.h"

static node_t *parse(parser_t *parser, lexer_t *lexer);
static node_t *command(parser_t *parser, lexer_t *lexer);
static node_t *builtins(parser_t *parser, lexer_t *lexer);
static node_t *pass_through(parser_t *parser, lexer_t *lexer);
static node_t *variable_assign(parser_t *parser, lexer_t *lexer);
static node_t *operands(parser_t *parse, lexer_t *lexer);
static node_t *quotes(parser_t *parser, lexer_t *lexer);
static node_t *literal(parser_t *parser, lexer_t *lexer);
static node_t *arguments(parser_t *parser, lexer_t *lexer);

static node_t *eat(parser_t *parser, lexer_t *lexer, type_t type);
static node_t *create_node(token_t *token);
static void cpy_value(node_t *node, char *value);
static node_t *get_left_tail(node_t *node);

static bool is_literal(type_t type);
static bool is_argument(type_t type);

parser_t *init_parser()
{
  parser_t *parser     = (parser_t*)malloc(sizeof(parser_t));
  parser->lookahead    = NULL;
  parser->parse        = parse;

  return parser;
}

/**/

static node_t *parse(parser_t *parser, lexer_t *lexer)
{

  parser->lookahead    = lexer->get_next_token(lexer);

  if(!parser->lookahead)
  {
    parser->valid = false;
    return NULL;
  }

  return command(parser, lexer);

}

/**/

static node_t *command(parser_t *parser, lexer_t *lexer)
{
  if(parser->lookahead->type == PASS_THROUGH)
    return pass_through(parser, lexer);

  if(parser->lookahead->type == VARIABLE_ASSIGN)
    return variable_assign(parser, lexer);

  else return builtins(parser, lexer);

}

/**/

static node_t *pass_through(parser_t *parser, lexer_t *lexer)
{
    node_t *root = eat(parser, lexer, PASS_THROUGH);

    if(parser->lookahead)
       root->left = operands(parser, lexer);

  return  root;
}


/**/

static node_t *builtins(parser_t *parser, lexer_t *lexer)
{
  node_t *root        = eat(parser, lexer, parser->lookahead->type);

  /* not using right branch for commands as they are not binary operations */
  if(parser->lookahead != NULL)
      root->left      = operands(parser, lexer);

  return  root;
}

/**/

static node_t *variable_assign(parser_t *parser, lexer_t *lexer)
{
  node_t *left       = eat(parser, lexer, VARIABLE_ASSIGN);
  node_t *root       = eat(parser, lexer, ASSIGN_OPERATOR);
  node_t *right      = NULL;

  if(parser->lookahead->type == QUOTE)
     right           = quotes(parser, lexer);
  else
     right           = literal(parser, lexer);

  root->left = left;
  root->right = right;

  return root;

}

/**/

static node_t *operands(parser_t *parser, lexer_t *lexer)
{
  node_t *root       = arguments(parser, lexer);

  while(parser->lookahead && is_argument(parser->lookahead->type))
  {

    /* always to the left */
    node_t *tail     = get_left_tail(root);
    tail->left       = arguments(parser, lexer);

  }

  return root;
}

/**/

static node_t *arguments(parser_t *parser, lexer_t *lexer)
{

  if(parser->lookahead->type == FLAG)
    return eat(parser, lexer, FLAG);

  if(parser->lookahead->type == DOUBLE_FLAG)
    return eat(parser, lexer, DOUBLE_FLAG);

  if(parser->lookahead->type == QUOTE)
    return quotes(parser, lexer);

  if(parser->lookahead->type == LITERAL)
    return eat(parser, lexer, LITERAL);

  if(parser->lookahead->type == VARIABLE)
    return eat(parser, lexer, VARIABLE);

  if(parser->lookahead->type == VARIABLE_ASSIGN)
      return variable_assign(parser, lexer);

  if(parser->lookahead->type == WHITESPACE)
    return eat(parser, lexer, WHITESPACE);

  return NULL;
}

/**/

static node_t *quotes(parser_t *parser, lexer_t *lexer)
{

  eat(parser, lexer, QUOTE);
  node_t *node       = literal(parser, lexer);
  eat(parser, lexer, QUOTE);

  return node;
}

/**/

static node_t *literal(parser_t *parser, lexer_t *lexer)
{
  node_t *literal    = NULL;
  node_t *root       = NULL;
  node_t *tail       = NULL;

  while(parser->lookahead && is_literal(parser->lookahead->type))
  {

  if(parser->lookahead->type == LITERAL)
      literal = eat(parser, lexer, LITERAL);

  else if(parser->lookahead->type == VARIABLE)
     literal = eat(parser, lexer, VARIABLE);

  else if(parser->lookahead->type == WHITESPACE)
     literal = eat(parser, lexer, WHITESPACE);

  else if(parser->lookahead->type == LINE_BREAK)
     literal = eat(parser, lexer, LINE_BREAK);

   if(root)
   {
      tail = get_left_tail(root);
      tail->left     = literal;
   }
   else
     root            = literal;

  }

  return root;
}

/**/

static node_t *eat(parser_t *parser, lexer_t *lexer, type_t type)
{
  token_t *token      = parser->lookahead;

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
    printf("unexpected token. \n");
    parser->valid = false;
    return NULL;
  }

  parser->lookahead    = lexer->get_next_token(lexer);


  /* do not return a tree node for quote */
  if(type == QUOTE)
  {
    free(token);
    return NULL;
  }

  return create_node(token);
}

/**/

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

/**/

static node_t *get_left_tail(node_t *node)
{
  node_t *temp = node;

  while(temp->left)
    temp = temp->left;

  return temp;

}

/**/

static void cpy_value(node_t *node, char *value)
{
  size_t len          = strlen(value);
  node->value         = (char*)malloc((len + 1) * sizeof(char));

  strncpy(node->value, value, len);
  node->value[len]    = '\0';

}

/**/

static bool is_argument(type_t type)
{
  return type == WHITESPACE || type == FLAG || type == DOUBLE_FLAG || type == QUOTE || type == LITERAL || type == VARIABLE || type == VARIABLE_ASSIGN;
}


static bool is_literal(type_t type)
{

  return type == VARIABLE || type == LITERAL || type == WHITESPACE || type == LINE_BREAK;


}

