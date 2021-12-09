/*
 * =====================================================================================
 *
 *       Filename:  lexer.h
 *
 *    Description:  The lexer modules tokenizes an expression producing tokens as 
 *                  requested by the parser module.
 *
 *        Version:  1.0
 *        Created:  11/15/21 23:20:33
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pedro Botsaris
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef LEXER_H
#define LEXER_H

#define CMD_ECHO "echo"
#define CMD_CD "cd"
#define CMD_PWD "pwd"
#define CMD_ENV "env"
#define CMD_SETENV "setenv"
#define CMD_UNSETENV "unsetenv"
#define CMD_WHICH "which"
#define CMD_EXIT "exit"


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include <ctype.h>
typedef enum type {
                       /* builtins */
  ECHO,                      /* 00 */
  CD,                        /* 01 */
  SETENV,                    /* 02 */
  UNSETENV,                  /* 03 */
  ENV,                       /* 04 */
  PWD,                       /* 05 */
  EXIT,                      /* 06 */
  WHICH,                     /* 07 */
                         /* syntax */
  LITERAL,                   /* 08 */
  VARIABLE_ASSIGN,           /* 09 */
  VARIABLE,                  /* 10 */
  ASSIGN_OPERATOR,           /* 11 */
  FLAG,                      /* 12 */
  DOUBLE_FLAG,               /* 13 */
  QUOTE,                     /* 14 */
  WHITESPACE,                /* 15 */
  LINE_BREAK,                /* 16 */
                         /* execve */
  PASS_THROUGH,              /* 17 */

}type_t;

/*
 *          the token produced by the lexer as it tokenized an expression. 
 *          It has a value and the type as defined above.
 *          these tokens are consumed by the parser.c
 */

typedef struct token
{
  char  *value;
  type_t type;

}token_t;

typedef struct lexer
{
 char *line;                                       /* the expression being tokenized */  
 size_t len;                                       /* the length of the expression being tokenized */  
 int cursor;                                       /* tracks the char position in line as lexer tokenizes the expression */ 
 bool is_first;                                    /* first handle commands then arguments */
 bool is_echo;                                     /* echo whitespaces get special treatment. tracks if was echo */
 void (*load) (struct lexer*, char*);              /* loads line into lexer */
 token_t *(*get_next_token) (struct lexer*);       /* produces next token. this method is called by the parser */

}lexer_t;

typedef int (fpointer_t)(lexer_t*, type_t, char*);

/* INITIALIZER */
lexer_t *init_lexer();

#endif
