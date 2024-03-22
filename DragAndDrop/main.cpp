#include <vector>
#include <SDL2/SDL.h>

class GameObject;
std::vector<GameObject> ALL_OBJECTS;

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

class GameObject
{
public:
    SDL_Point Position;
    SDL_Rect BBox;
    SDL_Color CurrentColor;

    bool IsMouseDown;
    GameObject(): Position({ 0, 0 }), BBox({ 0, 0, 100, 100}), IsMouseDown(false), CurrentColor({255, 255, 255, 255 })
    {

    }
    ~GameObject() = default;

    void SetMouseDown(bool _value)
    {
        IsMouseDown = _value;
        if(IsMouseDown)
            CurrentColor = m_mouseDownColor;
        else
            CurrentColor = m_mouseUpColor;
    }

    void SetPosition(const SDL_Point& _position)
    {
        BBox.x = _position.x - BBox.w / 2;
        BBox.y = _position.y - BBox.h / 2;
    }
private:

    SDL_Color m_mouseDownColor { 255, 0, 0, 255 };
    SDL_Color m_mouseUpColor   { 255, 255, 255, 255 };
};

void DrawGameObject(const GameObject& _gameObject)
{
    SDL_Color _col = _gameObject.CurrentColor;
    SDL_SetRenderDrawColor(RENDER, _col.r, _col.g, _col.b, _col.a);
    SDL_RenderFillRect(RENDER, &_gameObject.BBox);
}

int main()
{
    InitSDL();

    GameObject* _currentSelectedObject = nullptr;

    for(size_t _i = 0; _i < 5; ++_i)
    {
        GameObject _gameObject;
        _gameObject.BBox.x = _i * (10 + _gameObject.BBox.w);
        ALL_OBJECTS.push_back(_gameObject);
    }

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
                    for(GameObject& _gameObject: ALL_OBJECTS)
                    {
                        if(SDL_PointInRect(&MOUSE_POSITION, &_gameObject.BBox))
                        {
                            _currentSelectedObject = &_gameObject;
                            _currentSelectedObject->SetMouseDown(true);
                        }
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    if(_currentSelectedObject != nullptr)
                    {
                        _currentSelectedObject->SetMouseDown(false);
                        _currentSelectedObject = nullptr;
                    }
                    break;
            }
        }

        if(_currentSelectedObject && _currentSelectedObject->IsMouseDown)
        {
            SDL_GetMouseState(&MOUSE_POSITION.x, &MOUSE_POSITION.y);
            _currentSelectedObject->SetPosition(MOUSE_POSITION);
        }

        SDL_SetRenderDrawColor(RENDER, 0, 0, 0, 0);
        SDL_RenderClear(RENDER);

        for(GameObject& _gameObject : ALL_OBJECTS)
            DrawGameObject(_gameObject);

        SDL_RenderPresent(RENDER);
    }

    SDL_DestroyRenderer(RENDER);
    SDL_DestroyWindow(WINDOW);
    return -1;
}
