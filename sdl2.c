#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <math.h>

void to_grayscale(SDL_Surface *surface)
{
    uint8_t *pixels = (uint8_t *)surface->pixels;

    for (int y = 0; y < surface->h; y++)
    {
        uint8_t *row =
            pixels + y * surface->pitch;

        for (int x = 0; x < surface->w; x++)
        {
            uint8_t *p = row + x * 3;

            uint8_t r = p[0];
            uint8_t g = p[1];
            uint8_t b = p[2];

            uint8_t gray =
                0.299 * r +
                0.587 * g +
                0.114 * b;

            p[0] = gray;
            p[1] = gray;
            p[2] = gray;
        }
    }
}

void to_black_and_white(SDL_Surface *surface, int threshold)
{
    uint8_t *pixels = (uint8_t *)surface->pixels;

    for (int y = 0; y < surface->h; y++)
    {
        uint8_t *row =
            pixels + y * surface->pitch;

        for (int x = 0; x < surface->w; x++)
        {
            uint8_t *p = row + x * 3;

            uint8_t r = p[0];
            uint8_t g = p[1];
            uint8_t b = p[2];

            uint8_t gray =
                0.299 * r +
                0.587 * g +
                0.114 * b;

            uint8_t bl = 0;
            if (gray > threshold)
                bl = 255;
            else
                bl = 0;

            p[0] = bl;
            p[1] = bl;
            p[2] = bl;
        }
    }
}

void sobel(SDL_Surface *surface)
{
    uint8_t *pixels = surface->pixels;

    uint8_t *sobel_pixels =
        calloc(surface->pitch * surface->h,
               sizeof(uint8_t));

    int horizontal_ker[9] =
        {
            -1, 0, 1,
            -2, 0, 2,
            -1, 0, 1};

    int vertical_ker[9] =
        {
            -1, -2, -1,
            0, 0, 0,
            1, 2, 1};

    // on ignore les bords
    for (int y = 1; y < surface->h - 1; y++)
    {
        for (int x = 1; x < surface->w - 1; x++)
        {
            int gx = 0;
            int gy = 0;

            int pos = 0;

            // parcours kernel 3x3
            for (int yy = -1; yy <= 1; yy++)
            {
                uint8_t *kernel_row =
                    pixels + (y + yy) * surface->pitch;

                for (int xx = -1; xx <= 1; xx++)
                {
                    uint8_t *p =
                        kernel_row + (x + xx) * 3;

                    uint8_t pixel = p[0];

                    gx += horizontal_ker[pos] * pixel;
                    gy += vertical_ker[pos] * pixel;

                    pos++;
                }
            }

            int mag =
                sqrt(gx * gx + gy * gy);

            if (mag > 255)
                mag = 255;

            uint8_t *out_row =
                sobel_pixels + y * surface->pitch;

            uint8_t *out =
                out_row + x * 3;

            out[0] = mag;
            out[1] = mag;
            out[2] = mag;
        }
    }

    memcpy(
        pixels,
        sobel_pixels,
        surface->pitch * surface->h);

    free(sobel_pixels);
}

int main()
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Surface *load_surface = IMG_Load("images/fleur.jpg");
    SDL_Surface *surface = SDL_ConvertSurfaceFormat(load_surface, SDL_PIXELFORMAT_RGB24, 0);
    SDL_FreeSurface(load_surface);
    // to_grayscale(surface);
    to_black_and_white(surface, 128);
    sobel(surface);

    SDL_Window *window = SDL_CreateWindow("ocr", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, surface->w, surface->h, 0);
    SDL_Renderer *render = SDL_CreateRenderer(window, -1, 0);
    SDL_Texture *texture = SDL_CreateTexture(render, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, surface->w, surface->h);
    SDL_UpdateTexture(texture, NULL, surface->pixels, surface->pitch);
    SDL_RenderCopy(render, texture, NULL, NULL);
    SDL_RenderPresent(render);
    SDL_Delay(2000);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    SDL_Quit();
}