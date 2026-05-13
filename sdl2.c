#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

void to_grayscale(SDL_Surface *surface)
{
    uint8_t *pixels = (uint8_t *)surface->pixels;
    for (int i = 0; i < surface->w * surface->h * 3; i += 3)
    {
        uint8_t r = pixels[i];
        uint8_t g = pixels[i + 1];
        uint8_t b = pixels[i + 2];

        uint8_t gray = 0.299 * r + 0.587 * g + 0.114 * b;

        pixels[i] = gray;
        pixels[i + 1] = gray;
        pixels[i + 2] = gray;
    }
}

void to_black_and_white(SDL_Surface *surface, int threshold)
{
    uint8_t *pixels = (uint8_t *)surface->pixels;
    for (int i = 0; i < surface->w * surface->h * 3; i += 3)
    {
        uint8_t r = pixels[i];
        uint8_t g = pixels[i + 1];
        uint8_t b = pixels[i + 2];

        uint8_t gray = 0.299 * r + 0.587 * g + 0.114 * b;
        // uint8_t bl = (gray > threshold) ? 255 : 0;
        uint8_t bl = 0;
        if (gray > threshold)
            bl = 255;
        else
            bl = 0;

        pixels[i] = bl;
        pixels[i + 1] = bl;
        pixels[i + 2] = bl;
    }
}

void sobel(SDL_surface *surface)
{
    uint8_t *pixels = (uint8_t *)surface->pixels;
    uint8_t *sobel_pixels;
    int ker_sum_h = 0;
    int ker_sum_v = 0;
    // int horizontal_ker[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    // int verctical_ker[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    int horizontal_ker[9] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    int verctical_ker[9] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};
    // on ne traite pas les bord car notre kernel de 3x3 sort de limage pour les pixel en bordure d image
    for (int i = 1; i < surface->w - 1; i++)
    {
        for (int j = 1; j < surface->h - 1; j++)
        {
            // ker_sum_h += horizontal_ker[0][0] * pixels[(i - 1) * (j - 1)];
            // ker_sum_v += verctical_ker[0][0] * pixels[(i - 1) * (j - 1)];
            int pos_in_ker_h = 0;
            int pos_in_ker_v = 0;
            for (int k = -1; k < 2; k++)
            {
                for (int l = -1; l < 2; l++)
                {
                    ker_sum_h += horizontal_ker[pos_in_ker_h++] * pixels[(i + k) * (j + l)];
                    ker_sum_v += verctical_ker[pos_in_ker_v++] * pixels[(i + k) * (j + l)];
                }
            }
        }
    }
}

int main()
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Surface *load_surface = IMG_Load("images/A.jpg");
    SDL_Surface *surface = SDL_ConvertSurfaceFormat(load_surface, SDL_PIXELFORMAT_RGB24, 0);
    SDL_FreeSurface(load_surface);
    to_black_and_white(surface, 128);

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