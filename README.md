# v2d

This is a game engine. Kinda. It's a very simple 2D "game library" that provides a bunch of useful stuff for working with SDL2. It provides vector math, simple 2D collision detection, a simple game loop and some other stuff, and also lets you rewrite most of it yourself if you want to. 

The source code has less than 300 semicolons (plus another 100 or so for the headers) and should compile in a couple of seconds (`make -j4` completes in 0.5s on my Ryzen 3 2200G). You can probably read through the code in an hour or so. The more complex bits have comments. Sometimes.

The best place to find documentation is the headers. You can find them in `include/v2d/*.h`.

v2d compiles with GCC on Linux and mingw64 through MSYS2. It should work fine with any compiler that supports C99 on any platform that supports SDL2, but I've not tested it.

## Installation

You can do whatever you want to. I don't install, I just use git submodules and some nice Makefile:

```make
...

CFLAGS += -Iv2d/include

v2d/libv2d.a:
	make -C v2d

my_awsum_game: main.o blah.o ... v2d/libv2d.a
	...
```

## Features/TODO

- [x] Vector maths
- [ ] Narrow-phase collision detection & raycasting
  - [x] Axis-aligned bounding box
  - [x] Circle
  - [ ] Polygon
- [x] Rendering system
  - [x] Fix SDL's inverted Y axis
  - [x] Camera transformations
- [x] Actions
- [x] Game loop
- [ ] Broad-phase collision detection
- [ ] Tilemap loader
- [ ] More examples
