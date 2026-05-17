#include "knn.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>

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

int count_number_of_dir_in_dir(char *dirpath)
{
    DIR *dir = opendir(dirpath);
    struct dirent *e = NULL;
    int nb_of_dir = 0;
    while ((e = readdir(dir)) != NULL)
    {
        if (e->d_name[0] == '.')
            continue;
        nb_of_dir++;
    }
    closedir(dir);
    return nb_of_dir;
}

struct t_sample *t_sample_init(SDL_Surface *surface, char label)
{
    struct t_sample *s = calloc(1, sizeof(struct t_sample));
    s->surface = surface;
    s->label = label;
    return s;
}

void free_t_sample_list(struct t_sample **t_sample)
{
    int i = 0;
    while (t_sample[i] != NULL)
    {
        SDL_FreeSurface(t_sample[i]->surface);
        free(t_sample[i]);
        i++;
    }

    free(t_sample);
}

struct t_sample **load_images_from_trainset_in_list(char *dirpath, int nb_of_image_per_class)
{
    int nb_of_dir = count_number_of_dir_in_dir(dirpath);
    struct t_sample **X_train = malloc(sizeof(struct t_sample *) * (nb_of_dir * nb_of_image_per_class + 1));
    X_train[nb_of_dir * nb_of_image_per_class] = NULL;
    int curr_sample = 0;

    DIR *dir = opendir(dirpath);
    struct dirent *entry = NULL;
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_name[0] == '.')
            continue;
        char path[1024];
        snprintf(path, sizeof(path), "%s/%s", dirpath, entry->d_name);
        DIR *currdir = opendir(path);
        struct dirent *currdir_entry = NULL;
        // le +2 dans nb of img per class c est pour le . et .. qui sont par defaul et que l on ignore
        for (int i = 0; i < nb_of_image_per_class + 2 && (currdir_entry = readdir(currdir)) != NULL; i++)
        {
            if (currdir_entry->d_name[0] == '.')
                continue;
            char filepath[2048];
            snprintf(filepath, sizeof(filepath), "%s/%s", path, currdir_entry->d_name);
            // printf("%s\n", filepath);
            SDL_Surface *load_surface = IMG_Load(filepath);
            SDL_Surface *surface = SDL_ConvertSurfaceFormat(load_surface, SDL_PIXELFORMAT_RGB24, 0);
            SDL_FreeSurface(load_surface);
            struct t_sample *s = t_sample_init(surface, currdir_entry->d_name[0]);
            X_train[curr_sample++] = s;
        }
        closedir(currdir);
    }

    closedir(dir);
    return X_train;
}

int main()
{
    struct t_sample **X_train = load_images_from_trainset_in_list("dataset/src", 5);
    free_t_sample_list(X_train);
}

/*
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
*/

// cree un struct dictionaire et stocker les image surface * , et leur dist
// cree un list de surface pour stocker nos image X_train y_train(les labels)
