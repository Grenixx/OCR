#ifndef KNN_H
#define KNN_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <limits.h>

struct t_sample
{
    SDL_Surface *surface;
    char label;
};

int dist(SDL_Surface *s1, SDL_Surface *s2);

// prend des image dans les dossier et les stock dans un struct t_sample
// pour avoir un surface et un label
// dataset/
//     train/
//         A/
//             000001.png
//             000002.png
//         B/
//             000001.png
//             ...
// SDL_surface *load_images_in_list(char *filepath, int nb_img);

#endif // KNN_H