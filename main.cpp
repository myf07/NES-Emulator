#include <iostream>
#include <memory>

#include "Bus.h"
#include "Loader.h"
#include "main.h"
#include <SDL.h>

// SDL Graphics Code
SDL_Window * window;
SDL_Surface * window_surface;
unsigned int * pixels;

const int resW = 256;
const int resH = 240;

void initSDL() {
    // initialize SDL window
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        std::cout << "SDL could not be initialized: " << SDL_GetError();
    else
        std::cout << "SDL video system is ready to go\n";

    window = window = SDL_CreateWindow(
        "NES Emulator",        // window title
        SDL_WINDOWPOS_UNDEFINED, // initial x position
        SDL_WINDOWPOS_UNDEFINED, // initial y position
        resW,                     // width, in pixels
        resH,                     // height, in pixels
        SDL_WINDOW_SHOWN         // flags - see below
    );

    window_surface = SDL_GetWindowSurface(window);

    pixels = (unsigned int *) window_surface->pixels;
    int width = window_surface->w;
    int height = window_surface->h;

    printf("Pixel format: %s\n", SDL_GetPixelFormatName(window_surface->format->format));
}

void DisplayPixel(uint16_t x, uint16_t y, uint8_t r, uint8_t g, uint8_t b) {
    pixels[x + y * resW] = SDL_MapRGB(window_surface->format, r, g, b);
}

int main() {

    // initialize graphics window
    initSDL();

    // initiaize NES circuit components
    // initialize CPU, PPU, Bus, APU
    Bus bus;
    MOS6502 cpu;
    std::shared_ptr<Loader> loader = std::make_shared<Loader>("DonkeyKong.nes"); // make cartridge 
    bus.InsertCartridge(loader);
    bus.RST(); // initial reset

    // call the clock 
    while (1)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT) exit(0);
            if (event.type == SDL_WINDOWEVENT)
            {
                if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                {
                    window_surface = SDL_GetWindowSurface(window);
                    pixels = (unsigned int *) window_surface->pixels;
                    width = window_surface->w;
                    height = window_surface->h;
                    printf("Size changed: %d, %d\n", width, height);
                }
            }
        }
/* Might be useful for a buffer
        //Set every pixel to white.
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                if((x + y * width) % 2 == 0) {
                    pixels[x + y * width] =
                        SDL_MapRGB(window_surface->format, 200, 100, 250);
                }
            }
        }
*/
        SDL_UpdateWindowSurface(window);
    }
}
