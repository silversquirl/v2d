#include <complex.h>
#include <math.h>
#include "v2d/vector.h"
#include "v2d/transform.h"

static inline v2d_vec_t cis(double theta) {
	return v2d_vec(cos(theta), sin(theta));
}

double v2d_vec_mag2(v2d_vec_t v) {
	double x = creal(v), y = cimag(v);
	return x*x + y*y;
}

v2d_vec_t v2d_vec_norm(v2d_vec_t v) {
	if (v == 0) return 0;
	return v / v2d_vec_mag(v);
}

v2d_vec_t v2d_vec_dot(v2d_vec_t a, v2d_vec_t b) {
	return creal(a)*creal(b) + cimag(a)*cimag(b);
}

v2d_vec_t v2d_vec_rotate(v2d_vec_t v, double theta) {
	return v * cis(theta);
}

// --- Implementation of transform.h ---

v2d_transform_t v2d_transform_new(void) {
	return (v2d_transform_t){1, 0};
}

v2d_transform_t v2d_tr_invert(v2d_transform_t tr) {
	v2d_vec_t mul = 1/tr.mul, add = -tr.add * mul;
	return (v2d_transform_t){mul, add};
}

v2d_transform_t v2d_tr_compose(v2d_transform_t a, v2d_transform_t b) {
	return (v2d_transform_t){b.mul * a.mul, b.mul*a.add + b.add};
}

void v2d_tr_rotate(v2d_transform_t *tr, double theta) {
	double _Complex mul = cis(theta);
	tr->mul *= mul;
	tr->add *= mul;
}

void v2d_tr_scale(v2d_transform_t *tr, double sf) {
	tr->mul *= sf;
	tr->add *= sf;
}

void v2d_tr_translate(v2d_transform_t *tr, v2d_vec_t delta) {
	tr->add += delta;
}
