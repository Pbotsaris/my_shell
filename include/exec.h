/*
 * =====================================================================================
 *
 *       Filename:  exec.h
 *
 *    Description:  This module is responsible for executing an external binary.
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
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>

#include "../include/env.h"
#include "../include/parser.h"
#include "../include/path.h"

#define MAX_ARGV_LEN   100

/* 
 *           the exec struct holds the necessary data and functionality
 *           to execute an external program with execve as well as search path external bins.
 */

typedef struct exec {
  node_t *root;                                                /* root of the ast branch to extract command to be executed by execve */
  char *bin;                                                   /*  the bin to be executed by execve */
  char *argv[MAX_ARGV_LEN];                                    /*  arguments for execve. MAX of 99 arguments */
  char **envp;                                                 /*  envirioment for execve */
  void (*free_envp) (struct exec*);                            /*  frees up array in the envp field */
  void (*empty_envp) (struct exec*);                           /*  creates an empty env in the envp field */
  void (*execute) (struct exec*, char**, int);                 /*  forks and executes an external binary in another process */
  char *(*search_paths) (struct exec*, char**, int);           /* searches if exec->bin exists in paths */

}exec_t;

exec_t *init_exec(void);

#endif
