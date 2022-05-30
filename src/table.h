#ifndef STM_TABLE_H
#define STM_TABLE_H

#include "state.h"

/*
 * TABLE
 *
 * Table of state names and constructors
 */
struct STM_T {
  unsigned int       allocated;
  unsigned int       count;
  char             **names;
  STM_S_Constructor *constructors;
};

/*
 * Table functions
 *
 * create  | Create the table
 * destroy | Destroy the table
 * add     | Adds a row
 * get     | Gets the constructor
 */
int               STM_t_create (struct STM_T *table);
int               STM_t_destroy(struct STM_T *table);
int               STM_t_add    (struct STM_T *table, const char *name, int (*constructor)(struct STM_S *state));
STM_S_Constructor STM_t_get    (struct STM_T *table, const char *name);

#endif
