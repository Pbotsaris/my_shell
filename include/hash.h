/*
 * =====================================================================================
 *
 *       Filename:  hash.h
 *
 *    Description:  Hash map implemented using separate chaining. The size of the table
 *                  can be adjusted via the T_SIZE constant.
 *
 *        Version:  1.0
 *        Created:  11/15/21 07:44:07
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Pedro Botsaris
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef HASH_H
#define HASH_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define T_SIZE 64
#define HASH 5381

/*
 *      The entry struct represents an entry in the hash map. It contains
 *       the key and pair values as well as a pointer to next items 
 *       for separate chaining.
 */

typedef struct entry {

  char *key;
  char *pair;
  struct entry *next;

}entry_t;

typedef struct map {

 entry_t **entries;

 void            (*insert) (struct map*, char*, char*);          /* Inserts a entry_t to hash map . Ovewrites if same key */
 entry_t        *(*get) (struct map*, char*);                    /* Searches and retrieves an entry_t with the specific key. Returns NULL if not found */
 bool            (*destroy) (struct map*, char*);                /* Removes an entry_t of the specific key. returns true of successful */
 void            (*destroy_all) (struct map*);                   /* Removes every entry_t in hash map */ 
 void            (*print_all) (struct map*, bool);               /* Prints the value of every entry_t in the hash map. */ 
 char          **(*to_array) (struct map*);                      /* Converts hash map to array of strings like so {"key", pair, "key", "pair" ...} */ 
 void            (*free) (struct map*);                          /* Free up all memory related to this module including the module itself */ 

}map_t;

map_t *init_map(void);

#endif
