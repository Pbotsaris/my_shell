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
static void print_all(map_t *map);
static void free_map(map_t *map);

/* PRIVATE */
static entry_t *create_entry(char *key, char *pair);
static entry_t *update_entry(entry_t *entry, char *pair);
static unsigned int hash(const char *key);

/* PUBLIC INITIALIZER */

map_t *init_map(void)
{
  map_t *map            = (map_t*) malloc(sizeof(map_t));
  map->entries          = (entry_t**)malloc(T_SIZE * sizeof(entry_t*));
  map->insert           = insert;
  map->get              = get;  
  map->destroy          = destroy;
  map->print_all        = print_all;
  map->free             = free_map;

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
      free(pair);
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

  while(entry)
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

  if(entry == NULL)
    return false;

  free(entry->key);
  free(entry->pair);
  free(entry);

  map->entries[slot] = NULL;

  return true;

}

/**/

static void free_map(map_t *map)
{
  free(map->entries);
  free(map);
}


static void print_all(map_t *map)
{
  for(int i = 0; i < T_SIZE; i++)
  {
    entry_t *entry = map->entries[i];

    if(entry == NULL)
      continue;

    while(entry)
    {
      printf("%s=%s\n", entry->key, entry->pair);
      entry = entry->next;
    }

  }

}

/* PRIVATE */

static entry_t *update_entry(entry_t *entry, char *pair)
{
  size_t plen       = strlen(pair);
  entry->pair       = (char*)malloc((plen + 1) * sizeof(char));

  strncpy(entry->pair, pair, plen);

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

  strncpy(entry->key, key, klen);
  strncpy(entry->pair, pair, plen);

  return entry;
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

