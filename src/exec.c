/*
 * =====================================================================================
 *
 *       Filename:  exec.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  12/08/21 09:37:29
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pedro Botsaris, 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "../include/exec.h"


static void free_envp(exec_t *exec);
static void empty_envp(exec_t *exec);
static void execute(exec_t *exec, char **paths, int path_len);
static void fork_and_exec(exec_t *exec, char *cmd_path);
static char *search_paths(exec_t *exec, char **paths, int path_len);
static bool bin_exists(char *path, char *cmd);

exec_t *init_exec(void)
{
  exec_t * exec          = (exec_t*)malloc(sizeof(exec_t));
  exec->bin              = NULL;
  exec->envp             = NULL;
  exec->argv[0]          = NULL;
  exec->root             = NULL;
  exec->free_envp        = free_envp;
  exec->empty_envp       = empty_envp;
  exec->execute          = execute;
  exec->search_paths     = search_paths;
  
  return exec;

}


static void free_envp(exec_t *exec)
{

  int count = 0;
  while(exec->envp[count])
  {
    free(exec->envp[count]);
    count++;
  }

  free(exec->envp);

}

static void empty_envp(exec_t *exec)
{
  exec->envp = (char**)malloc(sizeof(char*));
  exec->envp[0] = NULL;
}

static void execute(exec_t *exec, char **paths, int paths_len)
{
  bool searched_bin   = false;
  char *cmd_path      = NULL;

  /* check relative or absolute path  */
  if(exec->bin[0] == '.' || exec->bin[0] == '/')
    cmd_path = exec->bin;
  else
  {
    cmd_path = search_paths(exec, paths, paths_len);
    searched_bin = true;
  }

  fork_and_exec(exec, cmd_path ? cmd_path : exec->bin);

  if(cmd_path && searched_bin)
    free(cmd_path);

}


static void fork_and_exec(exec_t *exec, char *cmd_path)
{

  pid_t pid;
  int status;

  if ((pid = fork()) == 0)
  {

    if((execve(cmd_path, exec->argv, exec->envp)) == -1)
    {
      perror(exec->bin);
      exit(EXIT_FAILURE);
    }
  }

  else if(pid == -1)
  {
    printf("error executing binary\n");
    exit(EXIT_FAILURE);
  }

  else
  {
    do 
    {
        // It was terminated by a segfault
      waitpid(pid, &status, WUNTRACED);

      if (WTERMSIG(status) == SIGSEGV)
        printf("%s: Segmentation fault\n", exec->bin);

    }
    while (!WIFEXITED(status) && !WIFSIGNALED(status));

  }
}


static char *search_paths(exec_t *exec, char **paths, int path_len)
{
  int index        = 0;
  bool found       = false;

  for(; index < path_len; index++)
    if(bin_exists(paths[index], exec->bin))
    {
      found = true;
      break;
    }

  if(found)
    return join_path(paths[index], exec->bin);
  else
    return NULL;

}


static bool bin_exists(char *path, char *cmd)
{

  struct dirent *pDirent;
  DIR *dir;


  if((dir = opendir(path)) == NULL)
    return false;

  while((pDirent = readdir(dir)) != NULL)
    if((strcmp(pDirent->d_name, cmd)) == 0)
    {
      closedir(dir);
      return true;
    }

  closedir(dir);

  return false;

}


