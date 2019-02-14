/* v2d/action.h
 *
 * Actions are the way user input is handled in v2d. When a user presses a
 * key, clicks the mouse or interacts with a controller, they may trigger an
 * action. Entities can then respond to this action when they are updated.
 *
 */
#ifndef _V2D_ACTION_H
#define _V2D_ACTION_H

#include <stdint.h>
#include <SDL2/SDL.h>
#include "v2d.h"
#include "v2d/vector.h"
#include "v2d/transform.h"

// This struct defines an action
// Entities should read the `value` field to determine what to do during their update step.
// `value` is a vector. For triggers it will be (0, 0) or (1, 0). For controller vectors it will be between (-1, -1) and (1, 1). For mouse vectors it can be anything.
//  `value.s` is the x component of that vector. This can be used like a boolean for trigger actions
//  `value.pos` is the vector itself
//  `value.xy` is an array of 2 doubles: the x component followed by the y component
struct v2d_action {
	const char *id;
	union {
		double s;
		v2d_vec_t pos;
		double xy[2];
	} value;
};

// For information on these 3 structs, see the SDL event documentation
struct v2d_action_trigger_key {
	SDL_Keycode sym;
	uint16_t mod;
};
struct v2d_action_trigger_mouse {
	uint8_t button;
	uint8_t clicks;
};
struct v2d_action_trigger_controller {
	uint8_t button;
};

// This struct represents a binary input
// It could be a keypress, a controller button, a mouse click, etc.
struct v2d_action_trigger {
	uint8_t type;

	union {
		struct v2d_action_trigger_key key;
		struct v2d_action_trigger_mouse mouse;
		struct v2d_action_trigger_controller controller;
	} trig;

	v2d_action_t *action;
};

enum v2d_action_trigger_type {
	V2D_ACTION_TRIG_END = 0,
	V2D_ACTION_TRIG_KEY,
	V2D_ACTION_TRIG_MOUSE,
	V2D_ACTION_TRIG_CONTROLLER,
};

// These macros are designed to ease the construction of trigger literals

// Create a key trigger
#define V2D_KEY_TRIG(sym, mod, action) {V2D_ACTION_TRIG_KEY, {.key = {(sym), (mod)}}, action}
// Create a key trigger for use as an expression
#define V2D_KEY_TRIG_LIT(sym, mod, action) ((struct v2d_action_trigger){V2D_ACTION_TRIG_KEY, {.key = {(sym), (mod)}}, action})

// Create a mouse trigger
#define V2D_MOUSE_TRIG(btn, nclick, action) {V2D_ACTION_TRIG_MOUSE, {.mouse = {(btn), (nclick)}}, action}
// Create a mouse trigger for use as an expression
#define V2D_MOUSE_TRIG_LIT(btn, nclick, action) ((struct v2d_action_trigger){V2D_ACTION_TRIG_MOUSE, {.mouse = {(btn), (nclick)}}, action})

// Create a controller trigger
#define V2D_CONTRL_TRIG(btn, action) {V2D_ACTION_TRIG_CONTROLLER, {.controller = {(btn)}}, action}
// Create a controller trigger for use as an expression
#define V2D_CONTRL_TRIG_LIT(btn, action) ((struct v2d_action_trigger){V2D_ACTION_TRIG_CONTROLLER, {.controller = {(btn)}}, action})

// This struct represents an axis input
// You can choose which axis of the vector to modify by setting `xy` to 0 or 1
// For more info see SDL's docs for SDL_ControllerAxisEvent
struct v2d_action_axis {
	uint8_t axis;

	double dead; // If -dead < value < dead, value is set to 0
	unsigned xy : 1;

	v2d_action_t *action;
};

#define V2D__AXIS_x 0
#define V2D__AXIS_X 0
#define V2D__AXIS_y 1
#define V2D__AXIS_Y 1
// This macro provides defaults for some axis values, and also allows you to specify `x` or `y` instead of `0` or `1`
#define V2D_AXIS(axis, xy, action) ((struct v2d_action_axis){axis, 0.00001, V2D__AXIS_##xy, action})

struct v2d_action_dispatcher {
	v2d_action_t *actions;
	size_t n_actions;

	struct v2d_action_trigger *triggers;
	size_t n_triggers;

	struct v2d_action_axis *axes;
	size_t n_axes;

	v2d_action_t *mouse;
};

// Creates an action dispatcher
// `actions` is an array of actions, terminated by an action with a NULL id
// `triggers` is an array of triggers, terminated by a V2D_ACTION_TRIG_END
// `axes` is an array of axes, terminated by an axis with `axis` set to -1
// `mouse` is a pointer to the action attached to the mouse or NULL
// WARNING: This will reorder the elements of its arguments to improve lookup speed.
// WARNING 2: This does not take ownership of its arguments, nor does it copy them. You must ensure they are in memory for the whole time that the dispatcher is, and must clean them up if necessary once the dispatcher is destroyed.
v2d_action_dispatcher_t v2d_adis_create(struct v2d_action *actions, struct v2d_action_trigger *triggers, struct v2d_action_axis *axes, v2d_action_t *mouse);

v2d_action_t *v2d_adis_find_action(v2d_action_dispatcher_t dis, const char *id);
v2d_action_t *v2d_adis_find_trigger_action(v2d_action_dispatcher_t dis, struct v2d_action_trigger trig);
struct v2d_action_axis *v2d_adis_find_axis(v2d_action_dispatcher_t dis, uint8_t axis);

// mouse_tr is the transformation that converts v2d coordinates into SDL coordinates. Its inverse will be used to convert mouse coordinates into game coordinates before they are stored in the action
void v2d_adis_handle_event(v2d_action_dispatcher_t dis, SDL_Event ev, v2d_transform_t mouse_tr);

#endif
