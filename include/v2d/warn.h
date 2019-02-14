#ifndef _V2D_WARN_H
#define _V2D_WARN_H

#ifdef V2D_DEBUG
#include <stdio.h>
#define v2d_warn(...) (fputs("v2d: WARNING: ", stderr), fprintf(stderr, __VA_ARGS__), fputc('\n', stderr))
#else
#define v2d_warn(...)
#endif

#endif
