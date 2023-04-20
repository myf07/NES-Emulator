#include <iostream>
#include <memory>
#include <stdlib.h>
#include <windows.h>

#include <SDL2/SDL.h>

#include "Bus.h"
#include "main.h"

// SDL Graphics Code
SDL_Window * window;
SDL_Surface * window_surface;
unsigned int * pixels;

int scale = 2;

const int resW = 256 * scale;
const int resH = 240 * scale;

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
    // printf("%ld, %ld, %ld, %ld, %ld\n", x, y, r, g, b);
    x *= scale;
    y *= scale;
    for(int i = 0; i < scale; i++){
        for(int j = 0; j < scale; j++){
            pixels[x + i + (y + j) * resW] = SDL_MapRGB(window_surface->format, r, g, b);
        }
    }
    
    SDL_UpdateWindowSurface(window);
}

int main (int argv, char** args) {

    // initialize graphics window
    initSDL();

    // initiaize NES circuit components
    // initialize CPU, PPU, Bus, APU
    Bus console;
    std::shared_ptr<Loader> loader = std::make_shared<Loader>("./tests/Donkey_Kong_World_Rev_A.nes"); // make cartridge 
    console.InsertCartridge(loader);
    console.RST();

    // call the clock 
    while (1)
    {
        console.CLK();
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
                }
            }
        }
    }

   return 0;
}
