/*
 * =====================================================================================
 *
 *       Filename:  exec.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/08/21 09:38:15
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef EXEC_H
#define EXEC_H

#include <stdlib.h>
#include "../include/env.h"
#include "../include/parser.h"

#define MAX_ARGV_LEN   100

/* 
 *           the exec struct holds the necessary paramenters
 *           to execute an external program with execve 
 */

typedef struct exec {
  node_t *root;                                                /* root of the ast branch to extract command to be executed by execve */
  char *bin;                                                   /*  the bin to be executed by execve */
  char *argv[MAX_ARGV_LEN];                                    /*  arguments for execve. MAX of 99 arguments */
  char **envp;                                                 /*  envirioment for execve */
  void (*free_envp) (struct exec*);

}exec_t;

exec_t *init_exec(void);

#endif
