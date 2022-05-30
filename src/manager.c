#include "manager.h"
#include <stdlib.h>
#include <string.h>

#define GROWTH_FACTOR 2
#define INITIAL_COUNT 8

/*
 * STATE_FN
 *
 * @desc
 *   Calls a state's function
 *
 *   manager->states[] gives an index, put
 *   that index into manage->table->states[]
 *   to get the actual state
 * @param index
 *   Index of the state from the top
 *   of the stack
 * @param name
 *   Name of the function
 */
#define STATE_FN(index, name) (manager->states[manager->count - (1 + index)].fn_##name(&manager->states[manager->count - (1 + index)]))

/*
 * STM_m_allocate
 *
 * @desc
 *   Allocates space for more states
 * @param manager
 *   The manager to allocate more states for
 * @param count
 *   How many states to allocate
 * @return
 *   Success
 */
#define ALLOC_VAR(type, name) \
  { \
    type name; \
    if (!(name = malloc(count * sizeof(*name)))) return -1; \
    memcpy(name, manager->name, manager->count * sizeof(*name)); \
    free(manager->name); \
    manager->name = name; \
  }
static int STM_m_allocate(struct STM_M *manager, size_t count)
{
  if (count <= manager->allocated) return -1;

  ALLOC_VAR(struct STM_S *, states);
  ALLOC_VAR(char **,        names);
  ALLOC_VAR(int *,          alive);

  manager->allocated = count;

  return 0;
}

/*
 * STM_m_create
 *
 * @desc
 *   Create a stack
 * @param manager
 *   The manager to create
 * @return
 *    0 on success
 *   -1 if out of memory
 */
#define CREATE_MALLOC(var) if (!(manager->var = malloc(INITIAL_COUNT * sizeof(*manager->var)))) return -1;
int STM_m_create(struct STM_M *manager, struct STM_T *table)
{
  manager->table = table;

  CREATE_MALLOC(states);
  CREATE_MALLOC(names);
  CREATE_MALLOC(alive);

  manager->queued = 0;
  manager->queue_name = NULL;
  manager->queue_flags = 0;

  manager->count = 0;
  manager->allocated = INITIAL_COUNT;

  return 0;
}

/*
 * STM_m_destroy
 *
 * @desc
 *   Destroys a manager, also destroys
 *   any alive states
 * @param manager
 *   Manager to destroy
 * @return
 *   0 on success
 */
int STM_m_destroy(struct STM_M *manager)
{
  while (manager->count > 0) {
    STM_m_pop(manager);
  }

  if (manager->queue_name) free(manager->queue_name);
  
  free(manager->states);
  free(manager->names);
  free(manager->alive);

  return 0;
}

/*
 * STM_m_push
 *
 * @desc
 *   Push a state onto the manager stack, also
 *   destroys
 * @param manager
 *   Manager to push to
 * @param name
 *   Name of the state to push
 * @return
 *    0 on success
 *   -1 on allocate fail
 */
int STM_m_push(struct STM_M *manager, const char *name, STM_S_Constructor constructor, uint8_t flags)
{
  if (manager->allocated <= manager->count) {
    if (STM_m_allocate(manager, manager->count * GROWTH_FACTOR)) return -1;
  }

  /* Assign the name and create the state */
  size_t name_size = strlen(name) + 1;
  manager->names[manager->count] = malloc(name_size * sizeof(char));
  for (size_t i = 0; i < name_size; ++i) manager->names[manager->count][i] = name[i];

  struct STM_S *state = &manager->states[manager->count];
  state->flags = flags;
  if (constructor(state)) return -1;

  /* Destroy the previous state (if there is any) unless PERSIST_PREV flag is there */
  if (manager->count > 0) {
    if (!(flags & STM_PERSIST_PREV)) {
      STATE_FN(0, destroy); /* 0 because I haven't increased manager count just yet */
    }
  }

  /* Actually create the new state */
  ++manager->count;
  STATE_FN(0, create);

  return 0;
}

/*
 * STM_m_pop
 *
 * @desc
 *   Removes a state from the stack,
 *   like closing a pause screen state
 *
 *   If there is a previous state which
 *   is not alive, it is created and
 *   becomes alive
 * @param manager
 *   Manager to pop from
 * @return
 *    0 on success
 *   -1 on failure
 */
int STM_m_pop(struct STM_M *manager)
{
  if (manager->count < 1) return 0;

  if (STATE_FN(0, destroy)) return -1;
  free(manager->names[manager->count - 1]);

  if (--manager->count > 0) {
    if (!manager->alive[manager->count - 1]) {
      manager->alive[manager->count] = 1;
      STATE_FN(0, create);
    }
  }

  return 0;
}

/*
 * STM_m_pop_until
 *
 * @desc
 *   If there are states, will pop them
 *   until the named state is reached. If
 *   that name doesn't exist all states are
 *   popped. Activates and creates the named
 *   state if it isn't already alive
 * @param manager
 *   STM_M
 * @param name
 *   Name of the state
 * @return
 *   0 on success
 */
int STM_m_pop_until(struct STM_M *manager, const char *name)
{
  while (manager->count > 0) {
    if (!strcmp(manager->names[manager->count - 1], name)) {
      break;
    }
    STM_m_pop(manager);
  }

  if (!manager->alive[manager->count - 1]) {
    STATE_FN(0, create);
    manager->alive[manager->count - 1] = 1;
  }

  return 0;
}

/*
 * STM_m_update
 *
 * @desc
 *   Updates the top most state
 * @param manager
 *   Manager for the state to update
 * @return
 *    0 on success
 *   -1 on failure
 */
int STM_m_update(struct STM_M *manager)
{
  if (STATE_FN(0, update)) return -1;

  return 0;
}

/*
 * STM_m_draw
 *
 * @desc
 *   Draws the top most state, can also
 *   draw one state below with the flag,
 *   but doesn't check if there is a state
 *   below
 * @param manager
 *   Manager for the state(s) to draw
 * @return
 *   Success
 */
int STM_m_draw(struct STM_M *manager)
{
  if (manager->states[manager->count - 1].flags & STM_DRAW_PREV) {
    if (STATE_FN(1, draw)) return -1;
  }

  if (STATE_FN(0, draw)) return -1;

  return 0;
}

/*
 * STM_m_queue
 *
 * @desc
 * @param manager
 * @param name
 * @param flags
 * @return
 */
int STM_m_queue(struct STM_M *manager, const char *name, uint8_t flags)
{
  manager->queued = 1;
  
  size_t name_size = strlen(name) + 1;
  manager->queue_name = malloc(name_size * sizeof(char));
  for (size_t i = 0; i < name_size; ++i) manager->queue_name[i] = name[i];

  manager->queue_flags = flags;

  return 0;
}

/*
 * STM_m_switch
 *
 * @desc
 * @param manager
 * @return
 */
int STM_m_switch(struct STM_M *manager)
{
  /* Search bottom up, if found, pop until, otherwise push */
  for (unsigned int i = 0; i < manager->count; ++i) {
    if (!strcmp(manager->queue_name, manager->names[i])) {
      STM_m_pop_until(manager, manager->queue_name);
      goto dequeue;
    }
  }

  /* Not found, push the state */
  STM_m_push(manager, manager->queue_name, STM_t_get(manager->table, manager->queue_name), manager->queue_flags);

dequeue:
  manager->queued = 0;
  free(manager->queue_name);
  manager->queue_name = NULL;
  manager->queue_flags = 0;

  return 0;
}
