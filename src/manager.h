#ifndef STM_MANAGER_H
#define STM_MANAGER_H

#include "state.h"
#include "table.h"

/*
 * MANAGER
 *
 * Stack of the states, the actual manager of the states
 *
 * You may need to update the update and draw functions
 * depending on your needs
 *
 */
struct STM_M {
  struct STM_T     *table;
  unsigned int      allocated;
  unsigned int      count;
  char            **names;  /* Name of the states */
  struct STM_S     *states; /* Array of state structs */
  int              *alive;  /* Is the state alive or not (create/destroy) */
  int               queued;
  char             *queue_name;
  int               queue_flags;
};

/*
 * Manager Functions
 *
 * create    | Create a stack
 * destroy   | Destroy a stack
 * push      | Push a state on the stack
 * pop       | Pop a state off the stack
 * pop_until | Pop until a state is found, if not found pops all states
 * update    | Game loop update the state
 * draw      | Game loop draw the state
 * queue     | Queue a sate to be switched
 * switch    | Actually switch states
 */
int STM_m_create   (struct STM_M *manager, struct STM_T *table);
int STM_m_destroy  (struct STM_M *manager);
int STM_m_push     (struct STM_M *manager, const char *name, STM_S_Constructor constructor, uint8_t flags);
int STM_m_pop      (struct STM_M *manager);
int STM_m_pop_until(struct STM_M *manager, const char *name);
int STM_m_update   (struct STM_M *manager);
int STM_m_draw     (struct STM_M *manager);
int STM_m_queue    (struct STM_M *manager, const char *name, uint8_t flags);
int STM_m_switch   (struct STM_M *manager);

#endif
