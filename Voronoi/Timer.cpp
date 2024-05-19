#include "Timer.h"

namespace Voronoi
{
    Timer::Timer():
    DeltaTime(0),
    TickCount(0)
    {
    }

    void Timer::Tick()
    {
        while(!SDL_TICKS_PASSED(SDL_GetTicks(), TickCount + 16));

        DeltaTime = (SDL_GetTicks() - TickCount) / 1000.0f;

        if(DeltaTime > 0.05f)
            DeltaTime = 0.05f;

        TickCount = SDL_GetTicks();
    }
}