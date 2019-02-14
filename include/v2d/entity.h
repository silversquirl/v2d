/* v2d/entity.h
 *
 * Entities are the core game object in v2d. They are updated at least once
 * per frame and can render themselves to the screen.
 *
 * The type (v2d_ent_t *) is actually just a void pointer, meaning that any
 * pointer type can be cast to it. This is so that a struct can have a first
 * field of type v2d_ent_cb_t and be seamlessly passed to functions
 * requiring values of type (v2d_ent_t *)
 *
 */
// NOTE: since this is just a type definition, there is no associated source file
#ifndef _V2D_ENTITY_H
#define _V2D_ENTITY_H

#include "v2d.h"

typedef void (*v2d_entity_render_callback_t)(v2d_ent_t *entity, v2d_render_t *render);
typedef void (*v2d_entity_update_callback_t)(v2d_ent_t *entity, double dt);
typedef void (*v2d_entity_destroy_callback_t)(v2d_ent_t *entity);

struct v2d_ent_cb {
	v2d_entity_render_callback_t render;
	v2d_entity_update_callback_t update;
	v2d_entity_destroy_callback_t destroy;
};

#endif
