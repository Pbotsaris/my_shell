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
static void load(env_t *env, char **envs);
static void free_envs(env_t *env);
static void print_envs(env_t *env);

/* PRIVATE */
static void load_envs(env_t *env, char **envs);
static void load_bin(env_t *env);

/* PRIVATE HELPERS */
static void load_from_bindir(env_t *env, char *path);
static void split_paths(env_t *env, char *paths);
static char* get_key(char *env);
static char* get_pair(char *env);
static int count_paths(char *paths);
static char *read_pair(char *env, int key_len);
static int key_length(char *env);

/* PLUBIC INITIALIZER */

env_t *init_env(void)
{
  env_t *env       = (env_t*)malloc(sizeof(env_t));
  env->vars        = init_map();
  /* TODO: remove this map */
  env->bin         = init_map();
  env->paths       = NULL;
  env->pwd         = NULL;
  env->user        = NULL;
  env->load        = load;
  env->free        = free_envs;
  env->print       = print_envs;

  return env;
}

/* PUBLIC METHOD */

static void load(env_t *env, char **envs)
{
  load_envs(env, envs);
  /* TODO: no need to load bins into memory. clear rest of code  */
 // load_bin(env);
}


/**/

static void free_envs(env_t *env)
{
  for(int i = 0; i < env->paths_len; i++)
    free(env->paths[i]);

  env->vars->free(env->vars);
  /* TODO: remove this map */
  env->bin->free(env->bin);

  free(env->paths);
  free(env);
}

/**/

static void print_envs(env_t *env)
{
  env->vars->print_all(env->vars);
}


/* PRIVATE */

static void load_envs(env_t *env, char **envs)
{

  int i              = 0;

  while(envs[i]) 
  {
    char *key        = get_key(envs[i]);
    char *pair       = get_pair(envs[i]);

    env->vars->insert(env->vars, key, pair);
    free(pair);
    free(key);

    i++;
  }

  entry_t *path     = env->vars->get(env->vars, PATH_ENV);
  entry_t *user     = env->vars->get(env->vars, USER_ENV);
  entry_t *pwd      = env->vars->get(env->vars, PWD_ENV);
  env->user         = user->pair;
  env->pwd          = pwd->pair;

  split_paths(env, path->pair);

}

/* TODO: remove this map */
static void load_bin(env_t *env)
{

   for(int i = 0; i < env->paths_len; i++)
     load_from_bindir(env, env->paths[i]);
}

/* PRIVATE HELPERS */

/* TODO: there is no need to load all binaries however this logic will be useful down the line
 *       to scan directories in path looking for certain bins.
 */
static void load_from_bindir(env_t *env, char *path)
{

  struct dirent *pDirent;
  DIR *dir;

 if((dir = opendir(path)) == NULL)
     return;

    while((pDirent = readdir(dir)) != NULL)
    {
      if(pDirent->d_name[0] != '.')
      {
          /* hash table will only take unique keys so duplicates are ignored */
          env->bin->insert(env->bin, pDirent->d_name, pDirent->d_name);
      }
    }

    closedir(dir);

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

/**/

static char* get_key(char *env)
{
  int key_len = key_length(env);

  char *key = (char*) malloc(key_len + 1 * sizeof(char));
  strncpy(key, env, key_len);
  key[key_len] = '\0';

  return key;

}

/**/

static char* get_pair(char *env)
{
  int key_len = key_length(env);
  return read_pair(env, key_len);

}

/**/

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
