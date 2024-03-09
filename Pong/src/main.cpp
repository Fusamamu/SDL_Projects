#include<iostream>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_ttf.h>
#include"Render.h"

struct Time
{
    float deltaTime;
    float tickCount;
    
    void Tick()
    {
        while(!SDL_TICKS_PASSED(SDL_GetTicks(), tickCount + 16));
            
        deltaTime = (SDL_GetTicks() - tickCount)/1000.0f;
       
        if(deltaTime > 0.05f)
            deltaTime = 0.05f;

        tickCount = SDL_GetTicks();
    }
};

enum Button
{
    ARROW_UP = 0,
    ARROW_DOWN
};

const int SCREEN_WIDTH  = 640;
const int SCREEN_HEIGHT = 480;

const int FRAME_THICKNESS = 10;

const int BALL_SPEED = 2;
const int BALL_DIR[2] = {1, 1};

const float DELTA_TIME = 0.05;

void DrawRect(SDL_Renderer* _render, SDL_Rect _rect)
{
    SDL_SetRenderDrawColor(_render, 255, 255, 255, 255);
    SDL_RenderDrawRect(_render, &_rect);
    SDL_RenderFillRect(_render, &_rect);
}

void DrawFrame(SDL_Renderer* _render)
{
    SDL_Rect 
         _leftFrame,
         _rightFrame, 
         _topFrame,
         _bottomFrame;

    _leftFrame.x = 0;
    _leftFrame.y = 0;
    _leftFrame.w = FRAME_THICKNESS;
    _leftFrame.h = SCREEN_HEIGHT;
   
    _rightFrame.x = SCREEN_WIDTH - FRAME_THICKNESS;
    _rightFrame.y = 0;
    _rightFrame.w = FRAME_THICKNESS;
    _rightFrame.h = SCREEN_HEIGHT; 
    
    _topFrame.x = 0;
    _topFrame.y = 0;
    _topFrame.w = SCREEN_WIDTH;
    _topFrame.h = FRAME_THICKNESS;

    _bottomFrame.x = 0;
    _bottomFrame.y = SCREEN_HEIGHT - FRAME_THICKNESS;
    _bottomFrame.w = SCREEN_WIDTH;
    _bottomFrame.h = FRAME_THICKNESS;

    DrawRect(_render, _leftFrame);
    DrawRect(_render, _rightFrame);
    DrawRect(_render, _topFrame);
    DrawRect(_render, _bottomFrame);
}

struct Ball
{
    float xPos;
    float yPos;

    float velocity;
    int direction[2];

    SDL_Rect ballRect;
    SDL_Renderer* render;
    
    Ball(SDL_Renderer* _render, float _xPos, float _yPos): 
        render(_render),
        velocity(1),
        xPos(_xPos), 
        yPos(_yPos)
    {
        direction[0] = 1;
        direction[1] = 1;

        ballRect.x = 50;
        ballRect.y = 50;
        ballRect.w = 15;
        ballRect.h = 15;
    }

    void Update(float _deltaTime)
    {
        xPos += velocity * direction[0] * _deltaTime;
        yPos += velocity * direction[1] * _deltaTime; 
        
        if(xPos + ballRect.w > SCREEN_WIDTH - FRAME_THICKNESS)
            direction[0] = -1;
        if(xPos < FRAME_THICKNESS)
            direction[0] = 1;
        if(yPos < FRAME_THICKNESS)
            direction[1] = 1;
        if(yPos + ballRect.h > SCREEN_HEIGHT - FRAME_THICKNESS)
            direction[1] = -1;
            
        ballRect.x = static_cast<int>(xPos);
        ballRect.y = static_cast<int>(yPos);
    }
    
    void Reflect()
    {
        direction[0] *= -1;
    }
};

struct Paddle
{
    float xPos;
    float yPos;

    float width;
    float height;

    float velocity;
    int yDir;

    SDL_Rect paddleRect;

    Uint8* keyState;

    Paddle(float _xPos, float _yPos):
        xPos(_xPos),
        yPos(_yPos),
        width(14),
        height(60),
        velocity(500)

    {
        paddleRect.x = _xPos - static_cast<int>(width  / 2);
        paddleRect.y = _yPos - static_cast<int>(height / 2);
        paddleRect.w = width;
        paddleRect.h = height;    
    }
    
    void Update(float _deltaTime)
    {
        paddleRect.x = xPos - static_cast<int>(width  / 2);
        paddleRect.y = yPos - static_cast<int>(height / 2);
        paddleRect.w = width;
        paddleRect.h = height;    
    }
    
    void SetPositionX(float _posX)
    {
        xPos = _posX - static_cast<int>(width/2);
        paddleRect.x = xPos;
    }

    void SetPositionY(float _posY)
    {
        yPos = _posY;
        paddleRect.y = yPos - static_cast<int>(height/2);
    }    

    void MoveUp(float _deltaTime)
    {
        yPos -= velocity * _deltaTime;

        if(yPos - height/2 < FRAME_THICKNESS)
            yPos = FRAME_THICKNESS + height/2;

        paddleRect.y = static_cast<int>(yPos - height/2);
    }

    void MoveDown(float _deltaTime)
    {
        yPos += velocity * _deltaTime;
        
        if(yPos + height/2 > SCREEN_HEIGHT - FRAME_THICKNESS)
            yPos = SCREEN_HEIGHT - FRAME_THICKNESS - height/2;

        paddleRect.y = static_cast<int>(yPos - height/2);
    }
};

int main()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return(-1);
    }

    if (TTF_Init() < 0)
    {
        printf("SDL_ttf could not initialize! SDL_Error: %s\n", TTF_GetError());
        return(-1);
    }

    TTF_Font* _font;

    _font = TTF_OpenFont("res/fonts/open-sans/OpenSans-Bold.ttf", 24);
    if(!_font)
    {
        printf("Could not open font!\n");
        return(-1);
    }

    SDL_Window* _window = NULL;
    
    _window = SDL_CreateWindow
        (
            "SDL Tutorial", 
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED, 
            SCREEN_WIDTH,
            SCREEN_HEIGHT, 
            SDL_WINDOW_SHOWN
        );

    if (_window == NULL)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return(0);
    }  

    SDL_Renderer* _render = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_Surface* _textSurface;
    SDL_Color _color = { 0, 0, 0};

    _textSurface = TTF_RenderText_Solid(_font, "Hello world", _color);

    SDL_Texture* _textTexture = SDL_CreateTextureFromSurface(_render, _textSurface);
    SDL_Rect _dest = { 0, 0, _textSurface->w, _textSurface->h };
    SDL_RenderCopy(_render, _textTexture, NULL, &_dest);

    Time _time;

    Ball _newBall(_render, 0.0f, 0.0f);
    _newBall.velocity = 100;

    Paddle _padA(FRAME_THICKNESS * 2, SCREEN_HEIGHT / 2);
    Paddle _padB(SCREEN_WIDTH - FRAME_THICKNESS * 2, SCREEN_HEIGHT / 2);

    bool buttons[4] = {};
        
    SDL_Event _e;
      
    bool _isRunning = true; 
   
    while (_isRunning) 
    {
        while (SDL_PollEvent(&_e)) 
        {
            switch(_e.type)
            {
                case SDL_QUIT:
                {
                    _isRunning = false;
                }break;
                case SDL_KEYDOWN:
                {
                    if(_e.key.keysym.sym == SDLK_ESCAPE)
                        _isRunning = false;
                    
                    if(_e.key.keysym.sym == SDLK_UP)
                        buttons[Button::ARROW_UP] = true;
                    
                    if(_e.key.keysym.sym == SDLK_DOWN)
                        buttons[Button::ARROW_DOWN] = true;     
                }break;
                case SDL_KEYUP:
                {
                    if(_e.key.keysym.sym == SDLK_UP)
                        buttons[Button::ARROW_UP] = false;
                    
                    if(_e.key.keysym.sym == SDLK_DOWN)
                        buttons[Button::ARROW_DOWN] = false;    
                }break;
            }
        }

        _time.Tick();

        _newBall.Update(_time.deltaTime);
        
        if(buttons[Button::ARROW_UP])
            _padA.MoveUp(_time.deltaTime);
        if(buttons[Button::ARROW_DOWN])
            _padA.MoveDown(_time.deltaTime);

        if(SDL_HasIntersection(&_newBall.ballRect, &_padA.paddleRect))
            _newBall.Reflect(); 
        if(SDL_HasIntersection(&_newBall.ballRect, &_padB.paddleRect))
            _newBall.Reflect();

        _padB.SetPositionY(_newBall.yPos);

        SDL_SetRenderDrawColor(_render, 0, 0, 0, 255);
        SDL_RenderClear(_render);

        DrawFrame(_render);           

        SDL_SetRenderDrawColor(_render, 255, 255, 255, 255); 
        DrawRect(_render, _newBall.ballRect);

        DrawRect(_render, _padA.paddleRect);
        DrawRect(_render, _padB.paddleRect);


        SDL_RenderPresent(_render);
    }
    
    SDL_DestroyTexture(_textTexture);
    SDL_FreeSurface(_textSurface);

    SDL_DestroyWindow(_window);
    SDL_Quit();
    return 0;
}

