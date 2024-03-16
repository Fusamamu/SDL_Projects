#include<iostream>
#include<string>
#include<functional>
#include<cstdlib>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_ttf.h>

int main()
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    const unsigned int SCREEN_WIDTH  = 900;
    const unsigned int SCREEN_HEIGHT = 1600;

    SDL_Window* _window = SDL_CreateWindow
        (
            "Quad Tree",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN
        );
    
    if(_window == NULL)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Renderer* _render = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_Event _event;

    bool _isRunning = true;

    while(_isRunning)
    {
        while(SDL_PollEvent(&_event))
        {
            switch(_event.type)
            {
                case SDL_QUIT:
                    _isRunning = false;
                    break;
                case SDL_KEYDOWN:
                    if(_event.key.keysym.sym == SDLK_ESCAPE)
                        _isRunning = false;
                    break;
            }
        }
    } 

    SDL_DestroyRenderer(_render);
    SDL_DestroyWindow(_window);    

    return -1;
    
}
