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

struct t_dist_label
{
    int dist;
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
struct t_sample **load_images_from_trainset(char *dirpath, int nb_of_image_per_class);

#endif // KNN_H