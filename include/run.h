/*
 * =====================================================================================
 *
 *       Filename:  run.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/13/21 22:19:18
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef RUN_H
#define RUN_H

#define EXIT "exit\n"
#define PATH "PATH"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../include/command.h"

typedef struct program
{
  char *path;
  cmd_t *cmd;

}program_t;

void read_envs(char **envs);
void main_loop(void);

#endif
