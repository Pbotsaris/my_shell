/*
 * =====================================================================================
 *
 *       Filename:  command.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/13/21 22:58:05
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef COMMAND_H
#define COMMAND_H
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define EXIT2 "exit"

typedef struct command
{
  char *line; 
  ssize_t len;
  int cursor;

  bool (*not_exit) (struct command*);

}cmd_t;

cmd_t *init_command(void);

#endif
