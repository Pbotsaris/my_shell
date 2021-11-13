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


command_t *init_command(void)
{

  command_t *command     = (command_t*)malloc(sizeof(command_t));
  command->input         = NULL;
  command->len           = 0;
  command->num_bytes     = 0;

  return command;

}




