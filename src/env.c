/*
 * =====================================================================================
 *
 *       Filename:  env.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/14/21 13:16:44
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "../include/env.h"

/* PUBLIC METHOD */
static void load_envs(env_t *env, char **envs);
static void free_envs(env_t *env);
static void print_envs(env_t *env);

/* PRIVATE */
static char *extract(char *env, char *key_name);
static void split_paths(env_t *env, char *paths);

static char* get_key(char *env);
static char* get_pair(char *env);

/* PRIVATE HELPERS */
static int count_paths(char *paths);
static char *read_pair(char *env, int key_len);
static int key_length(char *env);
static bool compare(char *env, char *str_to_cmp, int key_len);

/* PLUBIC INITIALIZER */

env_t *init_env(void)
{
  env_t *env       = (env_t*)malloc(sizeof(env_t));
  env->map         = init_map();
  env->paths       = NULL;
  env->pwd         = NULL;
  env->user        = NULL;
  env->value       = NULL;
  env->load        = load_envs;
  env->free        = free_envs;
  env->print       = print_envs;

  return env;
}

/* PUBLIC METHOD */

static void load_envs(env_t *env, char **envs)
{
  char *paths      = NULL;
  env->value       = envs;
  int i = 0;

  while(envs[i]) 
  {
    paths = extract(envs[i], PATH);
    char *key = get_key(envs[i]);
    char *pair = get_pair(envs[i]);

    env->map->insert(env->map, key, pair);
    free(pair);
    free(key);

    if(paths)
    {
      split_paths(env, paths);
      free(paths);
    }

    if(env->user == NULL)
      env->user  = extract(envs[i], USER);

    if(env->pwd == NULL)
      env->pwd = extract(envs[i], PWD);

    if(env->user && env->paths && env->pwd)
      break;

    i++;
  }
}

/**/

static void free_envs(env_t *env)
{
  for(int i = 0; i < env->paths_len; i++)
    free(env->paths[i]);
  
  
  env->map->free(env->map);

  free(env->user);
  free(env->pwd);
  free(env->paths);
  free(env);
}

/**/

static void print_envs(env_t *env)
{
  env->map->print_all(env->map);
}

/**/

/* PRIVATE */

static char *extract(char *env, char *key_name)
{

  int key_len = key_length(env);

  if(compare(env, key_name, key_len))
    return read_pair(env, key_len);


  return NULL;
}


/**/

static char* get_key(char *env)
{
  int key_len = key_length(env);

  char *key = (char*) malloc(key_len + 1 * sizeof(char));
  strncpy(key, env, key_len);
  key[key_len] = '\0';

  return key;

}

static char* get_pair(char *env)
{
  int key_len = key_length(env);
  return read_pair(env, key_len);

}


/**/

static void split_paths(env_t *env, char *paths)
{

  /* strtok modfies str  */
  size_t plen          = strlen(paths);
  char temp[plen];
  strncpy(temp, paths, plen);

  env->paths_len       =  count_paths(paths);
  char *cursor         = strtok(temp, DELIM);
  int index            = 0;
  env->paths           = (char**) malloc(env->paths_len * sizeof(char*));

  while(cursor)
  {
    size_t len         = strlen(cursor);
    char *path         = (char*) malloc((len + 1) * sizeof(char));

    strncpy(path, cursor, len);

    path[len]          = '\0';
    env->paths[index]  = path;

    cursor = strtok(NULL, DELIM);
    index++;
  }

}


/* PRIVATE HELPERS */

static int count_paths(char *paths)
{
  int i          = 0;
  int count      = 0;
  char delim[]   = DELIM;

  while(paths[i] != '\0')
  {

    if(paths[i]  == delim[0])
      count++;

    i++;
  }

  /* add one as last item does not have a separator */
  return ++count;
}

/**/

static char *read_pair(char *env, int key_len)
{
  int pair_index   = 0;
  size_t len       = strlen(env);
  char *pair       = (char*)malloc((len - key_len) * sizeof(char));

  /*  increment ++key_len to skip '=' delimiter */
  for(int i = ++key_len; i < (int)len; i++)
    pair[pair_index++] =  env[i];

  pair[pair_index++] = '\0';

  return pair;
}

/**/

static int key_length(char *env)
{

  char delim   = '=';
  int key_len  = 0;

  while(env[key_len] != '\0')
  {
    if(env[key_len] == delim)
      break;

    key_len++;
  }

  return key_len;
}


/**/

static bool compare(char *env, char *str_to_cmp, int key_len)
{

  char key[key_len]; 
  strncpy(key, env, key_len);
  key[key_len] = '\0';

  if(strncmp(str_to_cmp, key, key_len) == 0)
    return true;

  return false;
}
