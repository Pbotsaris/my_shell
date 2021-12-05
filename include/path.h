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

typedef struct path_node
{
char *value;
struct path_node *next;

}pathnode_t;

pathnode_t *split_path(char *path);
void print_dir_path(pathnode_t *dir);
bool path_exists(pathnode_t *dir_names, char *path);
int join_path(pathnode_t *dir_names, char *path, size_t plen);

#endif



