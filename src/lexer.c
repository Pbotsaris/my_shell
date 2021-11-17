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
static token_t *following_tokens(lexer_t *lexer);

/* PRIVATE HELPERS */
static bool has_more_tokens(lexer_t *lexer);
static token_t *do_bin(char* cmd, int len);
static token_t *do_var(char* cmd, int len);
static void add_token_value(token_t *token, char* cmd, int len);
static token_t *create_token(type_t type);
static int extract_command(lexer_t *lexer, char *cmd);

/* CONDITIONALS */
static bool is_quote(char *line, int cursor);
static bool is_flag(char *line, int cursor);
static bool is_doubleflag(char *line, int cursor);
static bool is_whitespace(char *line, int cursor);
static bool is_assign_operator(char *line, int cursor);
static bool is_var_declaration(char *line, int cursor);
static bool is_var_assignment(char *line, int cursor);
static bool is_literal(char *line, int cursor); 
static bool is_others(char *line, int cursor); 
static bool is_echo(char *cmd, int len);    
static bool is_cd(char *cmd, int len);      
static bool is_setenv(char *cmd, int len);  
static bool is_unsetenv(char *cmd, int len);
static bool is_env(char *cmd, int len);     
static bool is_pwd(char *cmd, int len);     
static bool is_which(char *cmd, int len);   
static bool is_exit(char *cmd, int len);    


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

  token_t *token = NULL;

  if(lexer->is_first)
    token =  first_token(lexer);
  else
    token = following_tokens(lexer);

  return token;
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

  int len                  = extract_command(lexer, cmd);
  lexer->is_first          = false;
  token_t *token           = NULL;

  if(is_echo(cmd, len))
    token = create_token(ECHO);

  else if(is_cd(cmd, len))
    token = create_token(CD);

  else if(is_setenv(cmd, len))
    token =  create_token(SETENV);

  else if(is_unsetenv(cmd, len))
    token =create_token(UNSETENV);

  else if(is_env(cmd, len))
    token =create_token(ENV);

  else if(is_pwd(cmd, len))
    token =create_token(PWD);

  else if(is_which(cmd, len))
    token =create_token(WHICH);

  else if(is_exit(cmd, len))
    token =create_token(EXIT);

  else if(is_var_assignment(cmd, len))
    token = do_var(cmd, len);

  else
    token = do_bin(cmd, len);

  return token;
}

static token_t *following_tokens(lexer_t *lexer)
{

  token_t *token            = NULL;

  if(is_quote(lexer->line, lexer->cursor))
    token = create_token(QUOTE);

  if(is_literal(lexer->line, lexer->cursor))
    token = create_token(LITERAL);

  else if(is_assign_operator(lexer->line, lexer->cursor))
    token = create_token(ASSIGN_OPERATOR);

  else if(is_flag(lexer->line, lexer->cursor))
    token =create_token(FLAG);

  else if(is_doubleflag(lexer->line, lexer->cursor))
    token =create_token(DOUBLE_FLAG);

//  else if(is_var_declaration(lexer->line, lexer->cursor))

  else if((is_whitespace(lexer->line, lexer->cursor)))
  {
    lexer->cursor++;
    get_next_token(lexer);
  }

  return token;

}

/* PRIVATE HELPERS */

static token_t *do_bin(char* cmd, int len)
{

  token_t *token      = create_token(BIN);
  add_token_value(token, cmd, len);
  return token;

}

static token_t *do_var(char* cmd, int len)
{

  token_t *token = create_token(VARIABLE);
  add_token_value(token, cmd, len);
  return token;

}

static void add_token_value(token_t *token, char* cmd, int len)
{
  token->value = (char*)malloc((len + 1) * sizeof(char));
  strncpy(token->value, cmd, len);
}


static token_t *create_token(type_t type)
{
  token_t *token = (token_t*) malloc(sizeof(token_t));
  token->type = type;
  /* value only used with binary */
  token->value = NULL;

  printf("token type: %d\n", token->type);

  return token;
}

static int extract_command(lexer_t *lexer, char *cmd)
{

  int i = 0;

  while(lexer->line[lexer->cursor] != ' ' && lexer->line[lexer->cursor] != '\0')
  {
    cmd[i] = lexer->line[lexer->cursor];

    lexer->cursor++;
    i++;
  }

  cmd[i++] = '\0';

  return i;

}
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
static bool is_assign_operator(char *line, int cursor)     { return line[cursor] == '=' && isalpha(line[cursor - 1]) && isalpha(line[cursor + 1]);} 
static bool is_var_declaration(char *line, int cursor)     { return line[cursor] == '$' && isalpha(line[cursor + 1]); }   
static bool is_others(char *line, int cursor)              { return isalpha(line[cursor]);} 

static bool is_var_assignment(char *line, int len)
{


  if(!(isalpha(line[0]))) 
     return false;

   for(int i = 0; i < len; i++)
       if(line[i] == '=' && isalpha(line[i + 1]))
         return true;

  return false;
}

