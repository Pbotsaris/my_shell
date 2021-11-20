/*
 * =====================================================================================
 *
 *       Filename:  lexer.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/15/21 23:20:33
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
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
  /* Builtins */
  ECHO,                      /* 00 */
  CD,                        /* 01 */
  SETENV,                    /* 02 */
  UNSETENV,                  /* 03 */
  ENV,                       /* 04 */
  PWD,                       /* 05 */
  EXIT,                      /* 06 */
  WHICH,                     /* 07 */
  /* execve */
  PASS_THROUGH,              /* 08 */
  /* syntax */
  LITERAL,                   /* 09 */
  ARGUMENT,                  /* 10 */
  VARIABLE_ASSIGN,           /* 11 */
  VARIABLE,                  /* 12 */
  ASSIGN_OPERATOR,           /* 13 */
  FLAG,                      /* 14 */
  DOUBLE_FLAG,               /* 14 */
  QUOTE,                     /* 16 */
}type_t;
typedef struct token
{
  char  *value;
  type_t type;

}token_t;

typedef struct lexer
{
 char *line;
 size_t len;
 int cursor;
 bool is_first;
 void (*load) (struct lexer*, char*);
 token_t *(*get_next_token) (struct lexer*);

}lexer_t;


lexer_t *init_lexer();

#endif
