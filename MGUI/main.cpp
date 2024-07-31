#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <MGUI.h>

const unsigned int SCREEN_WIDTH  = 1600;
const unsigned int SCREEN_HEIGHT = 900;

SDL_Window*   WINDOW = nullptr;
SDL_Renderer* RENDER = nullptr;
SDL_Event EVENT;
SDL_Point MOUSE_POSITION;

SDL_Window* LAUNCH_WINDOW   = nullptr;
SDL_Renderer* LAUNCH_RENDER = nullptr;

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

    int imgFlags = IMG_INIT_PNG;
    if( !( IMG_Init( imgFlags ) & imgFlags ) )
    {
        printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
        return;
    }

    if ( TTF_Init() < 0 )
    {
        printf("SDL_TTF could not initialize! SDL_Error: %s\n", TTF_GetError());
        return;
    }

//    LAUNCH_WINDOW = SDL_CreateWindow
//            (
//                    "Tile Editor",
//                    SDL_WINDOWPOS_UNDEFINED,
//                    SDL_WINDOWPOS_UNDEFINED,
//                    300,
//                    300,
//                    SDL_WINDOW_BORDERLESS
//            );
//    LAUNCH_RENDER = SDL_CreateRenderer(LAUNCH_WINDOW, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
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

int main()
{
    InitSDL();

//    bool _launching = true;
//    while(_launching)
//    {
//        _Timer.Tick();
//
//        while(SDL_PollEvent(&EVENT))
//        {
//            switch(EVENT.type)
//            {
//                case SDL_QUIT:
//                {
//                    _launching = false;
//                }
//                    break;
//                case SDL_KEYDOWN:
//                {
//                    if(EVENT.key.keysym.sym == SDLK_ESCAPE)
//                        _launching = false;
//                }
//                    break;
//            }
//        }
//
//
//        SDL_SetRenderDrawColor(LAUNCH_RENDER, 255, 15, 28, SDL_ALPHA_OPAQUE);
//        SDL_RenderClear(LAUNCH_RENDER);
//        SDL_RenderPresent(LAUNCH_RENDER);
//    }

    WINDOW = SDL_CreateWindow
            (
                    "Tile Editor",
                    SDL_WINDOWPOS_UNDEFINED,
                    SDL_WINDOWPOS_UNDEFINED,
                    SCREEN_WIDTH,
                    SCREEN_HEIGHT,
                    SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED
            );

    if(WINDOW == nullptr)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    RENDER = SDL_CreateRenderer(WINDOW, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    MGUI::Init(WINDOW, RENDER);
    //MGUI::gRender = RENDER;

    bool _isRunning = true;
    while(_isRunning)
    {
        _Timer.Tick();
        MGUI::BeginFrame();
        SDL_GetMouseState(&MOUSE_POSITION.x, &MOUSE_POSITION.y);
        while(SDL_PollEvent(&EVENT))
        {
            switch(EVENT.type)
            {
                case SDL_QUIT:
                    {
                        _isRunning = false;
                    }
                    break;
                case SDL_KEYDOWN:
                    {
                        if(EVENT.key.keysym.sym == SDLK_ESCAPE)
                            _isRunning = false;
                    }
                    break;
                case SDL_KEYUP:
                    break;
                case SDL_MOUSEMOTION:
                    MGUI::UI_State.MouseX = EVENT.motion.x;
                    MGUI::UI_State.MouseY = EVENT.motion.y;
                    MGUI::UI_State.MousePos = { EVENT.motion.x, EVENT.motion.y };
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if(EVENT.button.button == 1)
                        MGUI::UI_State.MouseDown = true;
                    break;
                case SDL_MOUSEBUTTONUP:
                    if(EVENT.button.button == 1)
                        MGUI::UI_State.MouseDown = false;
                    break;
            }
        }

        SDL_SetRenderDrawColor(RENDER, 107, 94, 74, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(RENDER);
//        MGUI::BeginWindow("WINDOW");
//            MGUI::Button("BUTTON");
//            MGUI::Label("Label");
//            MGUI::InputFloatField("x", 100.00);
//
//        MGUI::EndWindow();

        SDL_Rect _r { 100, 100, 400, 400 };
        SDL_RenderSetViewport(RENDER, &_r);

        SDL_SetRenderDrawColor(RENDER, 255, 94, 74, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(RENDER, &_r);

        SDL_RenderSetClipRect(RENDER, &_r);
        SDL_Rect _rr { 80, 100, 300, 20 };
        SDL_SetRenderDrawColor(RENDER, 255, 255, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(RENDER, &_rr);
        SDL_RenderSetClipRect(RENDER, nullptr);

        SDL_RenderSetViewport(RENDER, NULL);


        MGUI::BeginMenuBar(WINDOW);
        MGUI::MenuBarItem("File");
        MGUI::MenuBarItem("Edit");
        MGUI::MenuBarItem("View");
        MGUI::MenuBarItem("Help");
        MGUI::EndMenuBar();


        MGUI::BeginWindow("Property Inspector");
            MGUI::Button("BUTTON");
            MGUI::Label("Label");
            MGUI::InputFloatField("x", 100.00);
            MGUI::InputFloatField("x", 100.00);

            MGUI::Label("Transform", true);
            MGUI::InputFieldVec3("Position", 59.00);
            MGUI::InputFieldVec3("Rotation", 180.00);
            MGUI::InputFieldVec3("Scale"   , 1.00);

        MGUI::EndWindow();

        MGUI::BeginWindow("Navigator");

            MGUI::SelectableItem("Item A");
                MGUI::SelectableItem("Item AA", 1);
                MGUI::SelectableItem("Item AB", 1);
                    MGUI::SelectableItem("Item ABA", 2);
            MGUI::SelectableItem("Item B");
            MGUI::SelectableItem("Item C");
            MGUI::SelectableItem("Item D");

            MGUI::gItemLevel = 0;



        MGUI::EndWindow();

        SDL_RenderPresent(RENDER);
        MGUI::EndFrame();
    }

    SDL_DestroyRenderer(RENDER);
    SDL_DestroyWindow  (WINDOW);

    SDL_DestroyRenderer(LAUNCH_RENDER);
    SDL_DestroyWindow  (LAUNCH_WINDOW);

    RENDER = nullptr;
    WINDOW = nullptr;

    MGUI::Quit();
    return 0;
}
