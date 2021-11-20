/*
 * =====================================================================================
 *
 *       Filename:  lexer.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/15/21 23:20:17
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "../include/lexer.h"


/* PUBLIC METHODS */
static void load(lexer_t *lexer, char *cmd);
static token_t *get_next_token(lexer_t *lexer);

/* PRIVATE */
static token_t *first_token(lexer_t *lexer);
static token_t *next_tokens(lexer_t *lexer);

/* PRIVATE HELPERS */
static token_t *tokenize(lexer_t *lexer, type_t type);
static token_t *passthrough(char* cmd, int len);

static token_t *token_with_value(char *cmd, int len, type_t type);
static token_t *create_token(type_t type);
static void add_token_value(token_t *token, char* cmd, int len);
static int extract_value(lexer_t *lexer, type_t type, char *buffer);

/* CONDITIONALS */
static bool has_more_tokens(lexer_t *lexer);
static bool is_quote(char *line, int cursor);
static bool is_flag(char *line, int cursor);
static bool is_doubleflag(char *line, int cursor);
static bool is_whitespace(char *line, int cursor);
static bool is_assign_operator(char *line, int cursor);
static bool is_literal(char *line, int cursor); 
static bool is_argument(char *line, int cursor);
static bool is_echo(char *cmd, int len);    
static bool is_cd(char *cmd, int len);      
static bool is_setenv(char *cmd, int len);  
static bool is_unsetenv(char *cmd, int len);
static bool is_env(char *cmd, int len);     
static bool is_pwd(char *cmd, int len);     
static bool is_which(char *cmd, int len);   
static bool is_exit(char *cmd, int len);    

static bool is_var_assignment(char *line, int len);
static bool is_var(char *line, int cursor);


/* PUBLIC INITIALIZER */

lexer_t *init_lexer()
{

  lexer_t *lexer = (lexer_t*) malloc(sizeof(lexer_t));

  lexer->cursor           = 0;
  lexer->line             = (char*)NULL;
  lexer->is_first         = true;
  lexer->load             = load;
  lexer->get_next_token   = get_next_token;

  return lexer;
}

/* PUBLIC METHODS */

static void load(lexer_t *lexer, char *cmd)
{
  lexer->cursor            =  0;
  lexer->len               = strlen(cmd);
  lexer->line              = (char*)malloc((lexer->len + 1) * sizeof(char));
  lexer->is_first          = true;

  strncpy(lexer->line, cmd, lexer->len);
   
   lexer->line[lexer->len] = '\0';
}

static token_t *get_next_token(lexer_t *lexer)
{

  if(!(has_more_tokens(lexer)))
    return NULL;

  if(lexer->is_first)
    return  first_token(lexer);
  else
    return next_tokens(lexer);
  

}

/* PRIVATE */

static token_t *first_token(lexer_t *lexer)
{

    if(is_whitespace(lexer->line, lexer->cursor))
  {
    lexer->cursor++;
    return get_next_token(lexer);
  }

  char cmd[lexer->len + 2];

  /* the type passed in here doesn't matter as long as it is not a LITERAL */
  int len                  = extract_value(lexer, PASS_THROUGH, cmd);
  lexer->is_first          = false;
  token_t *token           = NULL;

  if(is_echo(cmd, len))
    return create_token(ECHO);

  else if(is_cd(cmd, len))
    return create_token(CD);

  else if(is_setenv(cmd, len))
    return create_token(SETENV);

  else if(is_unsetenv(cmd, len))
    return create_token(UNSETENV);

  else if(is_env(cmd, len))
    return create_token(ENV);

  else if(is_pwd(cmd, len))
    return create_token(PWD);

  else if(is_which(cmd, len))
    return create_token(WHICH);

  else if(is_exit(cmd, len))
    return create_token(EXIT);

  else if(is_var_assignment(lexer->line, lexer->cursor))
     return token_with_value(cmd, len, VARIABLE_ASSIGN);

  else
  {
    /* pass throughs return whole line in token->value */
    token = passthrough(lexer->line, lexer->len);
    lexer->cursor = lexer->len;
  }

  return token;
}

/**/

static token_t *next_tokens(lexer_t *lexer)
{

  if(is_quote(lexer->line, lexer->cursor))
  {
    lexer->cursor++;
    return create_token(QUOTE);
  }

  else if(is_assign_operator(lexer->line, lexer->cursor))
  {
    lexer->cursor++;
    return create_token(ASSIGN_OPERATOR);
  }

  else if(is_flag(lexer->line, lexer->cursor))
  {
    lexer->cursor++;
    return tokenize(lexer, FLAG);
  }

  else if(is_doubleflag(lexer->line, lexer->cursor))
  {
    lexer->cursor += 2;
    return tokenize(lexer, DOUBLE_FLAG);
  }

  else if(is_var(lexer->line, lexer->cursor)){
    /* skip $ */
    lexer->cursor++;
    return tokenize(lexer, VARIABLE);
  }

  else if (is_literal(lexer->line, lexer->cursor))
    return tokenize(lexer, LITERAL);

  else if(is_argument(lexer->line, lexer->cursor))
    return tokenize(lexer, ARGUMENT);

  else if((is_whitespace(lexer->line, lexer->cursor)))
  {
    lexer->cursor++;
    return get_next_token(lexer);
  }

  return NULL;
}

/* PRIVATE HELPERS */

static token_t *tokenize(lexer_t *lexer, type_t type)
{
  char buffer[lexer->len];
  int len = extract_value(lexer, type, buffer);
  return token_with_value(buffer, len, type);
}


static token_t *passthrough(char* cmd, int len)
{
  return token_with_value(cmd, len, PASS_THROUGH);
}

/**/

static token_t *token_with_value(char *cmd, int len, type_t type)
{
  token_t *token = create_token(type);
  add_token_value(token, cmd, len);
  return token;
}

/**/

static void add_token_value(token_t *token, char* cmd, int len)
{
  token->value = (char*)malloc((len + 1) * sizeof(char));
  strncpy(token->value, cmd, len);
  token->value[len] = '\0';
}

/**/

static token_t *create_token(type_t type)
{
  token_t *token = (token_t*) malloc(sizeof(token_t));
  token->type = type;
  /* value is only stored in some cases. See func down stream  */
  token->value = NULL;

  return token;
}

/**/

static int extract_value(lexer_t *lexer, type_t type, char *buffer)
{
  int i = 0;

  while(lexer->line[lexer->cursor] != ' ' && lexer->line[lexer->cursor] != '\0' && lexer->line[lexer->cursor] != '=')
  {
    /* literals are terminated with " */
    if(type == LITERAL && lexer->line[lexer->cursor] == '"')
      break;

    buffer[i] = lexer->line[lexer->cursor];

    lexer->cursor++;
    i++;
  }

  buffer[i++] = '\0';

  return i;
}


/* CODITIONALS */

static bool has_more_tokens(lexer_t *lexer)                { return lexer->cursor < (int)lexer->len; }

static bool is_echo(char *cmd, int len)                    { return strncmp(cmd, CMD_ECHO , len) == 0; }
static bool is_cd(char *cmd, int len)                      { return strncmp(cmd, CMD_CD , len) == 0; }
static bool is_setenv(char *cmd, int len)                  { return strncmp(cmd, CMD_SETENV , len) == 0; }
static bool is_unsetenv(char *cmd, int len)                { return strncmp(cmd, CMD_UNSETENV , len) == 0; }
static bool is_env(char *cmd, int len)                     { return strncmp(cmd, CMD_ENV , len) == 0; }
static bool is_pwd(char *cmd, int len)                     { return strncmp(cmd, CMD_PWD , len) == 0; }
static bool is_which(char *cmd, int len)                   { return strncmp(cmd, CMD_WHICH , len) == 0; }
static bool is_exit(char *cmd, int len)                    { return strncmp(cmd, CMD_EXIT , len) == 0; }
static bool is_quote(char *line, int cursor)               { return line[cursor] == '"';} 
static bool is_flag(char *line, int cursor)                { return line[cursor] == '-' && isalpha(line[cursor + 1]);} 
static bool is_literal(char *line, int cursor)             { return line[cursor - 1] == '"' || line[cursor - 1] == '=';} 
static bool is_doubleflag(char *line, int cursor)          { return line[cursor] == '-' && line[cursor + 1] == '-' && isalpha(line[cursor + 2]);} 
static bool is_whitespace(char *line, int cursor)          { return line[cursor] == ' ' || line[cursor] == '\t' || line[cursor] == 10;}
static bool is_assign_operator(char *line, int cursor)     { return line[cursor] == '=' && isalpha(line[cursor - 1]);} 
static bool is_argument(char *line, int cursor)            { return isalpha(line[cursor]);} 
static bool is_var(char *line, int cursor)                 { return line[cursor] == '$' && isalpha(line[cursor + 1]); }   
static bool is_var_assignment(char *line, int cursor)      { return line[cursor] == '='; }

