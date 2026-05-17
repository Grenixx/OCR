#ifndef GROUP_DETECTION_H
#define GROUP_DETECTION_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "stack.h"

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

void destroy_bb_box_and_stack(struct stack **bb_stack);
struct t_point *t_point_init(int x, int y);
struct stack *make_group(SDL_Surface *surface);

#endif // GROUP_DETECTION_H