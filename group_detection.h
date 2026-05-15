#ifndef GROUP_DETECTION_H
#define GROUP_DETECTION_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

struct t_point
{
    int x;
    int y;
};

struct bounding_box
{
    int xmin;
    int xmax;
    int ymin;
    int ymax;
};

struct t_point *t_point_init(int x, int y);
struct stack *make_group(SDL_Surface *surface);

#endif // GROUP_DETECTION_H