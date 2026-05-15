FLAGS = -Wall -Werror -fsanitize=address -g


all: ocr
	./ocr

color_to_black_and_white: color_to_black_and_white.c
	gcc $(FLAGS) color_to_black_and_white.c -o color_to_black_and_white

ocr: ocr.c filters.c group_detection.c stack.c
	gcc $(FLAGS) $^ -o ocr -lSDL2 -lSDL2_image -lm