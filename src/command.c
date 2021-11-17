/*
 * =====================================================================================
 *
 *       Filename:  command.c
 *
 *     
 *
 *        Version:  1.0
 *        Created:  11/13/21 22:57:42
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "../include/command.h"


static bool not_exit(cmd_t *cmd);

cmd_t *init_command(void)
{
  cmd_t *command         = (cmd_t*)malloc(sizeof(cmd_t));
  command->line          = (char*)NULL;
  command->len           = 0;
  command->cursor        = 0;
  command->not_exit      = not_exit;

  return command;

}

static bool not_exit(cmd_t *cmd) 
{
  if(cmd->line == NULL)
    return true;

  return !(strncmp(cmd->line, EXIT2, cmd->len) == 0);
}

