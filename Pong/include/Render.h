#ifndef RENDER_H
#define RENDER_H
#include<SDL2/SDL.h>

class Render
{
    public:
        Render(SDL_Renderer* _render);
        ~Render();   
        
    private:
        SDL_Renderer* _render;
};
#endif
