#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "group_detection.h"
#include "stack.h"

struct t_point *t_point_init(int x, int y)
{
    struct t_point *point = calloc(1, sizeof(struct t_point));
    point->x = x;
    point->y = y;
    return point;
}

struct bounding_box *bounding_box_init(int xmin, int xmax, int ymin, int ymax)
{
    struct bounding_box *box = calloc(1, sizeof(struct bounding_box));
    box->xmin = xmin;
    box->xmax = xmax;
    box->ymin = ymin;
    box->ymax = ymax;
    return box;
}

struct bounding_box *flood_fill(SDL_Surface *surface, int *is_visited, struct t_point *point_depart)
{
    uint8_t *px = (uint8_t *)surface->pixels;
    struct stack *s = stack_push(NULL, point_depart);
    struct bounding_box *box = bounding_box_init(point_depart->x, point_depart->x, point_depart->y, point_depart->y);
    while (s)
    {

        struct t_point *point = (struct t_point *)stack_peek(s);
        s = stack_pop(s);
        if (is_visited[point->y * surface->w + point->x])
        {
            free(point);
            continue;
        }
        is_visited[point->y * surface->w + point->x] = 1;
        for (int xx = -1; xx <= 1; xx++)
        {
            for (int yy = -1; yy <= 1; yy++)
            {
                if (point->x + xx < 0 || point->x + xx > surface->w)
                    continue;
                if (point->y + yy < 0 || point->y + yy > surface->h)
                    continue;
                uint8_t *currpx = px + (point->y + yy) * surface->pitch + (point->x + xx) * 3;
                if (currpx[0] > 128)
                {
                    if (point->x + xx < box->xmin)
                        box->xmin = point->x + xx;
                    if (point->x + xx > box->xmax)
                        box->xmax = point->x + xx;
                    if ((point->y + yy) < box->ymin)
                        box->ymin = (point->y + yy);
                    if ((point->y + yy) > box->ymax)
                        box->ymax = (point->y + yy);
                    struct t_point *to_stack_point = t_point_init(point->x + xx, point->y + yy);
                    s = stack_push(s, to_stack_point);
                }
            }
        }

        free(point);
    }

    stack_destroy(&s);
    return box;
}

void show_box_corner(SDL_Surface *surface, struct bounding_box *box)
{
    uint8_t *px = (uint8_t *)surface->pixels;
    px[box->ymin * surface->pitch + box->xmin * 3] = 255;
    px[box->ymax * surface->pitch + box->xmin * 3] = 255;
    px[box->ymin * surface->pitch + box->xmax * 3] = 255;
    px[box->ymax * surface->pitch + box->xmax * 3] = 255;
}

void show_box(SDL_Surface *surface, struct stack *bb_stack)
{
    uint8_t *px = (uint8_t *)surface->pixels;
    while (bb_stack)
    {
        struct bounding_box *box = (struct bounding_box *)stack_peek(bb_stack);
        for (int y = box->ymin; y < box->ymax; y++)
            px[y * surface->pitch + box->xmin * 3] = 255;

        for (int y = box->ymin; y < box->ymax; y++)
            px[y * surface->pitch + box->xmax * 3] = 255;

        for (int x = box->xmin; x < box->xmax; x++)
            px[box->ymin * surface->pitch + x * 3] = 255;

        for (int x = box->xmin; x < box->xmax; x++)
            px[box->ymax * surface->pitch + x * 3] = 255;

        bb_stack = stack_pop(bb_stack);
    }
}

struct stack *make_group(SDL_Surface *surface)
{
    uint8_t *px = (uint8_t *)surface->pixels;
    int *is_visited = calloc(surface->w * surface->h, sizeof(int));
    struct stack *bb_stack = NULL;
    for (int y = 0; y < surface->h; y++)
    {
        for (int x = 0; x < surface->w; x++)
        {
            uint8_t *currpx = px + y * surface->pitch + x * 3;
            if (currpx[0] > 128)
            {
                struct t_point *point_depart = t_point_init(x, y);
                struct bounding_box *box = flood_fill(surface, is_visited, point_depart);
                bb_stack = stack_push(bb_stack, box);
            }
        }
    }

    free(is_visited);
    show_box(surface, bb_stack);
    return bb_stack;
}
