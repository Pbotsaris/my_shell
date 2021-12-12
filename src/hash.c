/*
 * =====================================================================================
 *
 *       Filename:  hash.c
 *
 *    Description:   
 *
 *        Version:  1.0
 *        Created:  11/15/21 07:42:41
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include "../include/hash.h"

/* PUBLIC */
static void insert(map_t *map, char *key, char *pair);
static entry_t *get(map_t *map, char *key);
static bool destroy(map_t *map, char *key);
static void destroy_all(map_t *map);
static void print_all(map_t *map, bool null_flag);
static void free_map(map_t *map);
static char **to_array(map_t *map);


/* PRIVATE */
static entry_t *create_entry(char *key, char *pair);
static entry_t *update_entry(entry_t *entry, char *pair);
static int count(map_t *map);
static unsigned int hash(const char *key);

/* PUBLIC INITIALIZER */

map_t *init_map(void)
{
  map_t *map            = (map_t*) malloc(sizeof(map_t));
  map->entries          = (entry_t**)malloc(T_SIZE * sizeof(entry_t*));
  map->insert           = insert;
  map->get              = get;  
  map->destroy          = destroy;
  map->destroy_all      = destroy_all;
  map->print_all        = print_all;
  map->free             = free_map;
  map->to_array         = to_array;

  for(int i = 0; i < T_SIZE; i++)
    map->entries[i] = NULL;

  return map;

}

/* PUBLIC METHODS */


static void insert(map_t *map, char *key, char *pair)
{
  unsigned int slot    = hash(key);
  entry_t *entry       = map->entries[slot];

  if(entry == NULL)
  {
    map->entries[slot] = create_entry(key, pair);
    return;
  }

  entry_t *prev;

  while(entry)
  {
    if(strcmp(entry->key, key) == 0)
    {
      free(entry->pair);
      update_entry(entry, pair);
    }
    prev = entry;
    entry = prev->next;

  }
  /* no match, add to list */
  prev->next  = create_entry(key, pair);

}

/**/

static entry_t *get(map_t *map, char *key)
{

  unsigned int slot     = hash(key);
  entry_t *entry        = map->entries[slot];

  if(entry == NULL)
    return NULL;

  while(entry != NULL)
  {
    if(strcmp(entry->key, key) == 0)
      return entry;

    entry = entry->next;

  }
  return NULL;
}

/**/

static bool destroy(map_t *map, char *key)
{

  unsigned int slot     = hash(key);
  entry_t *entry        = map->entries[slot];
  entry_t *prev         = NULL;

  if(entry == NULL)
    return false;


while(entry != NULL)
  {
    if(strcmp(entry->key, key) == 0)
    {
    if(prev)
       prev->next = entry->next;
    /* if deleting first item of linked list */
    else
      map->entries[slot] = entry->next;

    free(entry->key);
    entry->key = NULL;
    free(entry->pair);
    entry->key = NULL;
    free(entry);
    entry = NULL;
    return true;
    }
    prev = entry;
    entry = entry->next;

  }

//  map->entries[slot] = NULL;

  return false;

}

/**/

static void destroy_all(map_t *map)
{

for(int i = 0; i < T_SIZE; i++)
  {
    entry_t *entry = map->entries[i];
    entry_t *entry_to_free;

    if(entry == NULL)
      continue;

    while(entry != NULL)
    {
     free(entry->key);
     entry->key = NULL;
     free(entry->pair);
     entry->key = NULL;

     entry_to_free = entry;
     entry = entry->next;

     free(entry_to_free);
     entry_to_free = NULL;
    }

    map->entries[i] = NULL;

  }
    

}

/**/

static void free_map(map_t *map)
{

  destroy_all(map);

  free(map->entries);
  map->entries = NULL;
  free(map);
  map = NULL;
}

/**/

static void print_all(map_t *map, bool null_flag)
{
  for(int i = 0; i < T_SIZE; i++)
  {
    entry_t *entry = map->entries[i];

    if(entry == NULL)
        continue;

    while(entry != NULL)
    {
      if(!null_flag)
      printf("%s=%s\n", entry->key, entry->pair);
      else
      printf("%s=%s", entry->key, entry->pair);
      entry = entry->next;
    }

  }

   if(null_flag)
      printf("\n");

}

/**/

static char **to_array(map_t *map)
{
  int n_entries        = count(map);
  int i_envs           = 0;
  char **envs          =(char**)malloc((n_entries + 1) * sizeof(char*));

  for(int i = 0; i < T_SIZE; i++)
  {
    entry_t *entry = map->entries[i];

    if(entry == NULL)
      continue;

    while(entry != NULL)
    {
      size_t klen     = strlen(entry->key);
      size_t plen     = strlen(entry->pair);
      char *env       = (char*) malloc((klen + plen + 2) * sizeof(char));

      strncpy(env, entry->key, klen);
      env[klen]       = '\0';

      strncat(env, "=", 2);
      strncat(env, entry->pair, plen);

      envs[i_envs]   = env;

      entry          = entry->next;
      i_envs++;
    }
  }

  /* null last item */
  envs[i_envs] = NULL;
  return envs;
}

/* PRIVATE */

static entry_t *update_entry(entry_t *entry, char *pair)
{
  size_t plen       = strlen(pair);
  entry->pair       = (char*)malloc((plen + 1) * sizeof(char));

  strncpy(entry->pair, pair, plen);
  entry->pair[plen]       = '\0';
 

  return entry;

}

/**/

static entry_t *create_entry(char *key, char *pair)
{
  size_t klen       = strlen(key);
  size_t plen       = strlen(pair);
  entry_t *entry    = (entry_t*)malloc(sizeof(entry_t));
  entry->key        = (char*)malloc( (klen  + 1) * sizeof(char));
  entry->pair       = (char*)malloc((plen + 1) * sizeof(char));
  entry->next       = NULL;

  strncpy(entry->key, key, klen);
  strncpy(entry->pair, pair, plen); 

  entry->key[klen] = '\0';
  entry->pair[plen] = '\0';

  return entry;
}

/**/

static int count(map_t *map)
{

  int count = 0;

  for(int i = 0; i < T_SIZE; i++)
  {
    entry_t *entry = map->entries[i];

    if(entry == NULL)
      continue;

    while(entry != NULL)
    {
      count++;
      entry = entry->next;
    }
  }

  return count;
}


/**/

/* http://www.cse.yorku.ca/~oz/hash.html  */
static unsigned int hash(const char *key)
{

  unsigned long hash = HASH;
  int c;

  while(( c = *key++))
    hash = ((hash << 5) + hash) + c;

  return hash % T_SIZE;
}

