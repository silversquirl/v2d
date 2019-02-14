/* v2d.h
 *
 * This header file includes all other v2d headers and provides forward declarations
 * of a number of typedefs.
 *
 * External programs should always include only this header, and never any of those
 * inside the v2d/ directory. Those files are for internal use only.
 *
 * Any header files not included in this one are not part of the public API and
 * should not be used externally.
 *
 */
#ifndef _V2D_H
#define _V2D_H

// Forward declaration typedefs for all the things
typedef struct v2d_action v2d_action_t;
typedef struct v2d_action_trigger v2d_action_trigger_t;
typedef struct v2d_action_dispatcher v2d_action_dispatcher_t;
typedef void v2d_ent_t;
typedef struct v2d_ent_cb v2d_ent_cb_t;
typedef struct v2d_gameloop_config v2d_gameloop_config_t;
typedef struct v2d_world v2d_world_t;
typedef struct v2d_obj v2d_obj_t;
typedef struct v2d_render v2d_render_t;

#include "v2d/action.h"
#include "v2d/collide.h"
#include "v2d/entity.h"
#include "v2d/error.h"
#include "v2d/gameloop.h"
#include "v2d/render.h"
#include "v2d/transform.h"
#include "v2d/vector.h"
#include "v2d/warn.h"
#include "v2d/world.h"

#endif
