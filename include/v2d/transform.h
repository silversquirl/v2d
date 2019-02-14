/* v2d/transform.h
 *
 * This file implements coordinate transformations using a pair of
 * complex numbers. Any translation, rotation or scale of a complex
 * number b can be represented using the form `ab + c` where a and c
 * are complex numbers.
 * 
 */
// Note: this file's implementation is in vector.c not transform.c
#ifndef _V2D_TRANSFORM_H
#define _V2D_TRANSFORM_H

#include "v2d/vector.h"

typedef struct {
	double _Complex mul, add;
} v2d_transform_t;

v2d_transform_t v2d_transform_new(void);

// Transform a vector
#define v2d_transform(v, tr) ((v)*(tr).mul + (tr).add)

// Take the inverse of a transformation
// v2d_transform(v2d_transform(v, tr), v2d_tr_invert(tr)) == v
v2d_transform_t v2d_tr_invert(v2d_transform_t tr);

// Compose two transformations into one
// The result is a transformation that transforms by a and then by b
v2d_transform_t v2d_tr_compose(v2d_transform_t a, v2d_transform_t b);

void v2d_tr_rotate(v2d_transform_t *tr, double theta);
void v2d_tr_scale(v2d_transform_t *tr, double sf);
void v2d_tr_translate(v2d_transform_t *tr, v2d_vec_t delta);

#endif
