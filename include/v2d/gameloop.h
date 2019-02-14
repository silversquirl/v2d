/* v2d/gameloop.h
 *
 * A game loop is a core part of any game engine. v2d provides a simple default
 * implementation, but does not require you to use it. You may write your own custom
 * game loop if you wish to, and can use the primitives defined in this header file
 * to ease the process.
 *
 */
#ifndef _V2D_GAMELOOP_H
#define _V2D_GAMELOOP_H

#include "v2d.h"

// Built-in game loop

struct v2d_gameloop_config {
	// The world to update and render
	const v2d_world_t *world;

	// The renderer to use for rendering the world
	v2d_render_t *render;

	// The action dispatcher to use for event handling
	v2d_action_dispatcher_t dis;

	// The game loop will exit when this is activated
	const v2d_action_t *quit_action;

	// The time to wait between rendering frames
	// The world may continue to be updated during this time
	unsigned int frame_time_ms; // 1000/FPS
};

v2d_gameloop_config_t v2d_gameloop_config_default(void);
void v2d_gameloop(v2d_gameloop_config_t conf);

// Game loop primitives for defining your own

// Process SDL events, dispatches to dis if dis is not NULL and returns whether the game should exit
_Bool v2d_loop_process_events(v2d_action_dispatcher_t dis, const v2d_action_t *quit_action, v2d_render_t *render);

// Update every entity in a world
void v2d_loop_update_world(const v2d_world_t *world, double dt);

// Render a world using the specified renderer
void v2d_loop_render_world(const v2d_world_t *world, v2d_render_t *render);

#endif
