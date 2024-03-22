#include <iostream>
#include <vector>
#include <cmath>
#include <SDL2/SDL.h>

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

struct Vector
{
    float x;
    float y;

    Vector operator+(const Vector& _other) const
    {
        return Vector { this->x + _other.x, this->y + _other.y };
    }

    Vector operator-(const Vector& _other) const
    {
        return Vector { this->x - _other.x, this->y - _other.y };
    }

    Vector operator*(const float _scalar) const
    {
        return Vector { _scalar * this->x, _scalar * this->y };
    }

    Vector normalized()
    {
        float _length = sqrt(x * x + y * y);
        x /= _length;
        y /= _length;
        return *this;
    }
};

struct Node
{
    Vector position;
    float radius;
    bool isSelected;

    static std::vector<Node*> ALL_NODES;

    Node(): position({0, 0}), radius(1.f), isSelected(false)
    {
        ALL_NODES.push_back(this);
    }

    ~Node()
    {
        for(auto _it = ALL_NODES.begin(); _it != ALL_NODES.end(); ++_it)
        {
            if(*_it == this)
            {
                ALL_NODES.erase(_it);
                break;
            }
        }
    }

    [[nodiscard]]
    SDL_Point AsSDL_Point() const
    {
        return {static_cast<int>(position.x),
                static_cast<int>(position.y) };
    }
};

std::vector<Node*> Node::ALL_NODES;

bool CheckPointInsideNode(const Node& node, SDL_Point  _point)
{
    SDL_Point _position ={
            static_cast<int>(node.position.x),
            static_cast<int>(node.position.y)
    };

    float _radius = node.radius;

    int _sqrtDist = (_point.x - _position.x) * (_point.x - _position.x) + (_point.y - _position.y) * (_point.y - _position.y);

    if(static_cast<float>(_sqrtDist) < _radius * _radius)
        return true;
    return false;
}

struct Segment
{
    Node origin;
    Node end;
    Node tNode;
    float tValue;

    Vector Direction()
    {
        Vector _dir {
            end.position.x - origin.position.x,
            end.position.y - origin.position.y
        };
        return _dir.normalized();
    }
};

struct Ray
{
    Vector origin;
    Vector dir;
    float tValue;

    Ray() : origin({0.f, 0.f}), dir({1.f, 0.f}), tValue(1.f)
    {

    }
};

void DrawRay(const Ray& _ray)
{
    SDL_Point _origin { (int)(_ray.origin.x) , (int)(_ray.origin.y) };

    SDL_SetRenderDrawColor(RENDER, 255, 255, 255 , 255);
    //SDL_RenderDrawLine(RENDER, _origin.x, _origin.y, )
}

void DrawNode(const Node& _node, SDL_Color _color)
{
    auto _radius  = static_cast<int>(_node.radius);
    auto _centerX = static_cast<int>(_node.position.x);
    auto _centerY = static_cast<int>(_node.position.y);

    for(int x = -_radius; x <= _radius; x++)
    {
        int height = (int)sqrt(_radius * _radius - x * x);

        for(int y = -height; y <= height; y++)
        {
            if(_node.isSelected)
                SDL_SetRenderDrawColor(RENDER, _color.r, 0, 0, 255);
            else
                SDL_SetRenderDrawColor(RENDER, _color.r, _color.g, _color.b, _color.a);

            SDL_RenderDrawPoint   (RENDER, _centerX + x, _centerY + y);
        }
    }
}

void DrawLine(SDL_Point _origin, SDL_Point _end, SDL_Color _color)
{
    SDL_SetRenderDrawColor(RENDER, _color.r, _color.g, _color.b, _color.a);
    SDL_RenderDrawLine(RENDER, _origin.x, _origin.y, _end.x, _end.y);
}

int main()
{
    InitSDL();

    Segment _raySegment;

    _raySegment.origin.radius = 10;
    _raySegment.end   .radius = 10;
    _raySegment.tNode .radius = 10;

    _raySegment.tValue = 100.5f;

    _raySegment.origin.position = { 0, 0 };
    _raySegment.end.position = {
            static_cast<float>(SCREEN_WIDTH  / 2.f),
            static_cast<float>(SCREEN_HEIGHT / 2.f)
    };

    _raySegment.tNode.position = _raySegment.origin.position + (_raySegment.Direction() * _raySegment.tValue);

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
                    for(Node* _node : Node::ALL_NODES)
                    {
                        if(CheckPointInsideNode(*_node, MOUSE_POSITION))
                            _node->isSelected = true;
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    for(Node* _node : Node::ALL_NODES)
                        _node->isSelected = false;
                    break;
            }
        }

        for(Node* _node : Node::ALL_NODES)
        {
            if(_node->isSelected)
                _node->position = {
                    static_cast<float>(MOUSE_POSITION.x),
                    static_cast<float>(MOUSE_POSITION.y)
            };
        }

        _raySegment.tValue = (SCREEN_WIDTH / 2.f - _raySegment.origin.position.x) / _raySegment.Direction().x;

        _raySegment.tNode.position = _raySegment.origin.position + (_raySegment.Direction() * _raySegment.tValue);

        SDL_SetRenderDrawColor(RENDER, 0, 0, 0, 0);
        SDL_RenderClear(RENDER);

        DrawLine(_raySegment.origin.AsSDL_Point(), _raySegment.end.AsSDL_Point(), { 255, 255, 255, 255} );
        DrawNode(_raySegment.origin, { 255,   0,   0, 255 } );
        DrawNode(_raySegment.end   , { 255, 255, 255, 255 } );
        DrawNode(_raySegment.tNode , { 255, 255, 255, 255 } );

        SDL_RenderPresent(RENDER);
    }

    SDL_DestroyRenderer(RENDER);
    SDL_DestroyWindow(WINDOW);
    return 0;
}
