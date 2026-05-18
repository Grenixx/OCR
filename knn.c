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

int count_number_of_entry_in_dir(char *dirpath)
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

struct t_sample **load_images_from_trainset(char *dirpath, int nb_of_image_per_class)
{
    int nb_of_dir = count_number_of_entry_in_dir(dirpath);
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
            struct t_sample *s = t_sample_init(surface, entry->d_name[0]);
            X_train[curr_sample++] = s;
        }
        closedir(currdir);
    }

    closedir(dir);
    return X_train;
}
struct t_sample **load_images_from_test_set(char *dirpath)
{
    int nb_of_files = count_number_of_entry_in_dir(dirpath);
    struct t_sample **X_test = malloc(sizeof(struct t_sample *) * (nb_of_files + 1));
    X_test[nb_of_files] = NULL;
    int curr_sample = 0;

    DIR *dir = opendir(dirpath);
    struct dirent *entry = NULL;
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_name[0] == '.')
            continue;
        char filepath[2048];
        snprintf(filepath, sizeof(filepath), "%s/%s", dirpath, entry->d_name);
        SDL_Surface *load_surface = IMG_Load(filepath);
        SDL_Surface *surface = SDL_ConvertSurfaceFormat(load_surface, SDL_PIXELFORMAT_RGB24, 0);
        SDL_FreeSurface(load_surface);
        X_test[curr_sample++] = t_sample_init(surface, '?');
    }

    closedir(dir);
    return X_test;
}
struct t_dist_label ***calulate_dist(struct t_sample **train, struct t_sample **test)
{
    int train_size = 0;
    while (train[train_size])
        train_size++;

    int test_size = 0;
    while (test[test_size])
        test_size++;

    struct t_dist_label ***dists =
        malloc(sizeof(struct t_dist_label **) * (test_size + 1));

    dists[test_size] = NULL;

    for (int i = 0; i < test_size; i++)
    {
        dists[i] =
            malloc(sizeof(struct t_dist_label *) * (train_size + 1));

        dists[i][train_size] = NULL;

        for (int j = 0; j < train_size; j++)
        {
            struct t_dist_label *node =
                malloc(sizeof(struct t_dist_label));

            node->dist =
                dist(test[i]->surface,
                     train[j]->surface);

            node->label = train[j]->label;

            dists[i][j] = node;
        }
    }

    return dists;
}

void free_dists(struct t_dist_label ***dists, int test_size, int train_size)
{
    for (int i = 0; i < test_size; i++)
    {
        for (int j = 0; j < train_size; j++)
        {
            free(dists[i][j]);
        }
        free(dists[i]);
    }
    free(dists);
}

int compare_dist(const void *a, const void *b)
{
    struct t_dist_label *d1 = *(struct t_dist_label **)a;
    struct t_dist_label *d2 = *(struct t_dist_label **)b;

    return d1->dist - d2->dist;
}

char most_common_label(struct t_dist_label **dists, int k)
{
    int freq[256] = {0};

    for (int i = 0; i < k; i++)
    {
        freq[(unsigned char)dists[i]->label]++;
    }

    int max = 0;
    char best = '?';

    for (int i = 0; i < 256; i++)
    {
        if (freq[i] > max)
        {
            max = freq[i];
            best = i;
        }
    }

    return best;
}

char *knn(struct t_sample **train,
          struct t_sample **test,
          int k)
{
    struct t_dist_label ***dists =
        calulate_dist(train, test);

    int test_size = 0;
    while (test[test_size])
        test_size++;

    int train_size = 0;
    while (train[train_size])
        train_size++;

    char *res = malloc(sizeof(char) * (test_size + 1));
    res[test_size] = '\0';

    for (int i = 0; i < test_size; i++)
    {
        qsort(dists[i],
              train_size,
              sizeof(struct t_dist_label *),
              compare_dist);

        res[i] = most_common_label(dists[i], k);
    }
    free_dists(dists, test_size, train_size);
    return res;
}

int main()
{
    struct t_sample **X_train = load_images_from_trainset("dataset/train", 50);
    struct t_sample **X_test = load_images_from_test_set("dataset/test");
    char *res = knn(X_train, X_test, 1);
    printf("%s\n", res);
    free(res);
    free_t_sample_list(X_test);
    free_t_sample_list(X_train);
}
