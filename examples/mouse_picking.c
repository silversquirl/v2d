/*
 * This example demonstrates how to use v2d's collision detection for
 * picking objects using the mouse.
 *
 * For an example demonstrating how to use this to add drag-and-drop
 * functionality, see collide.c
 */
#include <stdbool.h>
#include <stdio.h>
#include <v2d.h>

#define EX_NAME "v2d mouse picking"

v2d_action_t actions[] = {
	{"mouse", {0}},
	{0},
};

struct shape {
	v2d_ent_cb_t cb;

	v2d_action_t *mouse;
	bool colliding;

	enum {
		SHAPE_RECT,
		SHAPE_CIRCLE,
	} type;
	union {
		v2d_rect_t rect;
		v2d_circle_t circ;
	} shape;
};

static bool shape_picked(struct shape *s) {
	switch (s->type) {
	case SHAPE_RECT:
		return v2d_collide_point_rect(s->mouse->value.pos, s->shape.rect);

	case SHAPE_CIRCLE:
		return v2d_collide_point_circle(s->mouse->value.pos, s->shape.circ);
	}
	return false;
}

static void shape_render(v2d_ent_t *ent, v2d_render_t *ren) {
	struct shape *s = ent;

	if (s->colliding) v2d_render_rgb(ren, 1, 0.3, 0.7);
	else v2d_render_rgb(ren, 1, 1, 1);

	switch (s->type) {
	case SHAPE_RECT:
		v2d_render_draw_rect(ren, s->shape.rect.pos, s->shape.rect.dim);
		break;

	case SHAPE_CIRCLE:
		v2d_render_draw_circle(ren, s->shape.circ.pos, s->shape.circ.rad);
		break;
	}
}

static void shape_update(v2d_ent_t *ent, double dt) {
	struct shape *s = ent;
	s->colliding = shape_picked(s);
}

int ex_main(SDL_Window *win) {
	v2d_action_dispatcher_t dis = v2d_adis_create(actions, NULL, NULL, &actions[0]);
	v2d_world_t *world = v2d_world_new();

#define SHAPE_rect SHAPE_RECT
#define SHAPE_circ SHAPE_CIRCLE
#define SHAPE(type, ...) {{shape_render, shape_update, NULL}, &actions[0], false, SHAPE_##type, {.type = {__VA_ARGS__}}}

	struct shape rect1 = SHAPE(rect, v2d_vec(2, -1), v2d_vec(2, 1));
	v2d_world_add_entity(world, &rect1);

	struct shape rect2 = SHAPE(rect, v2d_vec(-1, 1), v2d_vec(2, 3));
	v2d_world_add_entity(world, &rect2);

	struct shape circ1 = SHAPE(circ, v2d_vec(-1, 1), 1);
	v2d_world_add_entity(world, &circ1);

	struct shape circ2 = SHAPE(circ, v2d_vec(1, -1), 0.4);
	v2d_world_add_entity(world, &circ2);

	v2d_render_t *render = v2d_render_new(win);
	if (!render) {
		fprintf(stderr, "v2d_render_new: %s\n", v2d_errtext);
		return 1;
	}

	v2d_gameloop_config_t conf = v2d_gameloop_config_default();
	conf.world = world;
	conf.render = render;
	conf.dis = dis;
	v2d_gameloop(conf);

	v2d_render_free(render);
	v2d_world_free(world);

	return 0;
}

#include "example_boilerplate.h"
