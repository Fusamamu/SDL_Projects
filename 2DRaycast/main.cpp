#include <iostream>
#include <vector>
#include <list>
#include <cmath>
#include <SDL2/SDL.h>

const unsigned int SCREEN_WIDTH  = 900;
const unsigned int SCREEN_HEIGHT = 900;

SDL_Window*   WINDOW = nullptr;
SDL_Renderer* RENDER = nullptr;
SDL_Event EVENT;
SDL_Point MOUSE_POSITION;

SDL_Color RED     = {255, 0, 0, 255};
SDL_Color GREEN   = {0, 255, 0, 255};
SDL_Color BLUE    = {0, 0, 255, 255};
SDL_Color BLACK   = {0, 0, 0, 255};
SDL_Color WHITE   = {255, 255, 255, 255};
SDL_Color YELLOW  = {255, 255, 0, 255};
SDL_Color CYAN    = {0, 255, 255, 255};
SDL_Color MAGENTA = {255, 0, 255, 255};

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
                    "2D Raycast",
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
    bool isVisible;

    static std::vector<Node*> ALL_NODES;

    Node(): position({0, 0}), radius(1.f), isSelected(false), isVisible(false)
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

    void Reset()
    {
        isSelected = false;
        isVisible  = false;
    }

    [[nodiscard]]
    SDL_Point AsSDL_Point() const
    {
        return {static_cast<int>(position.x),
                static_cast<int>(position.y) };
    }
};

std::vector<Node*> Node::ALL_NODES;

struct NodePool
{
    std::list<Node*> Pool;
    std::vector<Node*> activeNodes;

    const size_t POOL_COUNT = 50;

    NodePool()
    {
        for(size_t _i = 0; _i < POOL_COUNT; ++_i)
            Pool.push_back(new Node);
    }

    Node* GetNode()
    {
        if (Pool.empty())
        {
            Node* _newNode = new Node;
            _newNode->isVisible = true;
            Pool.push_back(_newNode);
            return _newNode;
        }
        else
        {
            Node* _node = Pool.front();
            _node->isVisible = true;
            Pool.pop_front();
            activeNodes.push_back(_node);
            return _node;
        }
    }

    void ReturnNode(Node* _node)
    {
        _node->Reset();
        Pool.push_back(_node);
    }

    void ResetPool()
    {
        for(Node* _node : activeNodes)
            ReturnNode(_node);
        activeNodes.clear();
    }

}_NodePool;

bool CheckPointInsideNode(const Node& node, SDL_Point  _point)
{
    SDL_Point _position ={
            static_cast<int>(node.position.x),
            static_cast<int>(node.position.y)
    };

    float _radius = node.radius;

    int _sqrtDist = (_point.x - _position.x) * (_point.x - _position.x) +
                    (_point.y - _position.y) * (_point.y - _position.y);

    if(static_cast<float>(_sqrtDist) < _radius * _radius)
        return true;
    return false;
}

struct AABB
{
    SDL_Rect rect;
    SDL_Point center;

    int width;
    int height;

    Node* nodeMinX;
    Node* nodeMaxX;
    Node* nodeMinY;
    Node* nodeMaxY;

    AABB(): rect({0, 0, 0, 0}), center({0, 0}), width(0), height(0)
    {
        nodeMinX = _NodePool.GetNode();
        nodeMaxX = _NodePool.GetNode();
        nodeMinY = _NodePool.GetNode();
        nodeMaxY = _NodePool.GetNode();
    }

    const SDL_Rect& GetRect()
    {
        rect.x = center.x - width  / 2;
        rect.y = center.y - height / 2;
        rect.w = width;
        rect.h = height;
        return rect;
    }

    float LeftSide() const
    {
        return (float)center.x - (float)width / 2.f;
    }
    
    float RightSide() const
    {
        return (float)center.x + (float)width / 2.f;
    }

    float TopSide() const
    {
        return (float)center.y - (float)height / 2.f;
    }

    float BottomSide() const
    {
        return (float)center.y + (float)height / 2.f;
    }
};

struct Segment
{
    Node origin;
    Node end;

    [[nodiscard]]
    Vector Direction() const
    {
        Vector _dir {
            end.position.x - origin.position.x,
            end.position.y - origin.position.y
        };
        return _dir.normalized();
    }
};

void DrawAABB(AABB& _aabb)
{
    SDL_SetRenderDrawColor(RENDER, 255, 255, 255, 255);
    SDL_RenderDrawRect(RENDER, &_aabb.GetRect());
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

            SDL_RenderDrawPoint(RENDER, _centerX + x, _centerY + y);
        }
    }
}

void DrawLine(SDL_Point _origin, SDL_Point _end, SDL_Color _col)
{
    SDL_SetRenderDrawColor(RENDER, _col.r, _col.g, _col.b, _col.a);
    SDL_RenderDrawLine(RENDER, _origin.x, _origin.y, _end.x, _end.y);
}

void DrawDashedLine(SDL_Point _origin, SDL_Point _end, SDL_Color _col)
{
    bool draw = true;

    int dx = _end.x - _origin.x;
    int dy = _end.y - _origin.y;
    int steps = std::max(abs(dx), abs(dy));

    float xIncrement = dx / (float)steps;
    float yIncrement = dy / (float)steps;

    float x = _origin.x;
    float y = _origin.y;

    SDL_SetRenderDrawColor(RENDER, _col.r, _col.g, _col.b, SDL_ALPHA_OPAQUE);

    for (int i = 0; i < steps; ++i)
    {
        if (draw)
            SDL_RenderDrawPoint(RENDER, (int)x, (int)y);

        x += xIncrement;
        y += yIncrement;
        draw = !draw;
    }
}

void DrawInfiniteLine(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b) {
    // Calculate the clipping region
    int minX = 0;
    int minY = 0;
    int maxX = SCREEN_WIDTH - 1;
    int maxY = SCREEN_HEIGHT - 1;

    // Clip the line to the visible region
    // Calculate the slope (m) and intercept (b) of the line y = mx + b
    float slope = (float)(y2 - y1) / (x2 - x1);
    float intercept = y1 - slope * x1;

    // Calculate intersections with screen edges
    int xLeft = minX;
    int yLeft = slope * xLeft + intercept;
    int xRight = maxX;
    int yRight = slope * xRight + intercept;
    int yTop = minY;
    int xTop = (yTop - intercept) / slope;
    int yBottom = maxY;
    int xBottom = (yBottom - intercept) / slope;

    // Check which intersection points are within the screen bounds
    bool leftInside = (yLeft >= minY && yLeft <= maxY);
    bool rightInside = (yRight >= minY && yRight <= maxY);
    bool topInside = (xTop >= minX && xTop <= maxX);
    bool bottomInside = (xBottom >= minX && xBottom <= maxX);

    // Draw the line segment within the visible region
    SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);
    if (leftInside && rightInside) {
        SDL_RenderDrawLine(renderer, xLeft, yLeft, xRight, yRight);
    } else if (topInside && bottomInside) {
        SDL_RenderDrawLine(renderer, xTop, yTop, xBottom, yBottom);
    }
}

void DrawInfiniteDashedLine(SDL_Renderer* renderer, SDL_Point _origin, SDL_Point _end, SDL_Color _col)
{
    // Calculate the clipping region
    int minX = 0;
    int minY = 0;
    int maxX = SCREEN_WIDTH - 1;
    int maxY = SCREEN_HEIGHT - 1;

    float _x1 = static_cast<float>(_origin.x);
    float _y1 = static_cast<float>(_origin.y);
    float _x2 = static_cast<float>(_end.x);
    float _y2 = static_cast<float>(_end.y);

    // Calculate the slope (m) and intercept (b) of the line y = mx + b
    float slope = (float)(_y2 - _y1) / (_x2 - _x1);
    float intercept = _y1 - slope * _x1;

    // Length of a dash segment
    int dashLength = 10;

    // Calculate the total length of the line
    float totalLength = sqrt(pow(_x2 - _x1, 2) + pow(_y2 - _y1, 2));

    // Calculate the unit vector along the line
    float unitX = (_x2 - _x1) / totalLength;
    float unitY = (_y2 - _y1) / totalLength;

    // Start drawing from the beginning of the line
    float currentX = _x1;
    float currentY = _y1;

    // Draw segments of the line
    SDL_SetRenderDrawColor(renderer, _col.r, _col.g, _col.b, SDL_ALPHA_OPAQUE);

    bool draw = true;

    while (currentX >= minX && currentX <= maxX && currentY >= minY && currentY <= maxY)
    {
        float nextX = currentX + unitX * dashLength;
        float nextY = currentY + unitY * dashLength;

        // Clip the segment to the visible region
        if (nextX < minX || nextX > maxX || nextY < minY || nextY > maxY) {
            // Calculate the intersection point with the screen edges
            if (currentX < minX) {
                currentY = slope * (minX - intercept) + 0.5; // +0.5 to round to nearest pixel
                currentX = minX;
            } else if (currentX > maxX) {
                currentY = slope * (maxX - intercept) + 0.5;
                currentX = maxX;
            } else if (currentY < minY) {
                currentX = (minY - intercept) / slope + 0.5;
                currentY = minY;
            } else if (currentY > maxY) {
                currentX = (maxY - intercept) / slope + 0.5;
                currentY = maxY;
            }

            nextX = currentX + unitX * dashLength;
            nextY = currentY + unitY * dashLength;
        }

        // Draw the dash segment
        if (draw)
            SDL_RenderDrawLine(renderer, (int)currentX, (int)currentY, (int)nextX, (int)nextY);

        // Move to the next segment
        currentX = nextX;
        currentY = nextY;

        // Toggle drawing state for the next segment
        draw = !draw;
    }
}

void DrawAxisY(SDL_Renderer* _renderer, float _x, SDL_Color _col)
{
    int _targetX = static_cast<int>(_x);
    SDL_SetRenderDrawColor(_renderer, _col.r, _col.g, _col.b, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(_renderer, _x, 0, _x, SCREEN_HEIGHT);
}

void DrawAxisX(SDL_Renderer* _renderer, float _y, SDL_Color _col)
{
    int _targetY = static_cast<int>(_y);
    SDL_SetRenderDrawColor(_renderer, _col.r, _col.g, _col.b, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawLine(_renderer, 0, _targetY, SCREEN_WIDTH, _targetY);
}

void CheckRayAABBCollision(const Segment& _raySegment, const AABB& _box)
{
    float _t_minX = -1;
    float _t_maxX = -1;
    float _t_minY = -1;
    float _t_maxY = -1;

    if(_raySegment.Direction().x != 0)
    {
        _t_minX = (_box.LeftSide()  - _raySegment.origin.position.x) / _raySegment.Direction().x;
        _t_maxX = (_box.RightSide() - _raySegment.origin.position.x) / _raySegment.Direction().x;

        _box.nodeMinX->radius = 5.f;
        _box.nodeMaxX->radius = 5.f;

        _box.nodeMinX->position = _raySegment.origin.position + (_raySegment.Direction() * _t_minX);
        _box.nodeMaxX->position = _raySegment.origin.position + (_raySegment.Direction() * _t_maxX);
    }

    if(_raySegment.Direction().y != 0)
    {
        _t_minY = (_box.TopSide()    - _raySegment.origin.position.y) / _raySegment.Direction().y;
        _t_maxY = (_box.BottomSide() - _raySegment.origin.position.y) / _raySegment.Direction().y;

        _box.nodeMinY->radius = 5.f;
        _box.nodeMaxY->radius = 5.f;

        _box.nodeMinY->position = _raySegment.origin.position + (_raySegment.Direction() * _t_minY);
        _box.nodeMaxY->position = _raySegment.origin.position + (_raySegment.Direction() * _t_maxY);
    }
}

int main()
{
    InitSDL();

    std::vector<AABB> _boxes;

    for(size_t _i = 0; _i < 5; ++_i)
    {
        AABB _box { };
        _box.center.x = 0;
        _box.center.y = 0;
        _box.width  = 100;
        _box.height = 100;

        _boxes.push_back(_box);
    }

    _boxes[0].center.x = SCREEN_WIDTH  / 2;
    _boxes[0].center.y = SCREEN_HEIGHT / 2;

    _boxes[1].center.x = 300;
    _boxes[1].center.y = 300;
    _boxes[2].center.x = SCREEN_WIDTH - 300;
    _boxes[2].center.y = 300;

    _boxes[3].center.x = 300;
    _boxes[3].center.y = SCREEN_HEIGHT - 300;
    _boxes[4].center.x = SCREEN_WIDTH  - 300;
    _boxes[4].center.y = SCREEN_HEIGHT - 300;

    Segment _raySegment;

    _raySegment.origin.radius = 10;
    _raySegment.end   .radius = 10;

    _raySegment.origin.isVisible = true;
    _raySegment.end   .isVisible = true;

    _raySegment.origin.position = { 0, 0 };
    _raySegment.end.position = {
            static_cast<float>(SCREEN_WIDTH  / 2.f),
            static_cast<float>(SCREEN_HEIGHT / 2.f)
    };

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

        for(const AABB& _box : _boxes)
            CheckRayAABBCollision(_raySegment, _box);

        SDL_SetRenderDrawColor(RENDER, 0, 0, 0, 0);
        SDL_RenderClear(RENDER);

        for(AABB& _box : _boxes)
        {
            DrawAxisX(RENDER, _box.TopSide()   , YELLOW);
            DrawAxisX(RENDER, _box.BottomSide(), YELLOW);
            DrawAxisY(RENDER, _box.LeftSide()  , BLUE);
            DrawAxisY(RENDER, _box.RightSide() , BLUE);

            DrawAABB(_box);
        }

        DrawLine(_raySegment.origin.AsSDL_Point(), _raySegment.end.AsSDL_Point(), { 255, 255, 255, 255} );
        DrawNode(_raySegment.origin, { 255,   0,   0, 255 } );
        DrawNode(_raySegment.end   , { 255, 255, 255, 255 } );

        for(Node* _node : Node::ALL_NODES)
            DrawNode(*_node, WHITE);

        SDL_RenderPresent(RENDER);

        _NodePool.ResetPool();
    }

    SDL_DestroyRenderer(RENDER);
    SDL_DestroyWindow(WINDOW);
    return 0;
}
