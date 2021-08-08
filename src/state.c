#include "state.h"
#include <stdlib.h>

/*
 * STM_state_create
 *
 * @desc
 *   Creates a state
 */
int STM_state_create(struct STM_State *state, const char *name, size_t stack_size, uint8_t flags, STM_create fn_create, STM_destroy fn_destroy, STM_update fn_update, STM_draw fn_draw)
{
  state->count = stack_size;
  if (!(state->data  = malloc(stack_size * sizeof(STM_Data)))) return -1;
  for (int i = 0; i < 16; ++i) {
    state->name[i] = *name++;
    if (!*name) break;
  }
  state->flags      = flags;
  state->fn_create  = fn_create;
  state->fn_destroy = fn_destroy;
  state->fn_update  = fn_update;
  state->fn_draw    = fn_draw;

  return 0;
}

/*
 * STM_state_destroy
 *
 * @desc
 *   Destroys a state
 */
int STM_state_destroy(struct STM_State *state)
{
  free(state->data);

  return 0;
}
