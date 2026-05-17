FLAGS = -Wall -Werror -fsanitize=address -g


all: ocr
	./ocr

color_to_black_and_white: color_to_black_and_white.c
	gcc $(FLAGS) color_to_black_and_white.c -o color_to_black_and_white

filters.o: filters.c filters.h
	gcc $(FLAGS) -c filters.c

group_detection.o: group_detection.c group_detection.h
	gcc $(FLAGS) -c group_detection.c

stack.o: stack.c stack.h
	gcc $(FLAGS) -c stack.c

knn.o: knn.c knn.h
	gcc $(FLAGS) -c knn.c

knn_test: knn
	./knn

knn: knn.c knn.h
	gcc $(FLAGS) knn.c -o knn -lSDL2 -lSDL2_image -lm

ocr: ocr.c filters.o group_detection.o stack.o knn.o
	gcc $(FLAGS) $^ -o ocr -lSDL2 -lSDL2_image -lm
