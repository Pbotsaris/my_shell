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
#include "../include/command.h"

static bool is_exit(command_t *command);
void main_loop(void)
{

  command_t *command = init_command();

  while (!(is_exit(command)))
  {
    command->len = getline(&command->input, &command->num_bytes, stdin);
  }

  free(command->input);
  free(command);

}


static bool is_exit(command_t *command) {
  if(command->input == NULL)
    return false;

  if(command->len == -1)
    return true;

  return strncmp(command->input, EXIT, command->len + 1) == 0;
}





