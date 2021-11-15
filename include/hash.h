/*
 * =====================================================================================
 *
 *       Filename:  hash.h
 *
 *    Description: 
 *
 *        Version:  1.0
 *        Created:  11/15/21 07:44:07
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
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

typedef struct entry {

  char *key;
  char *pair;
  struct entry *next;

}entry_t;

typedef struct map {

 entry_t **entries;

 void            (*insert) (struct map*, char*, char*);
 entry_t        *(*get) (struct map*, char*);
 bool            (*destroy) (struct map*, char*);
 void            (*free) (struct map*);
 void            (*print_all) (struct map*);

}map_t;


map_t *init_map(void);

#endif
