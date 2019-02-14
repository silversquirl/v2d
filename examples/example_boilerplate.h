#ifndef _V2D_EXAMPLE_BOILERPLATE_H
#define _V2D_EXAMPLE_BOILERPLATE_H

#include <stdio.h>

int main(int argc, char *argv[]) {
	if (SDL_Init(SDL_INIT_VIDEO)) {
		fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
		return 1;
	}

	SDL_Window *win = SDL_CreateWindow(EX_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);
	if (!win) {
		fprintf(stderr, "SDL_CreateWindow: %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}

	int ret = ex_main(win);

	SDL_DestroyWindow(win);
	SDL_Quit();

	return ret;
}

#endif
