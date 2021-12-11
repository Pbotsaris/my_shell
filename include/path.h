/*
 * =====================================================================================
 *
 *       Filename:  dir.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/23/2021 19:13:30
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef DIR_H
#define DIR_H

#include <dirent.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include  <errno.h>

#define CURRENT_DIR "."
#define PARENT_DIR ".."
#define ROOT "/"
#define MAX_DIR_LEN 255
#define MAX_PATH_LEN 4096

typedef struct path_node
{
char value[MAX_DIR_LEN];
struct path_node *next;
struct path_node *prev;

}pathnode_t;

bool dir_exists(char *path);
char *join_path(char *pwd, char *to_join);  

pathnode_t *split_to_list(char *path);
char *build_path(pathnode_t *head);

#endif



