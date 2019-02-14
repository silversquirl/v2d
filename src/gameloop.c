#include <stdbool.h>
#include <stdint.h>
#include "v2d.h"

v2d_gameloop_config_t v2d_gameloop_config_default(void) {
	return (v2d_gameloop_config_t){NULL, NULL, {0}, NULL, 1000/60};
}

void v2d_gameloop(v2d_gameloop_config_t conf) {
	uint32_t tnow, told = SDL_GetTicks(), tnext = told + conf.frame_time_ms;

	while (v2d_loop_process_events(conf.dis, conf.quit_action, conf.render)) {
		// Update the world until it's time to render the next frame
		while (!SDL_TICKS_PASSED((tnow = SDL_GetTicks()), tnext)) {
			v2d_loop_update_world(conf.world, (tnow - told) / 1000.0);
			told = tnow;
		}

		// Render everything
		v2d_loop_render_world(conf.world, conf.render);

		// Set the time for the next frame to be rendered
		tnext += conf.frame_time_ms;
	}
}

bool v2d_loop_process_events(v2d_action_dispatcher_t dis, const v2d_action_t *quit_action, v2d_render_t *render) {
		SDL_Event ev;
		while (SDL_PollEvent(&ev)) {
			if (ev.type == SDL_QUIT) return false;
			if (render && ev.type == SDL_WINDOWEVENT && ev.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
				v2d_render_transform_center(render, ev.window.data1, ev.window.data2);
				continue;
			}
			v2d_adis_handle_event(dis, ev, v2d_render_transform(render));
		}
		if (quit_action && quit_action->value.s) return false;
		return true;
}

void v2d_loop_update_world(const v2d_world_t *world, double dt) {
	if (!world) return;
	struct v2d_world_entity_list *l = world->entities;
	for (; l; l = l->next) {
		if (l->ent->update) l->ent->update(l->ent, dt);
	}
}

void v2d_loop_render_world(const v2d_world_t *world, v2d_render_t *render) {
	if (!render) return;

	v2d_render_rgb(render, 0, 0, 0);
	v2d_render_clear(render);

	if (world) {
		struct v2d_world_entity_list *l = world->entities;
		for (; l; l = l->next) {
			if (l->ent->render) l->ent->render(l->ent, render);
		}
	}

	v2d_render_flip(render);
}
