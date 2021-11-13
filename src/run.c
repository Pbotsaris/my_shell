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

static bool is_exit(char *command, ssize_t read_len);

void main_loop(void)
{

  char *command = NULL;
  size_t len = 0;
  ssize_t read_len = 0;

  while ((is_exit(command, read_len)) == false)
  {
    read_len = getline(&command, &len, stdin);
  }

  free(command);

}



static bool is_exit(char *command, ssize_t read_len) {
  if(command == NULL)
    return false;

  if(read_len == -1)
    return true;

  return strncmp(command, EXIT, read_len) == 0;
}





