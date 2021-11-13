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

typedef struct command
{
  char *input; 
  ssize_t len;
  size_t num_bytes;

}command_t;

command_t *init_command(void);

#endif
