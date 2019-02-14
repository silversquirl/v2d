#include <complex.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "v2d/render.h"

// Transform v2d world coordinates to SDL screen coordinates
// We use complex conjugate because SDL has y coordinates increasing downwards, whereas v2d has them increasing upwards
static inline v2d_vec_t _tr(v2d_render_t *render, v2d_vec_t v) {
	return v2d_transform(conj(v), v2d_render_transform(render));
}

// Transform a v2d world size to an SDL screen size
static inline v2d_vec_t _tr_siz(v2d_render_t *r, v2d_vec_t v) {
	return r->screen_tr.mul * conj(r->camera_tr.mul * v);
}

v2d_render_t *v2d_render_new(SDL_Window *sdl_win) {
	SDL_Renderer *ren = SDL_CreateRenderer(sdl_win, -1, 0);
	if (!ren) {
		v2d_raise_error(V2D_ERROR_SDL, SDL_GetError());
		return NULL;
	}

	v2d_render_t *render = malloc(sizeof *render);
	render->sdl_ren = ren;

	render->camera_tr = v2d_transform_new();
	render->screen_tr = v2d_transform_new();
	v2d_tr_scale(&render->screen_tr, 64);

	int width, height;
	SDL_GetWindowSize(sdl_win, &width, &height);
	v2d_render_transform_center(render, width, height);

	return render;
}

void v2d_render_free(v2d_render_t *render) {
	if (!render) return;
	SDL_DestroyRenderer(render->sdl_ren);
	free(render);
}

void v2d_render_transform_center(v2d_render_t *render, double width, double height) {
	render->screen_tr.add = v2d_vec(width, height)/2;
}

v2d_transform_t v2d_render_transform(v2d_render_t *render) {
	v2d_transform_t cam = render->camera_tr, scr = render->screen_tr;
	cam.add = conj(cam.add); // Flip the Y translation because this needs to work on a conjugated vector
	return v2d_tr_compose(cam, scr);
}

void v2d_render_rgb(v2d_render_t *render, double r, double g, double b) {
	SDL_SetRenderDrawColor(render->sdl_ren, 255*r, 255*g, 255*b, 255);
}

void v2d_render_rgba(v2d_render_t *render, double r, double g, double b, double a) {
	SDL_SetRenderDrawColor(render->sdl_ren, 255*r, 255*g, 255*b, 255*a);
}

void v2d_render_clear(v2d_render_t *render) {
	SDL_RenderClear(render->sdl_ren);
}

void v2d_render_flip(v2d_render_t *render) {
	SDL_RenderPresent(render->sdl_ren);
}

void v2d_render_draw_rect(v2d_render_t *render, v2d_vec_t pos, v2d_vec_t size) {
	v2d_vec_t screen_pos = _tr(render, pos);
	v2d_vec_t screen_size = _tr_siz(render, size);
	SDL_Rect r = {
		v2dvx(screen_pos), v2dvy(screen_pos),
		v2dvx(screen_size), v2dvy(screen_size),
	};
	SDL_RenderDrawRect(render->sdl_ren, &r);
}

void v2d_render_draw_pixel(v2d_render_t *render, v2d_vec_t pos) {
	pos = _tr(render, pos);
	SDL_RenderDrawPoint(render->sdl_ren, v2dvx(pos), v2dvy(pos));
}

// Midpoint circle algorithm stolen from https://en.wikipedia.org/wiki/Midpoint_circle_algorithm#C_example
void v2d_render_draw_circle(v2d_render_t *render, v2d_vec_t center, double radius) {
	v2d_vec_t pos = _tr(render, center);
	double rad = creal(_tr_siz(render, radius));

	int x0 = v2dvx(pos);
	int y0 = v2dvy(pos);
	int x = rad - 1;
	int y = 0;
	int dx = 1;
	int dy = 1;
	int diam = rad*2;
	int err = dx - diam;

	while (x >= y) {
		SDL_RenderDrawPoint(render->sdl_ren, x0 + x, y0 + y);
		SDL_RenderDrawPoint(render->sdl_ren, x0 + y, y0 + x);
		SDL_RenderDrawPoint(render->sdl_ren, x0 - y, y0 + x);
		SDL_RenderDrawPoint(render->sdl_ren, x0 - x, y0 + y);
		SDL_RenderDrawPoint(render->sdl_ren, x0 - x, y0 - y);
		SDL_RenderDrawPoint(render->sdl_ren, x0 - y, y0 - x);
		SDL_RenderDrawPoint(render->sdl_ren, x0 + y, y0 - x);
		SDL_RenderDrawPoint(render->sdl_ren, x0 + x, y0 - y);

		if (err <= 0) {
			y++;
			err += dy;
			dy += 2;
		} else {
			x--;
			dx += 2;
			err += dx - diam;
		}
	}
}
