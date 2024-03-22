#include <vector>
#include <random>
#include <SDL2/SDL.h>
#include "Object.h"
#include "include/QuadTree.h"

const size_t AGENT_SIZE = 100;

const unsigned int SCREEN_WIDTH  = 1600;
const unsigned int SCREEN_HEIGHT = 900;

SDL_Window*   WINDOW = nullptr;
SDL_Renderer* RENDER = nullptr;
SDL_Event EVENT;

std::vector<Object*> ALL_OBJECTS;

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
                    "Quad Tree",
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
}

void InstantiateObjects(std::vector<Object*>& _objects)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    for(size_t _i = 0; _i < AGENT_SIZE; ++_i)
    {
        std::uniform_int_distribution<int> _distX(0, SCREEN_WIDTH);
        std::uniform_int_distribution<int> _distY(0, SCREEN_HEIGHT);

        std::uniform_int_distribution<int> _distVel(100, 150);
        auto _randomVel = static_cast<float>(_distVel(gen));

        auto _object = new Object();
        _object->x = static_cast<float>(_distX(gen));
        _object->y = static_cast<float>(_distY(gen));
        _object->velX = _randomVel;
        _object->velY = _randomVel;
        _object->Radius = 10.0f;

        _objects.push_back(_object);
    }
}

void UpdateObject(Object& _object, float _dt)
{
    _object.x += _object.velX * _dt;
    _object.y += _object.velY * _dt;
}

double distanceSquared(int x1, int y1, int x2, int y2)
{
    int deltaX = x2 - x1;
    int deltaY = y2 - y1;
    return deltaX*deltaX + deltaY*deltaY;
}

bool CheckCollision(const Object& _objA, const Object& _objB)
{
    float _totalRadiusSquared = _objA.Radius + _objB.Radius;
    _totalRadiusSquared *= _totalRadiusSquared;

    float _deltaX = _objB.x - _objA.x;
    float _deltaY = _objB.y - _objA.y;

    if(_deltaX * _deltaX + _deltaY * _deltaY < _totalRadiusSquared)
        return true;

    return false;
}

void EvaluateCollision(Object& _objA, Object& _objB)
{
    if(!CheckCollision(_objA, _objB))
        return;

    float _velX = _objA.velX;
    float _velY = _objA.velY;

    _objA.velX = _objB.velX;
    _objA.velY = _objB.velY;

    _objB.velX = _velX;
    _objB.velY = _velY;
}

void EvaluateFrameCollision(Object& _object)
{
    if(_object.x + _object.Radius > SCREEN_WIDTH ||
       _object.x - _object.Radius < 0)
    {
        _object.velX *= -1;
    }

    if(_object.y + _object.Radius > SCREEN_HEIGHT ||
       _object.y - _object.Radius < 0)
    {
        _object.velY *= -1;
    }
}

void DrawObject(const Object& _object, SDL_Color _color)
{
    auto _radius  = static_cast<int>(_object.Radius);
    auto _centerX = static_cast<int>(_object.x);
    auto _centerY = static_cast<int>(_object.y);

    for(int x = -_radius; x <= _radius; x++)
    {
        int height = (int)sqrt(_radius * _radius - x * x);

        for(int y = -height; y <= height; y++)
        {
            SDL_SetRenderDrawColor(RENDER, _color.r, _color.g, _color.b, _color.a);
            SDL_RenderDrawPoint   (RENDER, _centerX + x, _centerY + y);
        }
    }
}

int main()
{
    InitSDL();
    InstantiateObjects(ALL_OBJECTS);

    bool _isRunning = true;
    while(_isRunning)
    {
        _Timer.Tick();

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
            }
        }

        for(Object* _object: ALL_OBJECTS)
        {
            UpdateObject(*_object, _Timer.deltaTime);

            for(Object* _otherObject: ALL_OBJECTS)
            {
                if(_object != _otherObject)
                    EvaluateCollision(*_object, *_otherObject);
            }

            EvaluateFrameCollision(*_object);
        }

        QuadTree _quadTree;
        _quadTree.Box.SetMin(0, 0);
        _quadTree.Box.SetMax(SCREEN_WIDTH, SCREEN_HEIGHT);

        std::vector<Point> _points;
        for(Object* _obj: ALL_OBJECTS)
        {
            Point _point { _obj->x, _obj->y };
            _points.push_back(_point);
        }

        BuildQuadTree(_quadTree, _points.begin(), _points.end());

        SDL_SetRenderDrawColor(RENDER, 0, 0, 0, 0);
        SDL_RenderClear(RENDER);

        for(Object* _object: ALL_OBJECTS)
            DrawObject(*_object, {255, 255, 255, 255});

        SDL_SetRenderDrawColor(RENDER, 255, 0, 0, 255);
        for(BBox& _box : _quadTree.Boxes)
        {
            int _x = static_cast<int>(_box.Min.X);
            int _y = static_cast<int>(_box.Min.Y);
            int _w = static_cast<int>(_box.Max.X - _box.Min.X);
            int _h = static_cast<int>(_box.Max.Y - _box.Min.Y);

            SDL_Rect _targetRect {_x, _y, _w, _h};
            SDL_RenderDrawRect(RENDER, &_targetRect);
        }

        SDL_RenderPresent(RENDER);
    }

    for(Object* _object: ALL_OBJECTS)
        delete _object;

    SDL_DestroyRenderer(RENDER);
    SDL_DestroyWindow(WINDOW);
    return -1;
}
