/* v2d/world.h
 *
 * v2d's world type provides a system for managing a collection of entities in an
 * efficient manner. It can be used with the render component or as part of a
 * broad-phase collision detection system.
 *
 */
#ifndef _V2D_WORLD_H
#define _V2D_WORLD_H

#include "v2d.h"

struct v2d_world_entity_list {
	v2d_ent_cb_t *ent;
	struct v2d_world_entity_list *next;
};

struct v2d_world {
	struct v2d_world_entity_list *entities;
};

// Creates a new world
v2d_world_t *v2d_world_new(void);

// Frees a world and calls the destructors of any entities it contains
void v2d_world_free(v2d_world_t *world);

// Adds an entity to the world. No checks are made for duplicate entities
void v2d_world_add_entity(v2d_world_t *world, v2d_ent_t *entity);

// Removes an entity from the world
// This requires a traversal of the entire entity list until the desired object is found, which takes O(n) time
// Returns true on success, false if the object was not found in the world
_Bool v2d_world_del_entity(v2d_world_t *world, v2d_ent_t *entity);

// Used in a for loop to loop through all the entities in a world
// Usage:
//  for (struct my_entity_type *v2d_world_iterate(ent, world)) {
//      do_thing_with_entity(ent);
//  }
#define v2d_world_iterate(var, world) (var), *_v2d_world_iterate_list = (void *)((world)->entities); _v2d_world_iterate_list && ((var) = (void *)(((struct v2d_world_entity_list *)_v2d_world_iterate_list)->ent)); _v2d_world_iterate_list = (void *)(((struct v2d_world_entity_list *)_v2d_world_iterate_list)->next)

#endif
