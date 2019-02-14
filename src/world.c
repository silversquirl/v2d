#include <stdbool.h>
#include "v2d.h"

v2d_world_t *v2d_world_new(void) {
	v2d_world_t *world = malloc(sizeof *world);
	world->entities = NULL;
	return world;
}

void v2d_world_free(v2d_world_t *world) {
	if (!world) return;

	struct v2d_world_entity_list *l = world->entities;
	while (l) {
		struct v2d_world_entity_list *next = l->next;
		if (l->ent->destroy) l->ent->destroy(l->ent);
		free(l);
		l = next;
	}
}

void v2d_world_add_entity(v2d_world_t *world, v2d_ent_t *entity) {
	struct v2d_world_entity_list *head = world->entities;
	world->entities = malloc(sizeof *world->entities);
	world->entities->ent = entity;
	world->entities->next = head;
}

_Bool v2d_world_del_entity(v2d_world_t *world, v2d_ent_t *entity) {
	struct v2d_world_entity_list *l = world->entities, *prev = NULL;
	while (l) {
		if (l->ent == entity) {
			if (prev) {
				prev->next = l->next;
			} else {
				world->entities = l->next;
			}
			free(l);
			return true;
		}
	}
	return false;
}
