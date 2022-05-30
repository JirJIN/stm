#include "table.h"
#include <string.h>

#define GROWTH_FACTOR 2
#define INITIAL_COUNT 8

/*
 * STM_t_allocate
 *
 * @desc
 * @param table
 * @param count
 * @return
 */
#define ALLOC_VAR(type, name) \
  { \
    type name; \
    if (!(name = malloc(count * sizeof(*name)))) return -1; \
    memcpy(name, table->name, table->count * sizeof(*name)); \
    free(table->name); \
    table->name = name; \
  }
static int STM_t_allocate(struct STM_T *table, unsigned int count)
{
  if (table->count <= table->allocated) return -1;

  ALLOC_VAR(char **,             names);
  ALLOC_VAR(STM_S_Constructor *, constructors);

  table->allocated = count;

  return 0;
}

/*
 * STM_t_create
 *
 * @desc
 * @param table
 * @return
 */
#define CREATE_MALLOC(var) if (!(table->var = malloc(INITIAL_COUNT * sizeof(*table->var)))) return -1;
int STM_t_create(struct STM_T *table)
{
  CREATE_MALLOC(names);
  CREATE_MALLOC(constructors);

  table->count = 0;
  table->allocated = INITIAL_COUNT;

  return 0;
}

/*
 * STM_t_destroy
 *
 * @desc
 * @param table
 * @return
 */
int STM_t_destroy(struct STM_T *table)
{
  for (unsigned int i = 0; i < table->count; ++i) {
    free(table->names[i]);
  }

  free(table->names);
  free(table->constructors);

  return 0;
}

/*
 * STM_t_add
 *
 * @desc
 * @param table
 * @param name
 * @param constructor
 * @return
 */
int STM_t_add(struct STM_T *table, const char *name, STM_S_Constructor constructor)
{
  if (table->allocated <= table->count) {
    if (STM_t_allocate(table, table->count * GROWTH_FACTOR)) return -1;
  }

  size_t name_size = strlen(name) + 1;
  table->names[table->count] = malloc(name_size * sizeof(char));
  for (size_t i = 0; i < name_size; ++i) table->names[table->count][i] = name[i];

  table->constructors[table->count] = constructor;

  ++table->count;

  return 0;
}

/*
 * STM_t_get
 *
 * @desc
 * @param table
 * @parm name
 * @return
 */
STM_S_Constructor STM_t_get(struct STM_T *table, const char *name)
{
  for (unsigned int i = 0; i < table->count; ++i) {
    if (!(strcmp(table->names[i], name))) {
      return table->constructors[i];
    }
  }

  return NULL;
}
