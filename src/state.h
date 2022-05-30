#ifndef STM_STATE_H
#define STM_STATE_H

#include <stdint.h>
#include <stdlib.h>

/*
 * STATE
 *
 * A game state, like menu, game, pause
 *
 * States have updates and draws, input is not
 * handled
 *
 * The actual functions for create and destroy
 * are for initializing and destroying the struct
 * (this is handled by the table), while the 
 * callbacks are for when they are created and
 * destroyed on the stack.
 *
 * The state data should persist in memory as long as
 * the state is alive. If you need to communicate
 * data between states, a resource is probably needed.
 *
 * The create callback is called on a push,
 * and the destroy callback is called on a pop
 */
struct STM_S;

typedef int (*STM_S_Constructor)(struct STM_S *);

typedef int (STM_create) (struct STM_S *);
typedef int (STM_destroy)(struct STM_S *);
typedef int (STM_update) (struct STM_S *);
typedef int (STM_draw)   (struct STM_S *);

enum STM_F {
  STM_PERSIST_PREV = 0x01, /* Keep previous state alive */
  STM_DRAW_PREV    = 0x02, /* Draw the previous state */
  STM_FLAG_3       = 0x04, /* Unused */
  STM_FLAG_4       = 0x08, /* Unused */
  STM_FLAG_5       = 0x10, /* Unused */
  STM_FLAG_6       = 0x20, /* Unused */
  STM_FLAG_7       = 0x40, /* Unused */
  STM_FLAG_8       = 0x80, /* Unused */
};

struct STM_S {
  void        *data;       /* buffer of data */
  uint8_t      flags;      /* Bit array of flags */
  STM_create  *fn_create;  /* Function when state is created */
  STM_destroy *fn_destroy; /* Function when state is destroyed */
  STM_update  *fn_update;  /* How to update in game loop */
  STM_draw    *fn_draw;    /* How to draw in game loop */
};

/*
 * State functions
 *
 * create  | Create a state
 * destroy | Destroys/cleans up a state
 */
int STM_s_create (struct STM_S *state, uint8_t flags, STM_create fn_create, STM_destroy fn_destroy, STM_update fn_update, STM_draw fn_draw);
int STM_s_destroy(struct STM_S *state);

#endif
