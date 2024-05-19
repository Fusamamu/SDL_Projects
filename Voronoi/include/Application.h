#pragma once
#include <SDL2/SDL.h>
#include "Timer.h"

namespace Voronoi
{
    class Application
    {
    public:
        bool IsRunning;

        unsigned int WindowWidth;
        unsigned int WindowHeight;

        Timer Timer;

        SDL_Window* Window;
        SDL_Renderer* Render;
        SDL_Event Event;

        Application();
        ~Application();

        void Update();
    };


    class Foo
    {
        class Bar
        {
            friend Foo;


        };
    };
}