/*
 * =====================================================================================
 *
 *       Filename:  env.h
 *
 *    Description:  Envirioment struct. This module holdes data and functionality related
 *                  the users envirioment.
 *
 *        Version:  1.0
 *        Created:  11/14/21 13:18:26
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:   Pedro Botsaris
 *   Organization:  
 *
 * =====================================================================================
 */



#ifndef ENV_H
#define ENV_H

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include "../include/hash.h"

#define PATH_ENV "PATH"
#define USER_ENV "USER"
#define PWD_ENV "PWD"
#define DELIM ":"

typedef struct env{

  map_t    *vars; /* env vars stored in a hash table */
  char     *user;
  char     *pwd;
  char     **paths; /* paths from $PATH split into a array */
  int      paths_len;
  map_t    *bin; /* hold a list of all program in users envirioment */
  int      bin_len;
  void     (*load)  (struct env*, char**);
  void     (*free)  (struct env*);
  void     (*print) (struct env*);

}env_t;

env_t *init_env(void);

#endif

