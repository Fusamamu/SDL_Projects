#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include "UIState.h"

const unsigned int SCREEN_WIDTH  = 1600;
const unsigned int SCREEN_HEIGHT = 900;

SDL_Window*   WINDOW = nullptr;
SDL_Renderer* RENDER = nullptr;
SDL_Event EVENT;
SDL_Point MOUSE_POSITION;

struct Time
{
    float deltaTime;
    float tickCount;

    Time(): deltaTime(0), tickCount(0){ }

    void Tick()
    {
        while(!SDL_TICKS_PASSED(SDL_GetTicks(), tickCount + 16));

        deltaTime = (SDL_GetTicks() - tickCount)/1000.0f;

        if(deltaTime > 0.05f)
            deltaTime = 0.05f;

        tickCount = SDL_GetTicks();
    }
}_Timer;

void InitSDL()
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return;
    }
    WINDOW = SDL_CreateWindow
            (
                    "Tile Editor",
                    SDL_WINDOWPOS_UNDEFINED,
                    SDL_WINDOWPOS_UNDEFINED,
                    SCREEN_WIDTH,
                    SCREEN_HEIGHT,
                    SDL_WINDOW_SHOWN
            );

    if(WINDOW == nullptr)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return;
    }
    RENDER = SDL_CreateRenderer(WINDOW, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    int imgFlags = IMG_INIT_PNG;
    if( !( IMG_Init( imgFlags ) & imgFlags ) )
    {
        printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
        return;
    }
}

SDL_Texture* LoadTexture(const std::string& _path)
{
    SDL_Texture* newTexture;
    SDL_Surface* loadedSurface = IMG_Load(_path.c_str());

    if(loadedSurface == nullptr)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", _path.c_str(), IMG_GetError() );
        return nullptr;
    }

    newTexture = SDL_CreateTextureFromSurface(RENDER, loadedSurface);
    if(newTexture == nullptr)
    {
        printf("Unable to create texture from %s! SDL Error: %s\n", _path.c_str(), SDL_GetError() );
        return nullptr;
    }

    SDL_FreeSurface(loadedSurface);
    return newTexture;
}

void DrawGrid(SDL_Renderer* _render, const int& _gridSize = 32, SDL_Color _col = { 58, 66, 66, SDL_ALPHA_OPAQUE})
{
    SDL_SetRenderDrawColor(_render, _col.r, _col.g, _col.b, SDL_ALPHA_OPAQUE);

    for(int _i = 0; _i < SCREEN_WIDTH; _i += _gridSize)
        SDL_RenderDrawLine(_render, _i, 0, _i, SCREEN_HEIGHT);

    for(int _i = 0; _i < SCREEN_WIDTH; _i += _gridSize)
        SDL_RenderDrawLine(_render, 0, _i, SCREEN_WIDTH, _i);
}

struct Cell
{
    SDL_Point position;
    SDL_Rect rect;
};

std::vector<Cell> ALL_CELLS;

struct Grid
{
    static const int GRID_SIZE = 32;

    SDL_Point origin;
    SDL_Rect frame;

    int cellCountWidth;
    int cellCountHeight;

    Cell CellArray[32][32] { };

    Grid(): origin({0, 0}), cellCountWidth(32), cellCountHeight(32)
    {
        for(int _i = 0; _i < cellCountWidth; ++_i)
        {
            for(int _j = 0; _j < cellCountHeight; ++_j)
            {
                CellArray[_i][_j].position = {_i, _j};
                CellArray[_i][_j].rect     = {_i, _j, cellCountWidth, cellCountHeight};
            }
        }

        frame.x = origin.x;
        frame.y = origin.y;
        frame.w = cellCountWidth  * GRID_SIZE;
        frame.h = cellCountHeight * GRID_SIZE;
    }
};

int main()
{
    InitSDL();

    SDL_Texture* _texture = LoadTexture("../res/tiles/dirt_path_corner.png");

    int w, h, access;
    unsigned int format;
    SDL_QueryTexture( _texture, &format, &access, &w, &h );

    bool _isRunning = true;
    while(_isRunning)
    {
        _Timer.Tick();

        SDL_GetMouseState(&MOUSE_POSITION.x, &MOUSE_POSITION.y);

        while(SDL_PollEvent(&EVENT))
        {
            switch(EVENT.type)
            {
                case SDL_QUIT:
                    _isRunning = false;
                    break;
                case SDL_KEYDOWN:
                    if(EVENT.key.keysym.sym == SDLK_ESCAPE)
                        _isRunning = false;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    Cell _cell { };
                    _cell.position.x = MOUSE_POSITION.x / Grid::GRID_SIZE * Grid::GRID_SIZE;
                    _cell.position.y = MOUSE_POSITION.y / Grid::GRID_SIZE * Grid::GRID_SIZE;
                    _cell.rect.x = _cell.position.x;
                    _cell.rect.y = _cell.position.y;
                    _cell.rect.w = w;
                    _cell.rect.h = h;
                    ALL_CELLS.push_back(_cell);
                    break;
//                case SDL_MOUSEBUTTONUP:
//                    std::cout << "Mouse is up" << std::endl;
                    break;
            }

            MGUI::UpdateState(EVENT, MGUI::_UIState);
        }

        SDL_SetRenderDrawColor(RENDER, 9, 15, 28, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(RENDER);

        SDL_Rect dst;
        dst.x = MOUSE_POSITION.x / Grid::GRID_SIZE * Grid::GRID_SIZE;
        dst.y = MOUSE_POSITION.y / Grid::GRID_SIZE * Grid::GRID_SIZE;
        dst.w = w;
        dst.h = h;

        SDL_RenderCopy(RENDER, _texture, nullptr, &dst);

        for(const Cell& _cell : ALL_CELLS)
            SDL_RenderCopy(RENDER, _texture, nullptr, &_cell.rect);

        DrawGrid(RENDER);

        MGUI::BeginFrame();

        if(MGUI::Button(RENDER, 4, 10, 10))
        {

        }
        MGUI::EndFrame();


        SDL_RenderPresent(RENDER);
    }

    SDL_DestroyTexture(_texture);
    SDL_DestroyRenderer(RENDER);
    SDL_DestroyWindow(WINDOW);

    _texture = nullptr;
    RENDER = nullptr;
    WINDOW = nullptr;
    return 0;
}
