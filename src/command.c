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


cmd_t *init_command(void)
{

  cmd_t *command     = (cmd_t*)malloc(sizeof(cmd_t));
  command->input         = NULL;
  command->len           = 0;
  command->num_bytes     = 0;
  command->cursor        = 0;

  return command;

}

static void parse(cmd_t *command)
{

   if(command->input[command->cursor] == ' ')
   {
     command->cursor++;
     parse(command);
   }
}

