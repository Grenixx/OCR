#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

int dist(SDL_Surface *s1, SDL_Surface *s2)
{
    if (s1->w != s2->w || s1->h != s2->h || s1->pitch != s2->pitch)
        printf("Attention les images ont pas la meme longeur et ou hauteur et ou pitch");
    int sum = 0;
    uint8_t *p1 = (uint8_t *)s1->pixels;
    uint8_t *p2 = (uint8_t *)s2->pixels;
    for (int y = 0; y < s1->h; y++)
    {
        for (int x = 0; x < s1->w; x++)
        {
            int diff = p1[x * 3 + y * s1->pitch] - p2[x * 3 + y * s2->pitch];
            sum += diff * diff;
        }
    }
    return sqrt(sum);
}

// cree un struct dictionaire et stocker les image surface * , et leur dist
// cree un list de surface pour stocker nos image X_train y_train(les labels)
SDL_surface *load_images_in_list(char *filepath, int nb_img)
{
}

int main()
{
    SDL_Surface *load_surface = IMG_Load("images/A.png");
    SDL_Surface *surface = SDL_ConvertSurfaceFormat(load_surface, SDL_PIXELFORMAT_RGB24, 0);
    SDL_FreeSurface(load_surface);

    SDL_Surface *load_surface2 = IMG_Load("images/A2.png");
    SDL_Surface *surface2 = SDL_ConvertSurfaceFormat(load_surface2, SDL_PIXELFORMAT_RGB24, 0);
    SDL_FreeSurface(load_surface2);

    printf("%d\n", dist(surface, surface2));

    SDL_FreeSurface(surface);
    SDL_FreeSurface(surface2);
}