#ifndef STM_H
#define STM_H

#include <stdint.h>
#include <stdlib.h>

/*
 * STATE
 *
 * A game state, like menu, game, pause
 *
 * State Data are basically variables,
 * but instead of names you need to use
 * an index, make enums if you want names
 * 
 * The data is also just an array of
 * whatever data type you want, preferably
 * a uint type that you cast into others
 *
 * Access state data by:
 *   state.data[index]
 *
 * States have updates and draws, input is not
 * handled
 *
 * The actual functions for create and destroy
 * are for initializing and destroying the struct,
 * while the callbacks are for when they are
 * created and destroyed on the stack.
 *
 * The state data persists in memory as long as
 * the state manager is active, but other data
 * can be created dynamically with the create
 * and destroy callbacks
 *
 * The create callback is called on a push,
 * and the destroy callback is called on a pop
 */
typedef uint32_t STM_Data;

struct STM_State;

typedef int (STM_create) (struct STM_State *);
typedef int (STM_destroy)(struct STM_State *);
typedef int (STM_update) (struct STM_State *);
typedef int (STM_draw)   (struct STM_State *);

enum STM_Flags {
  STM_DRAW_PREV = 0x01, /* Draw the previous state */
  STM_FLAG_2    = 0x02, /* Unused */
  STM_FLAG_3    = 0x04, /* Unused */
  STM_FLAG_4    = 0x08, /* Unused */
  STM_FLAG_5    = 0x10, /* Unused */
  STM_FLAG_6    = 0x20, /* Unused */
  STM_FLAG_7    = 0x40, /* Unused */
  STM_FLAG_8    = 0x80, /* Unused */
};

struct STM_State {
  char         name[16];   /* Name of the state */
  size_t       count;      /* How much data in the state */
  STM_Data    *data;       /* uint32_t of data */
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
int STM_state_create (struct STM_State *state, const char *name, size_t stack_size, uint8_t flags, STM_create fn_create, STM_destroy fn_destroy, STM_update fn_update, STM_draw fn_draw);
int STM_state_destroy(struct STM_State *state);

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
