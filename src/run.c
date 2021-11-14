/*
 * =====================================================================================
 *
 *       Filename:  run.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/13/21 22:07:50
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "../include/run.h"

static bool is_not_exit(cmd_t *command);
static void prompt(void);
static char *extract_paths(char *env);
static int key_length(char *env);
static char *read_pair(char *env, int key_len);
static bool is_path(char *env, int key_len);

void main_loop(void)
{

  cmd_t *command = init_command();

  while (is_not_exit(command))
  {
    prompt();
    command->len = getline(&command->input, &command->num_bytes, stdin);
  }

  free(command->input);
  free(command);

}

void read_envs(char **envs)
{

  int i = 0;
  while(envs[i]) 
  {
   char *path = extract_paths(envs[i]);
    i++;
  }

}

static bool is_not_exit(cmd_t *command) {
  if(command->input == NULL)
    return true;

  if(command->len == -1)
    return false;

  return !(strncmp(command->input, EXIT, command->len + 1) == 0);
}

static void prompt(void) {printf("%%> ");}


static char *extract_paths(char *env)
{

  int key_len = key_length(env);

  if(is_path(env, key_len))
    return read_pair(env, key_len);


  return NULL;
}

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

static bool is_path(char *env, int key_len)
{

  char key[key_len]; 
  strncpy(key, env, key_len);
  key[key_len] = '\0';

  if(strncmp(PATH, key, key_len) == 0)
    return true;

  return false;

}




