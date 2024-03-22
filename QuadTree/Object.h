#pragma once
#include <SDL2/SDL.h>

struct Object
{
    float x;
    float y;

    float velX;
    float velY;

    float Radius;

    Object(): x(0.f), y(0.f), velX(0.f), velY(0.f), Radius(1)
    {

    }

    ~Object() = default;
};