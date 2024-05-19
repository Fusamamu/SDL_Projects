#pragma once
#include <SDL2/SDL.h>

namespace Voronoi
{
    class Timer
    {
    public:
        float DeltaTime;
        float TickCount;

        Timer();
        void Tick();
    };
}