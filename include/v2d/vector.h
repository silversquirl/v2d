/* v2d/vector.h
 * 
 * WARNING: Only multiply/divide two vectors if you know what you're doing
 *
 * Vectors are an important part of any graphics system. v2d provides a
 * 2D vector type implemented as a double precision floating point complex
 * number. v2d also provides implementations of some common vector
 * operations for this type.
 *
 * The reason complex numbers are used is that C implements basic
 * arithmetic operaions for them. This means you can add or subtract vectors
 * just like you would regular numbers. You can also multiply or divide them
 * by scalars and get the expected results. However, multiplying or dividing
 * two vectors will give a result with complex number semantics. This may or
 * may not be what you want, so bear that in mind.
 * 
 * A number of the macros defined in this header are simple wrappers for
 * functions and macros defined in `complex.h`. If you wish, you may use
 * the standard library functions instead.
 *
 */
#ifndef _V2D_VECTOR_H
#define _V2D_VECTOR_H

#include <complex.h>

typedef double _Complex v2d_vec_t;

// Create a vector from X and Y components
#if defined(CMPLX)
#define v2d_vec(x, y) (CMPLX((x), (y)))
#else
#define v2d_vec(x, y) ((x) + (y)*I)
#endif

// Fast macros with short names because we do this a lot
// Return the X component of a vector
#define v2dvx(v) (creal(v))
// Return the Y component of a vector
#define v2dvy(v) (cimag(v))

// Convert a vector to a pointer to two doubles
#define v2d_vec_a(v) ((double *)(&(v))) // This is well-defined by C99 - see section 6.2.5, point 13

// Return an lvalue referencing the X component of a vector
#define v2d_vec_x(v) (v2d_vec_a(v)[0])

// Return an lvalue referencing the Y component of a vector
#define v2d_vec_y(v) (v2d_vec_a(v)[1])

// Return the X or Y component of a vector depending on idx
#define v2d_vec_idx(v, i) ((i) ? cimag(v) : creal(v))

// Calculate the square of a vector's magnitude
double v2d_vec_mag2(v2d_vec_t v);

// Calculate a vector's magnitude
#define v2d_vec_mag(v) (cabs(v))

// Normalize a vector
v2d_vec_t v2d_vec_norm(v2d_vec_t v);

// Calculate the dot product of the two vectors
v2d_vec_t v2d_vec_dot(v2d_vec_t a, v2d_vec_t b);

// Rotate a vector by theta radians
// If you want rotate multiple vectors by the same theta, you should
// use v2d_tr_rotate and v2d_transform instead.
v2d_vec_t v2d_vec_rotate(v2d_vec_t v, double theta);

#endif
