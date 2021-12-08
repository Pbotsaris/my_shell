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

exec_t *init_exec(void)
{
  exec_t * exec          = (exec_t*)malloc(sizeof(exec_t));
  exec->bin              = NULL;
  exec->envp             = NULL;
  exec->argv[0]          = NULL;
  exec->root             = NULL;
  exec->free_envp        =free_envp;
  
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
