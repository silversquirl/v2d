/*
 * This example shows basic use of a portion of v2d's functionality.
 *
 * It uses actions with keyboard and mouse triggers, as well as the mouse
 * position action. v2d's world system, entity type and rendering engine are
 * used to draw the "player" on screen, and events are handled using v2d's
 * built-in game loop.
 *
 * Some things which are not demonstrated include: custom event handling,
 * custom game loops, camera transformations, collision detection, etc.
 *
 */
#include <SDL2/SDL.h>
#include <v2d.h>

v2d_action_t actions[] = {
	{"up", {0}},
	{"down", {0}},
	{"right", {0}},
	{"left", {0}},

	{"click", {0}},
	{"mouse", {0}},

	{"quit", {0}},

	{0}
};

v2d_action_trigger_t triggers[] = {
	V2D_KEY_TRIG(SDLK_w, 0, &actions[0]),
	V2D_KEY_TRIG(SDLK_UP, 0, &actions[0]),
	V2D_KEY_TRIG(SDLK_s, 0, &actions[1]),
	V2D_KEY_TRIG(SDLK_DOWN, 0, &actions[1]),
	V2D_KEY_TRIG(SDLK_d, 0, &actions[2]),
	V2D_KEY_TRIG(SDLK_RIGHT, 0, &actions[2]),
	V2D_KEY_TRIG(SDLK_a, 0, &actions[3]),
	V2D_KEY_TRIG(SDLK_LEFT, 0, &actions[3]),

	V2D_MOUSE_TRIG(SDL_BUTTON_LEFT, 1, &actions[4]),

	V2D_KEY_TRIG(SDLK_ESCAPE, 0, &actions[6]),

	{0}
};

struct player {
	v2d_ent_cb_t cb;
	v2d_action_t *up, *down, *right, *left, *click, *mouse;
	v2d_vec_t pos;
	double speed;
};

void player_render(v2d_ent_t *entity, v2d_render_t *render) {
	struct player *player = entity;
	v2d_vec_t size = v2d_vec(2, 2);
	v2d_render_rgb(render, 1, 1, 1);
	v2d_render_draw_rect(render, player->pos - size/2, size);
}

void player_update(v2d_ent_t *entity, double dt) {
	struct player *player = entity;
	v2d_vec_t vel = 0;

	// Keyboard input
	if (player->up->value.s) vel += v2d_vec(0, 1);
	if (player->down->value.s) vel += v2d_vec(0, -1);
	if (player->right->value.s) vel += v2d_vec(1, 0);
	if (player->left->value.s) vel += v2d_vec(-1, 0);

	// Mouse input
	if (player->click->value.s) vel = player->mouse->value.pos - player->pos;

	// Normalize the vector if it's greater than length 1
	double mag = v2d_vec_mag(vel);
	if (mag > 1) vel /= mag;

	player->pos += vel * dt * player->speed;
}

int main(int argc, char *argv[]) {
	if (SDL_Init(SDL_INIT_VIDEO)) {
		fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
		return 1;
	}

	SDL_Window *win = SDL_CreateWindow("Hello, v2d!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
	if (!win) {
		fprintf(stderr, "SDL_CreateWindow: %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}

	v2d_action_dispatcher_t dis = v2d_adis_create(actions, triggers, NULL, &actions[5]);

	v2d_world_t *world = v2d_world_new();

	struct player player = {
		.cb = {player_render, player_update, NULL},
		.up = v2d_adis_find_action(dis, "up"),
		.down = v2d_adis_find_action(dis, "down"),
		.right = v2d_adis_find_action(dis, "right"),
		.left = v2d_adis_find_action(dis, "left"),

		.click = v2d_adis_find_action(dis, "click"),
		.mouse = v2d_adis_find_action(dis, "mouse"),

		.pos = 0,
		.speed = 5,
	};

	v2d_world_add_entity(world, &player);

	v2d_render_t *render = v2d_render_new(win);
	if (!render) {
		fprintf(stderr, "v2d_render_new: %s\n", v2d_errtext);
		SDL_DestroyWindow(win);
		SDL_Quit();
		return 1;
	}

	v2d_gameloop_config_t conf = v2d_gameloop_config_default();
	conf.world = world;
	conf.render = render;
	conf.dis = dis;
	conf.quit_action = v2d_adis_find_action(dis, "quit");

	v2d_gameloop(conf);

	v2d_render_free(render);
	v2d_world_free(world);
	SDL_DestroyWindow(win);
	SDL_Quit();

	return 0;
}
