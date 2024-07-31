#pragma once
#include "SDL2/SDL.h"

class Timer
{
public:
    float DeltaTime;
    float TickCount;

    Timer();
    ~Timer();

public:
    void Tick();

    static Timer& GetInstance()
    {
        static Timer _instance;
        return _instance;
    }
};