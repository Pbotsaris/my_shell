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
static void print_env(env_t *env, bool null_flag);
static void print_temp_env(env_t *env, bool null_flag);
static void restore_envs(env_t *env, char **envs, bool is_init);
static void update_pwdprev(env_t *env, char *pwd);
static void load_paths(env_t *env);
static void load_user(env_t *env);
static void load_pwd(env_t *env);

/* PRIVATE */
static void load_envs(env_t *env, char **envs);

/* PRIVATE HELPERS */
static void split_paths(env_t *env, char *paths);
static char* get_key(char *env);
static char* get_pair(char *env);
static int count_paths(char *paths);
static char *read_pair(char *env, int key_len);
static int key_length(char *env);

/* PLUBIC INITIALIZER */

env_t *init_env(void)
{
  env_t *env           = (env_t*)malloc(sizeof(env_t));
  env->vars            = init_map();
  env->temp_vars       = init_map();
  env->paths           = NULL;
  env->pwd             = NULL;
  env->prev_pwd        = NULL;
  env->user            = NULL;
  env->load            = load;
  env->free            = free_envs;
  env->print           = print_env;
  env->print_temp      = print_temp_env;
  env->restore_env     = restore_envs;
  env->update_pwdprev  = update_pwdprev;
  env->load_paths      = load_paths;
  env->load_user       = load_user;
  env->load_pwd        = load_pwd;

  return env;
}

/* PUBLIC METHOD */

static void load(env_t *env, char **envs)
{
  if(envs[0] != NULL)
     load_envs(env, envs);
}

/**/

static void free_envs(env_t *env)
{
  for(int i = 0; i < env->paths_len; i++)
    free(env->paths[i]);

  /* free hash maps after use */
  env->vars->free(env->vars);
  env->vars = NULL;
  env->temp_vars->free(env->temp_vars);
  env->temp_vars = NULL;

  if(env->prev_pwd)
    free(env->prev_pwd);

  free(env->paths);
  env->paths = NULL;
  free(env);

  env = NULL;
}

/**/

static void print_env(env_t *env , bool null_flag)
{
  env->vars->print_all(env->vars, null_flag);
}


/**/

static void update_pwdprev(env_t *env, char *pwd)
{

  if(env->prev_pwd)
  {
    free(env->prev_pwd);
    env->prev_pwd   = NULL;
  }

  size_t len         = strlen(pwd);
  env->prev_pwd      = (char*) malloc((len + 1) * sizeof(char));
  strncpy(env->prev_pwd, pwd, len);
  env->prev_pwd[len] = '\0';

}


/**/

static void print_temp_env(env_t *env, bool null_flag)
{
  env->temp_vars->print_all(env->temp_vars, null_flag);
}

/**/

static void load_paths(env_t *env)
{

  entry_t *path     = env->vars->get(env->vars, PATH_ENV);

  if(path)
    split_paths(env, path->pair);
}


static void load_user(env_t *env)
{

  entry_t *user = env->vars->get(env->vars, USER_ENV);

  if(user)
    env->user = user->pair;
}


static void load_pwd(env_t *env)
{
  entry_t *pwd = env->vars->get(env->vars, PWD_ENV);
  if(pwd)
    env->pwd = pwd->pair;
}


/**/

static void restore_envs(env_t *env, char **envs, bool is_init){

  int i              = 0;

  while(envs[i]) 
  {
    char *key        = get_key(envs[i]);
    char *pair       = get_pair(envs[i]);

    /* only load up main env during initialization */
    if(is_init)
      env->vars->insert(env->vars, key, pair);

    /* loads a copy of envirioment to be modifed by the env command */
    env->temp_vars->insert(env->temp_vars, key, pair);

    free(pair);
    free(key);

    i++;
  }

}


/* PRIVATE */

static void load_envs(env_t *env, char **envs)
{
  restore_envs(env, envs, true);

  entry_t *path     = env->vars->get(env->vars, PATH_ENV);
  entry_t *user     = env->vars->get(env->vars, USER_ENV);
  entry_t *pwd      = env->vars->get(env->vars, PWD_ENV);

  /* shortcut for easy access */
  env->user         = user->pair;
  env->pwd          = pwd->pair;

  update_pwdprev(env, pwd->pair);
  split_paths(env, path->pair);

}


/* PRIVATE HELPERS */

/**/

static void split_paths(env_t *env, char *paths)
{

  size_t plen          = strlen(paths);
  char temp[plen];

  /* strtok modfies str so make a cpy */
  strncpy(temp, paths, plen);

  temp[plen]           = '\0';
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
