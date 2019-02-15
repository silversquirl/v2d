/*
 * This example demonstrates v2d's ray-to-shape collision detection
 * 
 * A lot of this code is copied from collide.c
 */
#include <stdbool.h>
#include <stdio.h>
#include <v2d.h>

#define EX_NAME "v2d ray casting"

v2d_action_t actions[] = {
	{"click", {0}},
	{"mouse", {0}},
	{0}
};

v2d_action_trigger_t triggers[] = {
	V2D_MOUSE_TRIG(SDL_BUTTON_LEFT, 1, &actions[0]),
	{0}
};

struct shape {
	v2d_ent_cb_t cb;
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

struct ray {
	v2d_ent_cb_t cb;
	v2d_action_t *click, *mouse;
	v2d_world_t *world;
	v2d_ray_t ray;
};

static bool shape_raycast(struct ray *r, struct shape *s) {
	switch (s->type) {
	case SHAPE_RECT:
		return isfinite(v2d_raycast_rect(r->ray, s->shape.rect));
	case SHAPE_CIRCLE:
		return isfinite(v2d_raycast_circle(r->ray, s->shape.circ));
	}
	return false;
}

static void ray_render(v2d_ent_t *ent, v2d_render_t *ren) {
	struct ray *r = ent;

	v2d_render_rgb(ren, 1, 1, 1);
	v2d_render_draw_line(ren, r->ray.pos, r->ray.dir);
}

static void ray_update(v2d_ent_t *ent, double dt) {
	struct ray *r = ent;

	// Update ray coordinates
	if (r->click->value.s) {
		r->ray.pos = r->mouse->value.pos;
		r->ray.dir = 0;
	} else {
		r->ray.dir = r->mouse->value.pos - r->ray.pos;
	}

	// Detect collisions
	for (struct shape *v2d_world_iterate(ent, r->world)) {
		if ((void *)ent == r) continue;
		ent->colliding = shape_raycast(r, ent);
	}
}

int ex_main(SDL_Window *win) {
	v2d_action_dispatcher_t dis = v2d_adis_create(actions, triggers, NULL, &actions[1]);
	v2d_world_t *world = v2d_world_new();

#define SHAPE_rect SHAPE_RECT
#define SHAPE_circ SHAPE_CIRCLE
#define SHAPE(type, ...) {{shape_render, NULL, NULL}, false, SHAPE_##type, {.type = {__VA_ARGS__}}}

	struct shape rect1 = SHAPE(rect, v2d_vec(2, -1), v2d_vec(2, 1));
	v2d_world_add_entity(world, &rect1);

	struct shape rect2 = SHAPE(rect, v2d_vec(-1, 1), v2d_vec(2, 3));
	v2d_world_add_entity(world, &rect2);

	struct shape circ1 = SHAPE(circ, v2d_vec(-1, -1), 1);
	v2d_world_add_entity(world, &circ1);

	struct shape circ2 = SHAPE(circ, v2d_vec(1, -1), 0.4);
	v2d_world_add_entity(world, &circ2);

	struct ray ray = {
		{ray_render, ray_update, NULL},
		&actions[0], &actions[1],
		world,
		{0, 0},
	};
	v2d_world_add_entity(world, &ray);

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
