#include "Application.h"

namespace Voronoi
{
    Application::Application():
    IsRunning(true),
    WindowWidth(1600),
    WindowHeight(900),
    Window(nullptr),
    Render(nullptr)
    {
        if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
        {
            printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
            return;
        }
        Window = SDL_CreateWindow
                (
                        "VORONOI",
                        SDL_WINDOWPOS_UNDEFINED,
                        SDL_WINDOWPOS_UNDEFINED,
                        (int)WindowWidth,
                        (int)WindowHeight,
                        SDL_WINDOW_SHOWN
                );


        Render = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    }

    Application::~Application()
    {
        SDL_DestroyRenderer(Render);
        SDL_DestroyWindow(Window);
    }

    void Application::Update()
    {
        while(IsRunning)
        {
            Timer.Tick();

            while(SDL_PollEvent(&Event))
            {
                switch(Event.type)
                {
                    case SDL_QUIT:
                        IsRunning = false;
                        break;
                    case SDL_KEYDOWN:
                        if(Event.key.keysym.sym == SDLK_ESCAPE)
                            IsRunning = false;
                        break;
                }
            }

            SDL_SetRenderDrawColor(Render, 0, 0, 0, 0);
            SDL_RenderClear(Render);


            SDL_RenderPresent(Render);
        }
    }
}