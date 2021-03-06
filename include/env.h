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
#define HOME_ENV "HOME"
#define DELIM ":"

typedef struct env{

  map_t    *vars;                                        /* envirioment variables stored in a hash table */
  map_t    *temp_vars;                                   /* temporary envirioment when using env command */
  char     *user;                                        /* user alias for quick access */
  char     *pwd;                                         /* pwd alias env var for quick access */
  char     *prev_pwd;                                    /* the previous pwd before a dir change for cd - */
  char     **paths;                                      /* paths from $PATH split into a array */
  int      paths_len;                                    /* length of path array */
  void     (*load)  (struct env*, char**);               /* loads envrioment variable into memory */
  void     (*free)  (struct env*);                       /* frees env module from memory */
  void     (*print) (struct env*, bool);                 /* prints current envirioment */
  void     (*print_temp) (struct env*, bool);            /* prints current temp envirioment */
  void     (*restore_env) (struct env*, char**, bool);   /* restore an envirioment from **envs array */
  void     (*update_pwdprev) (struct env*, char*);       /* updates de prev_pwd field */
  void     (*load_paths) (struct env*);                  /* loads up the paths field from the env */
  void     (*load_user) (struct env*);                   /* loads up the user field from the env */
  void     (*load_pwd) (struct env*);                    /* loads up the pwd field from the env */  

}env_t;

/* intializes module */
env_t *init_env(void);

#endif

