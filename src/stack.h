#ifndef STM_STACK_H
#define STM_STACK_H

#include "state.h"
#include <stdlib.h>

/*
 * STACK
 *
 * Stack of the states, the actual manager of the states
 *
 * You may need to update the update and draw functions
 * depending on your needs
 */

struct STM_Stack {
  size_t             allocated;
  size_t             count;
  struct STM_State **states;
};

/*
 * Stack Functions
 *
 * create  | Create a stack
 * destroy | Destroy a stack
 * push    | Push a state on the stack
 * pop     | Pop a state off the stack
 * update  | Game loop update the state
 * draw    | Game loop draw the state
 */
int STM_stack_create (struct STM_Stack *stack);
int STM_stack_destroy(struct STM_Stack *stack);
int STM_stack_push   (struct STM_Stack *stack, struct STM_State *state);
int STM_stack_pop    (struct STM_Stack *stack);
int STM_stack_update (struct STM_Stack *stack);
int STM_stack_draw   (struct STM_Stack *stack);

#endif
