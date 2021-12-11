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
static bool has_dotdot(pathnode_t *head);
static void add_to_tail(pathnode_t *head, char *buffer, int buff_index);
static pathnode_t *reduce_path(pathnode_t *head);
static pathnode_t *create_node(pathnode_t *prev, char *buffer, int buff_index);
static void free_path(pathnode_t *dir_names);
static char *list_to_path(pathnode_t *head);
static int add_to_buffer(char *buffer, char *path, int buffer_index);

/* PUBLIC */

pathnode_t *split_to_list(char *path)
{
  char buffer[strlen(path)];

  pathnode_t *head   = create_node(NULL, ROOT, 2);
  int path_index     = 1;
  int buff_index     = 0;
  buffer[0]          = '\0';

  while(path[path_index] != '\0')
  {
    if(path[path_index] != '/' && path[path_index + 1] != '\0')
      buffer[buff_index++] = path[path_index];

    else
    { 
      if(buff_index == 0)
      {
        path_index++;
        continue;
      }
      
      if(path[path_index + 1] == '\0')
        buffer[buff_index++] = path[path_index];

      add_to_tail(head, buffer, buff_index);

      buff_index = 0;
    }
    path_index++;
  }

  free(path);

  return head;
}
/**/


char *join_path(char *pwd, char *to_join)
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

/**/

bool dir_exists(char *path)
{
  DIR      *dir;

  if((dir = opendir(path)) == NULL)
  {
    printf("%s\n",strerror(errno));
    return false;
  }

  closedir(dir);
  return true;

}

/**/

char *build_path(pathnode_t *head)
{

  if(has_dotdot(head))
    head = reduce_path(head);

  char *path = list_to_path(head);

  free_path(head);

  return path;

}

/* PRIVATE */

static bool has_dotdot(pathnode_t *head)
{
  pathnode_t *temp = head;

  while(temp)
  {
    if((strcmp(temp->value, PARENT_DIR)) == 0 || (strcmp(temp->value, CURRENT_DIR)) == 0)
      return true;

    temp = temp->next;
  }

  return false;
}

/**/

static pathnode_t *reduce_path(pathnode_t *head)
{
  pathnode_t *temp = head;

  while(temp)
  {
    pathnode_t *current = temp;
    /* skip when no more paths to reduce */
    if(temp->prev == head && ((strcmp(temp->value, PARENT_DIR)) == 0 || (strcmp(temp->value, CURRENT_DIR)) == 0 ))
    {
      free_path(current);
      head->next = NULL;
      break;
    }

    if((strcmp(temp->value, CURRENT_DIR)) == 0)
    {
      temp->prev->next = temp->next;

      if(temp->next)
        temp->next->prev = temp->prev;

      free(current);
      temp = temp->next;
      continue;
    }

    if((strcmp(temp->value, PARENT_DIR)) == 0)
    {
      temp->prev->prev->next = temp->next;

      if(temp->next)
        temp->next->prev = temp->prev->prev;

      free(temp->prev);
      free(current);
    }

    temp = temp->next;
  }

  return head;
}

/**/

static void add_to_tail(pathnode_t *head, char *buffer, int buff_index)
{
  pathnode_t *temp               = head;
  pathnode_t *prev               = NULL;

  while(temp)
  {
    /* doubly linked list */
    temp->prev = prev;

    if(!temp->next)
    {
      temp->next = create_node(temp, buffer, buff_index);
      return;
    }

    prev = temp;
    temp = temp->next;

  }

}

/**/

static char *list_to_path(pathnode_t *head)
{

  char buffer[MAX_PATH_LEN];
  int buffer_index                  = 0;
  pathnode_t *temp                  = head;

  while(temp)
  {
    if(temp == head)
      buffer_index = add_to_buffer(buffer, temp->value, buffer_index);   

    if(temp != head && temp->value[0] != '/')
      buffer_index = add_to_buffer(buffer, temp->value, buffer_index);   

    if(temp != head && temp->next != NULL && temp->value[0] != '\0')
      buffer[buffer_index++] = '/';

    temp = temp->next;
  }

  buffer[buffer_index] = '\0';
  char *path = (char*)malloc((buffer_index + 1) * sizeof(char));

  strncpy(path, buffer, buffer_index);
  path[buffer_index]  = '\0';

  return path;

}

/**/

static int add_to_buffer(char *buffer, char *path, int buffer_index)
{
  size_t len                     = strlen(path);
  int path_index                 = 0;

  for(size_t i = buffer_index; i < buffer_index + len; ++i)
    buffer[i] = path[path_index++];

  return buffer_index + len;
}

/**/

static pathnode_t *create_node(pathnode_t *prev, char *buffer, int buff_index)
{
  pathnode_t *node               = (pathnode_t*)malloc(sizeof(pathnode_t));

  strncpy(node->value, buffer, buff_index + 1);

  node->value[buff_index]        = '\0';
  node->next                     = NULL;
  node->prev                     = prev;

  return node;

}

/**/

static void free_path(pathnode_t *head)
{
  pathnode_t *current;

  while(head)
  {
    current = head;
    free(current);

    head = head->next;
  }
}


