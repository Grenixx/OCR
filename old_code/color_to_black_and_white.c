#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void print_bm(FILE *img)
{
    char bmp_check[2];
    size_t byte_read = fread(bmp_check, 1, sizeof(bmp_check), img);
    if (byte_read != 2)
    {
        printf("failed to read BM file les byte lu sont au nombre de: %ld\n", byte_read);
        return;
    }
    putchar(bmp_check[0]);
    putchar(bmp_check[1]);
    putchar('\n');
}

typedef struct
{
    int32_t width;
    int32_t height;
} t_dim;

t_dim *get_dim(FILE *img)
{
    /*
    header[0] à header[3] = taille du header (pas utile)
    header[4] à header[7] = width
    header[8] à header[11] = height
    */
    t_dim *dim = calloc(sizeof(t_dim), 1);
    dim->width = 0;
    dim->height = 0;
    char header[40];
    fseek(img, 14, 0);
    fread(header, 1, sizeof(header), img);
    dim->width = *(int32_t *)(header + 4); // on a un pointeur et on veux un int de 4 octect :D
    dim->height = *(int32_t *)(header + 8);
    return dim;
}

int32_t get_pixel_offset(FILE *img)
{
    // retourne l offset la ou les pixel commance dans le bmp
    char buf[4];
    fseek(img, 10, 0);
    fread(buf, 1, sizeof(buf), img);
    return *(int32_t *)(buf);
}

uint8_t *get_pixel_array(FILE *img)
{
    t_dim *dim = get_dim(img);
    int32_t px_offset = get_pixel_offset(img);
    uint8_t *pixel = malloc(sizeof(uint8_t) * 3 * dim->height * dim->width); // on a 3 uint8_t de taille 1 byte chaque uint == BRG
    fseek(img, px_offset, 0);
    char px_line[(dim->width * 3 + 3) & ~3]; // on fait otu ca car y a un extra 3 octect par ligne
    for (int32_t i = 0; i < dim->height; i++)
    {
        fread(px_line, sizeof(uint8_t), sizeof(px_line), img);
        memcpy(pixel + i * dim->width * 3, px_line, dim->width * 3);
    }
    free(dim);
    return pixel;
}

int main(int argc, char *argv[])
{
    char *file_to_open = "images/Dofus.bmp";
    if (argc >= 2)
        file_to_open = argv[1];
    FILE *img = fopen(file_to_open, "rb");
    if (!img)
        printf("No image has been found or Memory error during fopen\n");

    print_bm(img);
    t_dim *dim = get_dim(img);
    printf("(w:%d,h:%d)\n", dim->width, dim->height);
    free(dim);

    printf("Les pixel commence avec un offset de: %d\n", get_pixel_offset(img));

    uint8_t *pixel = get_pixel_array(img);
    (void)(pixel);
    free(pixel);

    fclose(img);
}