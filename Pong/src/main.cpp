#include<iostream>
#include<string>
#include<functional>
#include<cstdlib>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_ttf.h>
#include"Render.h"

enum class GameState
{
    BEFORE_START, RUNNING, GAMEOVER, QUIT
};

struct GameContext
{
    int m_PlayerScore;
    int m_ComScore;

    GameState m_GameState;

}_GlobalGameContext;

enum class AIState
{
    IDLE, FOLLOW_BALL, MOVE_TO_BALL
}_AIState;

struct Time
{
    const float TIMER = 3;

    float deltaTime;
    float tickCount;

    float m_ElapsedCountDown;
    
    bool IsCountingDown;

    std::function<void()> OnTimeUp;
    
    Time(): 
        deltaTime(0),
        tickCount(0), 
        m_ElapsedCountDown(0),
        IsCountingDown(false)
    {
        m_ElapsedCountDown = TIMER;
    }

    void Tick()
    {
        while(!SDL_TICKS_PASSED(SDL_GetTicks(), tickCount + 16));
            
        deltaTime = (SDL_GetTicks() - tickCount)/1000.0f;
       
        if(deltaTime > 0.05f)
            deltaTime = 0.05f;

        tickCount = SDL_GetTicks();
    }

    void UpdateTimer()
    {
        m_ElapsedCountDown -= deltaTime;
        if(m_ElapsedCountDown <= 0.0f)
        {
            m_ElapsedCountDown = TIMER;
            OnTimeUp();
        }       
    }
    
    unsigned int UpdateCountDown(std::function<void()> _onTimeUp)
    {
        if(!IsCountingDown)
        {
            IsCountingDown = true;
            m_ElapsedCountDown = TIMER;
        }

        m_ElapsedCountDown -= deltaTime;
       
        if(m_ElapsedCountDown <= 0)
        {
            IsCountingDown = false;

            m_ElapsedCountDown = TIMER;

            if(_onTimeUp)
                _onTimeUp();
        } 
        return static_cast<unsigned int>(m_ElapsedCountDown);
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


typedef void (*OnHitLeftWall)  (GameContext*);
typedef void (*OnHitRightWall) (GameContext*);

struct Ball
{
    float xPos;
    float yPos;

    float velocity;
    int direction[2];

    SDL_Rect ballRect;
    SDL_Renderer* render;
   
    GameContext* m_GameContext;

    OnHitLeftWall  OnHitLeftWallCallback;
    OnHitRightWall OnHitRightWallCallback;
    
    Ball(SDL_Renderer* _render, float _xPos, float _yPos, float _velocity): 
        render(_render),
        velocity(_velocity),
        xPos(_xPos), 
        yPos(_yPos)
    {
        direction[0] = 1;
        direction[1] = 1;

        ballRect.x = 0;
        ballRect.y = 0;
        ballRect.w = 15;
        ballRect.h = 15;
    }

    void Update(float _deltaTime)
    {
        xPos += velocity * direction[0] * _deltaTime;
        yPos += velocity * direction[1] * _deltaTime; 
        
        if(xPos + ballRect.w > SCREEN_WIDTH - FRAME_THICKNESS)
        {
            direction[0] = -1;
            if(OnHitRightWallCallback)
                OnHitRightWallCallback(m_GameContext);
        }
        if(xPos < FRAME_THICKNESS)
        {
            direction[0] = 1;
            if(OnHitLeftWallCallback)
                OnHitLeftWallCallback(m_GameContext);
        }
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

    SDL_Rect paddleRect;
    Uint8* keyState;
        
    float loopTime;
    float elapsedLoop;

    bool IsDirUp;
    bool IsMovingToPos;

    Paddle(float _xPos, float _yPos):
        xPos(_xPos),
        yPos(_yPos),
        width(14),
        height(60),
        velocity(500),
        loopTime(0.25f),
        elapsedLoop(0.0f)
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
        IsDirUp = true;

        yPos -= velocity * _deltaTime;

        if(yPos - height/2 < FRAME_THICKNESS)
            yPos = FRAME_THICKNESS + height/2;

        paddleRect.y = static_cast<int>(yPos - height/2);
    }

    void MoveDown(float _deltaTime)
    {
        IsDirUp = false;

        yPos += velocity * _deltaTime;
        
        if(yPos + height/2 > SCREEN_HEIGHT - FRAME_THICKNESS)
            yPos = SCREEN_HEIGHT - FRAME_THICKNESS - height/2;

        paddleRect.y = static_cast<int>(yPos - height/2);
    }

    void MoveUpDownInLoop(float _deltaTime)
    {
        elapsedLoop += _deltaTime;

        if(elapsedLoop < loopTime)
        {
            if(!IsDirUp)
                MoveDown(0.15f * _deltaTime);
            else
                MoveUp(0.15f * _deltaTime);
        }           
        else
        {
            elapsedLoop = 0;
            IsDirUp = !IsDirUp;
        }     
    }

    void MoveToPosY(float _targetPosY, float _deltaTime)
    {
        if(_targetPosY > yPos)
            MoveDown(_deltaTime);
        else
            MoveUp(_deltaTime);
        
        if(std::abs(_targetPosY - yPos) < 0.25f)
        {
            _AIState = AIState::FOLLOW_BALL;
        }
    } 
};

enum class TextAlignment
{
    LEFT, CENTER, RIGHT
};

void RenderTextTTF(
        SDL_Renderer* _render,
        int _x, 
        int _y,
        const char* _text,
        TTF_Font*  _font, 
        TextAlignment _alignment)
{
    SDL_Surface* _surface = TTF_RenderText_Solid(_font, _text, {255, 255, 255});
    SDL_Texture* _texture = SDL_CreateTextureFromSurface(_render, _surface);
  
    SDL_Rect _destRect;

    switch(_alignment)
    {
        case TextAlignment::LEFT:
            _destRect.x = _x;
            break;
        case TextAlignment::CENTER:
            _destRect.x= _x - _surface->w / 2;
            break;
        case TextAlignment::RIGHT:
            _destRect.x= _x + _surface->w;
            break;
    } 

    _destRect.y = _y; 
    _destRect.w = _surface->w;
    _destRect.h = _surface->h;

    SDL_RenderCopy(_render, _texture, NULL, &_destRect);
    SDL_FreeSurface   (_surface);
    SDL_DestroyTexture(_texture);
}

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

    SDL_Window* _window = NULL;
    
    _window = SDL_CreateWindow
        (
            "PONG", 
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

    TTF_Font* _font = TTF_OpenFont("res/fonts/open-sans/OpenSans-Bold.ttf", 24);
    if(!_font)
    {
        printf("Could not open font!\n");
        return(-1);
    }

    TTF_SetFontSize(_font, 30);
    
    _GlobalGameContext.m_GameState = GameState::BEFORE_START;    

    _AIState = AIState::FOLLOW_BALL;

    Time _time;

    _time.OnTimeUp = [&](){
        
        if(_AIState != AIState::MOVE_TO_BALL)
        {
            if(_AIState == AIState::FOLLOW_BALL)
                _AIState = AIState::IDLE;
            else if (_AIState == AIState::IDLE)
                _AIState = AIState::MOVE_TO_BALL;
        }
    };

    unsigned int _playerScore = 0;
    unsigned int _comScore    = 0;

    Ball _ball (_render, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 100.0f); 
    
    _ball.m_GameContext = &_GlobalGameContext;

    _ball.OnHitLeftWallCallback = [](GameContext* _context){
        _context->m_GameState = GameState::GAMEOVER;        
    };   

    Paddle _padLeft  (FRAME_THICKNESS * 2, SCREEN_HEIGHT / 2);
    Paddle _padRight (SCREEN_WIDTH - FRAME_THICKNESS * 2, SCREEN_HEIGHT / 2);

    bool buttons[4] = {};
        
    SDL_Event _e;

    while (_GlobalGameContext.m_GameState != GameState::QUIT) 
    {
        _time.Tick();
        
        while (SDL_PollEvent(&_e)) 
        {
            switch(_e.type)
            {
                case SDL_QUIT:
                {
                    _GlobalGameContext.m_GameState = GameState::QUIT;
                }
                break;
                case SDL_KEYDOWN:
                {
                    if(_e.key.keysym.sym == SDLK_ESCAPE)
                       _GlobalGameContext.m_GameState = GameState::QUIT; 
                    
                    if(_e.key.keysym.sym == SDLK_UP)
                        buttons[Button::ARROW_UP] = true;
                    
                    if(_e.key.keysym.sym == SDLK_DOWN)
                        buttons[Button::ARROW_DOWN] = true;     
                }
                break;
                case SDL_KEYUP:
                {
                    if(_e.key.keysym.sym == SDLK_UP)
                        buttons[Button::ARROW_UP] = false;
                    
                    if(_e.key.keysym.sym == SDLK_DOWN)
                        buttons[Button::ARROW_DOWN] = false;    
                }
                break;
            }
        }
        
        switch(_GlobalGameContext.m_GameState)
        {
            case GameState::BEFORE_START:

//                 auto _onTimeUp = [&]()
//                 {
//                     _GlobalGameContext.m_GameState = GameState::RUNNING;
//                 };
//                 unsigned int _countDown = _time.UpdateCountDown(_onTimeUp);

                 break;

            case GameState::RUNNING:

                if(buttons[Button::ARROW_UP])
                    _padLeft.MoveUp(_time.deltaTime);
                if(buttons[Button::ARROW_DOWN])
                    _padLeft.MoveDown(_time.deltaTime);

                switch(_AIState)
                {
                    case AIState::FOLLOW_BALL:
                        _padRight.SetPositionY(_ball.yPos);
                        break;
                    case AIState::IDLE:
                        _padRight.MoveUpDownInLoop(_time.deltaTime);
                        break;
                    case AIState::MOVE_TO_BALL:
                        _padRight.MoveToPosY(_ball.yPos, _time.deltaTime);
                        break;
                }

                _time.UpdateTimer();
                _ball.Update(_time.deltaTime);

                /*--------------Check collision section--------------*/
                if(SDL_HasIntersection(&_ball.ballRect, &_padLeft.paddleRect))
                    _ball.Reflect(); 
                if(SDL_HasIntersection(&_ball.ballRect, &_padRight.paddleRect))
                    _ball.Reflect();
                /*---------------------------------------------------*/

                break;
                
            case GameState::GAMEOVER:
                break;
            case GameState::QUIT:
                break;
        }

        /*--------------Render section-----------------------*/
        SDL_SetRenderDrawColor(_render, 0, 0, 0, 255);
        SDL_RenderClear(_render);
        
        DrawFrame(_render);           

        SDL_SetRenderDrawColor(_render, 255, 255, 255, 255); 
        DrawRect(_render, _ball.ballRect);

        DrawRect(_render, _padLeft.paddleRect );
        DrawRect(_render, _padRight.paddleRect);

        std::string _scoreString = std::to_string(_GlobalGameContext.m_PlayerScore) + " : 0";
        const char* _scoreText = _scoreString.c_str();

        RenderTextTTF(
                _render,
                SCREEN_WIDTH / 2,
                FRAME_THICKNESS * 2,
                _scoreText,
                _font,
                TextAlignment::CENTER
                );
       
        if(_GlobalGameContext.m_GameState == GameState::BEFORE_START)
        {
            RenderTextTTF(                           
                _render,                     
                SCREEN_WIDTH  / 2,           
                SCREEN_HEIGHT / 2,           
                "3",                 
                _font,                       
                TextAlignment::CENTER        
                );                           
        } 
        
        if(_GlobalGameContext.m_GameState == GameState::GAMEOVER)
        {
            RenderTextTTF(                           
                _render,                     
                SCREEN_WIDTH  / 2,           
                SCREEN_HEIGHT / 2,           
                "GAME OVER",                 
                _font,                       
                TextAlignment::CENTER        
                );                           
        }
        
        SDL_RenderPresent(_render);
        /*--------------------------------------------------*/
    }
    
    TTF_CloseFont(_font);
    SDL_DestroyWindow(_window);
    
    TTF_Quit();
    SDL_Quit();
    return 0;
}

