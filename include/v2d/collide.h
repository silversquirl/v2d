/* v2d/collide.h
 *
 * Collision detection is a pretty important part of most games. This file
 * implements some basic collision primitives that should allow you to model
 * most situations with a decent amount of accuracy.
 *
 * This file only implements narrow-phase collision detection.
 *
 */
#ifndef _V2D_COLLIDE_H
#define _V2D_COLLIDE_H

#include "v2d/vector.h"

// Basic collision types

typedef struct {
	v2d_vec_t pos;
	double rad;
} v2d_circle_t;

typedef struct {
	v2d_vec_t pos, dim;
} v2d_rect_t;

typedef struct {
	v2d_vec_t pos, dir;
} v2d_ray_t;

// Shape-to-shape collision
// These functions return true if there is a collision and false otherwise

_Bool v2d_collide_circle_circle(v2d_circle_t a, v2d_circle_t b);
_Bool v2d_collide_rect_rect(v2d_rect_t a, v2d_rect_t b);
_Bool v2d_collide_circle_rect(v2d_circle_t a, v2d_rect_t b);
#define v2d_collide_rect_circle(b, a) (v2d_collide_circle_rect((a), (b)));

// Ray-to-shape collision
// Returns the distance along the line as a fraction, or an infinite value when there is no collision

double v2d_raycast_circle(v2d_ray_t r, v2d_circle_t c);
double v2d_raycast_rect(v2d_ray_t r, v2d_rect_t b);

#endif
