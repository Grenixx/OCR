FLAGS = -Wall -Werror -fsanitize=address -g


all: sdl2
	./sdl2

color_to_black_and_white: color_to_black_and_white.c
	gcc $(FLAGS) color_to_black_and_white.c -o color_to_black_and_white

sdl2: sdl2.c
	gcc $(FLAGS) sdl2.c -o sdl2 -lSDL2 -lSDL2_image