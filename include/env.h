/*
 * =====================================================================================
 *
 *       Filename:  env.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/14/21 13:18:26
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
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

#define PATH "PATH"
#define DELIM ":"

typedef struct env{

  int paths_len;
  char **paths;
  void (*read_envs) (struct env*, char**);
  void (*free) (struct env*);

}env_t;


env_t *init_env(void);

#endif

