/* v2d/error.h
 *
 * This file deals with error handling for v2d
 *
 */
#ifndef _V2D_ERROR_H
#define _V2D_ERROR_H

enum v2d_error {
	V2D_ERROR_NONE = 0,
	V2D_ERROR_SDL,
};

extern enum v2d_error v2d_errcode;
extern const char *v2d_errtext;

void v2d_raise_error(enum v2d_error code, const char *text);

#endif
