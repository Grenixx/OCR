#ifndef FILTER_H
#define FILTER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void to_grayscale(SDL_Surface *surface);
void to_black_and_white(SDL_Surface *surface, int threshold);
void gauss_blur(SDL_Surface *surface);
void sobel(SDL_Surface *surface);
void apply_filters(SDL_Surface *surface);

#endif // FILTER_H