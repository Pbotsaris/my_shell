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

//static void print_envs(prgm_t *program);

void main_loop(char **envs)
{

  prgm_t *program = init_program(envs);
  map_t *map = init_map();

   map->insert(map, "pedro", "500");
   map->insert(map, "marina", "200");
   map->insert(map, "wilson", "100");

  // entry_t *entry = map->get(map, "pedro");
  // printf("%s=%s\n", entry->key, entry->pair);
  //
   map->print_all(map);
   map->destroy(map, "marina");
   map->print_all(map);

 //  print_envs(program);
  while (program->cmd->not_exit(program->cmd))
  {
    program->print_prompt(program);
    program->readline(program);
  }

  map->free(map);
  program->free(program);

}


