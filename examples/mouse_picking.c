/*
 * This example demonstrates how to use v2d's collision functionality for
 * picking objects using the mouse.
 */
#include <v2d.h>

#define EX_NAME "v2d mouse picking"

v2d_action_t actions[] = {
	{"mouse", {0}},
	{0},
};

struct rect {
	v2d_ent_cb_t cb;
	v2d_action_t *mouse;
	v2d_rect_t rect;
};

static void rect_render(v2d_ent_t *ent, v2d_render_t *ren) {
	struct rect *r = ent;

	double l = 0.7;
	if (v2d_collide_point_rect(r->mouse->value.pos, r->rect)) l = 1;

	v2d_render_rgb(ren, l, l, l);
	v2d_render_draw_rect(ren, r->rect.pos, r->rect.dim);
}

#if 0
struct circle {
	v2d_ent_cb_t cb;
	v2d_action_t *mouse;
	v2d_vec_t pos;
	double rad;
};

static void circle_render(v2d_ent_t *ent, v2d_render_t *ren) {
	struct circle *c = ent;

	double l = 0.7;
	if (c->mouse->value.s) l = 1;

	v2d_render_rgb(ren, l, l, l);
	v2d_render_draw_circle(ren, c->pos, r->rad);
}
#endif

int ex_main(SDL_Window *win) {
	v2d_action_dispatcher_t dis = v2d_adis_create(actions, NULL, NULL, &actions[0]);
	v2d_world_t *world = v2d_world_new();

	struct rect rect_a = {
		{rect_render, NULL, NULL},
		&actions[0],
		{v2d_vec(2, -1), v2d_vec(1, 1)},
	};
	v2d_world_add_entity(world, &rect_a);

	struct rect rect_b = {
		{rect_render, NULL, NULL},
		&actions[0],
		{v2d_vec(-1, 1), v2d_vec(1, 1)},
	};
	v2d_world_add_entity(world, &rect_b);

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
