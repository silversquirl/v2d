/* v2d/render.h
 *
 * This file contains header information for the rendering component of v2d.
 * Rendering uses SDL, and you can tap into this directly if you wish to.
 * The render component takes care of initializing SDL, as well as
 * dispatching render events to multiple game entities.
 *
 */

#ifndef _V2D_RENDER_H
#define _V2D_RENDER_H

#include <SDL2/SDL.h>
#include "v2d.h"
#include "v2d/world.h"
#include "v2d/transform.h"
#include "v2d/vector.h"

struct v2d_render {
	SDL_Renderer *sdl_ren;
	// camera_tr converts v2d world coordinates to v2d screen coordinates
	// screen_tr converts v2d screen coordinates with inverted y to SDL screen coordinates
	// To convert from world coordinates to SDL coordinates, do v2d_transform(conj(world_pos), v2d_render_transform(render))
	v2d_transform_t camera_tr, screen_tr;
};

// Create a new renderer for the specified SDL window
// The default transformation converts 1 game unit into 64 pixels and centers the game coordinates on screen.
// In order to maintain this transformation if the window is resized, you must handle the correct events. The default game loop does this automatically.
v2d_render_t *v2d_render_new(SDL_Window *sdl_win);

// Free a renderer and all resources associated with it
void v2d_render_free(v2d_render_t *render);

// Set a renderer's transformation to center the origin on screen
void v2d_render_transform_center(v2d_render_t *render, double width, double height);

// Create a transformation that will convert from conjugated world coordinates into SDL screen coordinates
v2d_transform_t v2d_render_transform(v2d_render_t *render);

// Set a renderer's current colour. Values range from 0 to 1
void v2d_render_rgb(v2d_render_t *render, double r, double g, double b);

// Set a renderer's current colour and opacity. Values range from 0 to 1
void v2d_render_rgba(v2d_render_t *render, double r, double g, double b, double a);

// Clear the screen
void v2d_render_clear(v2d_render_t *render);

// Swap the front and back buffers
void v2d_render_flip(v2d_render_t *render);

// Draw an axis-aligned rectangle
// WARNING: any rotational transformations applied to the camera will only apply to the corners of this rectangle, and will not rotate the rectangle itself
// `pos` is the vector from origin to the bottom left corner
// `size` is the vector from the bottom left to the top right corner
void v2d_render_draw_rect(v2d_render_t *render, v2d_vec_t pos, v2d_vec_t size);

// Draw a single pixel
void v2d_render_draw_pixel(v2d_render_t *render, v2d_vec_t pos);

// Draw a line starting at pos and ending at pos+dir
void v2d_render_draw_line(v2d_render_t *render, v2d_vec_t pos, v2d_vec_t dir);

// Draw a circle using the midpoint circle algorithm
void v2d_render_draw_circle(v2d_render_t *render, v2d_vec_t center, double radius);

#endif
