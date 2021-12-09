/*
 * =====================================================================================
 *
 *       Filename:  dir.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/23/2021 19:12:49
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "../include/path.h"

/* PRIVATE FUNCTIONS */
static void add_to_tail(pathnode_t *head, char *buffer, int buff_index);
static pathnode_t *create_node(pathnode_t *node, char *buffer, int buff_index);
static bool dir_exists(char *dir_name, char *path);
static int join_path(pathnode_t *dir_names, char *path, size_t plen);

/* PUBLIC */

pathnode_t *split_path(char *path)
{
  pathnode_t *head   = NULL;
  int path_index     = 0;
  int buff_index     = 0;
  char buffer[strlen(path)];
  buffer[0]          = '\0';

  while(path[path_index] != '\0')
  {

    if(path[path_index] != '/' && path[path_index + 1] != '\0')
      buffer[buff_index++] = path[path_index];

    else
    { 
      if(path[path_index + 1] == '\0')
        buffer[buff_index++] = path[path_index];

      if(!head)
        head  = create_node(head, buffer, buff_index);

      else
        add_to_tail(head, buffer, buff_index);

      buff_index = 0;
    }
    path_index++;
  }

  return head;
}

/**/

bool path_exists(pathnode_t *dir_names, char *path)
{
  if(dir_names == NULL)
    return true;

  size_t plen = strlen(path);
  size_t dlen = strlen(dir_names->value);
  char buffer[plen + dlen + 2]; /* space for the / */
  strncpy(buffer, path, plen);
  buffer[plen] = '\0';

  if(dir_exists(dir_names->value, buffer))
  {
    join_path(dir_names, buffer, plen);
    return path_exists(dir_names->next, buffer);
  }
  else
    return false;

//  free_dir_path(dir_names);
}


void free_paths(pathnode_t *dir_names)
{
  pathnode_t *next;

  while(dir_names)
  {
    free(dir_names->value);
    next = dir_names->next;
    free(dir_names);
    dir_names = next;
  }
}

char *new_pwd(char *pwd, char *to_join)
{

  size_t pwd_len                  = strlen(pwd);
  size_t join_len                 = strlen(to_join);
  /* + 2 for null char and separtor */
  size_t new_pwd_len              = pwd_len + join_len + 2;
  char *new_pwd                   = (char*)malloc( new_pwd_len * sizeof(char));

  strncpy(new_pwd, pwd, pwd_len);
  new_pwd[pwd_len]                = '/';
  new_pwd[pwd_len + 1]            = '\0';
  strncat(new_pwd, to_join, new_pwd_len);
  new_pwd[new_pwd_len - 1]        = '\0';

  return new_pwd;


}


/* PRIVATE */

static bool dir_exists(char *dir_name, char *path)
{
  struct   dirent *dirent;
  DIR      *dir;

  if((dir = opendir(path)) == NULL)
  {
    printf("error: %s\n",strerror(errno));
    return false;
  }

  while((dirent = readdir(dir)) != NULL)
  {

    if(dirent->d_type == DT_DIR && strcmp(dirent->d_name, dir_name) == 0)
    {
      closedir(dir);
      return true;
    }
  }

  closedir(dir);
  return false;

}


/**/

static int join_path(pathnode_t *dir_names, char *path, size_t plen)
{
  int index = 0;
  path[plen++] = '/';

  while(dir_names->value[index] != '\0')
  {
    path[plen++] = dir_names->value[index];
    index++;
  }

  path[plen++] = '\0';

  return plen;
}

/**/

static void add_to_tail(pathnode_t *head, char *buffer, int buff_index)
{
  pathnode_t *temp = head;
  pathnode_t *node = NULL;

  while(temp)
  {

    if(!temp->next)
    {
      temp->next = create_node(node, buffer, buff_index);
      return;

    }

  }

}

/**/

static pathnode_t *create_node(pathnode_t *node, char *buffer, int buff_index)
{
  node = (pathnode_t*)malloc(sizeof(pathnode_t));
  node->value = (char*)malloc((buff_index + 2) * sizeof(char));
  strncpy(node->value, buffer, buff_index + 1);
  node->value[buff_index] = '\0';
  node->next = NULL;

  return node;

}

