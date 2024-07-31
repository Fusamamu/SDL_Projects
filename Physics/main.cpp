#include <iostream>
#include <SDL2/SDL.h>

struct vec2
{
    union
    {
        struct
        {
            float x;
            float y;
        };
        float asArray[2];
    };

    float& operator[](int _i)
    {
        return asArray[_i];
    }

    vec2 operator+(const vec2& _lhs, const vec2& _rhs);
    vec2 operator-(const vec2& _lhs, const vec2& _rhs);
    vec2 operator*(const vec2& _lhs, const vec2& _rhs);
    vec2 operator*(const vec2& _lhs, float _rhs);
    bool operator==(const vec2& _lhs, const vec2& _rhs);
    bool operator!=(const vec2& _lhs, const vec2& _rhs);
};

vec2 vec2::operator+(const vec2& _lhs, const vec2& _rhs)
{
    return { _lhs.x + _rhs.x, _lhs.y + _rhs.y };
}

vec2 vec2::operator-(const vec2& _lhs, const vec2& _rhs)
{
    return { _lhs.x - _rhs.x, _lhs.x - _rhs.y };
}

int main()
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
        return -1;

    SDL_Window* _window = SDL_CreateWindow
            (
                    "Physics",
                    SDL_WINDOWPOS_UNDEFINED,
                    SDL_WINDOWPOS_UNDEFINED,
                    800,
                    600,
                    SDL_WINDOW_SHOWN
            );

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

        SDL_SetRenderDrawColor(_render, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(_render);

        SDL_RenderPresent(_render);
    }

    SDL_DestroyRenderer(_render);
    SDL_DestroyWindow(_window);
    SDL_Quit();

    return 0;
}
