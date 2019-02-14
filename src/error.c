#include "v2d/error.h"

enum v2d_error v2d_errcode = V2D_ERROR_NONE;
const char *v2d_errtext = 0;

void v2d_raise_error(enum v2d_error code, const char *text) {
	v2d_errcode = code;
	v2d_errtext = text;
}
