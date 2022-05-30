#include "state.h"
#include <stdlib.h>

/*
 * STM_s_create
 *
 * @desc
 *   Creates a state
 * @param state
 *   The state to create
 * @param flags
 *   The STM_Fs to activate
 * @param fn_create
 *   State creation function (on the stack)
 * @param fn_destroy
 *   State destroy function (on the stack)
 * @param fn_update
 *   State update function
 * @param fn_draw
 *   State draw function
 * @return
 *   0 on success
 */
int STM_s_create(struct STM_S *state, uint8_t flags, STM_create fn_create, STM_destroy fn_destroy, STM_update fn_update, STM_draw fn_draw)
{
  state->flags      = flags;
  state->fn_create  = fn_create;
  state->fn_destroy = fn_destroy;
  state->fn_update  = fn_update;
  state->fn_draw    = fn_draw;

  return 0;
}

/*
 * STM_s_destroy
 *
 * @desc
 *   Destroys a state. Used to do
 *   something but now is just for consistency
 *   with STM_s_create
 * @param state
 *   The state to destroy
 * @return
 *   0 on success
 */
int STM_s_destroy(struct STM_S *state)
{
  return 0;
}
