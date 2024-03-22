#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <SDL2/SDL.h>

const unsigned int SCREEN_WIDTH  = 800;
const unsigned int SCREEN_HEIGHT = 600;

const int PADDING = 50;
const SDL_Rect SORT_FRAME { PADDING, PADDING, SCREEN_WIDTH - 2 * PADDING, SCREEN_HEIGHT - 2 * PADDING };

const size_t DATA_COUNT = 100;
const int COLUMN_WIDTH = SORT_FRAME.w / static_cast<int>(DATA_COUNT);

typedef struct Color {
    Uint8 r;
    Uint8 g;
    Uint8 b;
    Uint8 a;
} Color;

Color RED     = {255, 0, 0, 255};
Color GREEN   = {0, 255, 0, 255};
Color BLUE    = {0, 0, 255, 255};
Color BLACK   = {0, 0, 0, 255};
Color WHITE   = {255, 255, 255, 255};
Color YELLOW  = {255, 255, 0, 255};
Color CYAN    = {0, 255, 255, 255};
Color MAGENTA = {255, 0, 255, 255};

std::vector<int> GenerateRandomSequence(int _length)
{
    std::vector<int> _result;
    std::random_device _rd;
    std::mt19937 _gen(_rd());
    std::uniform_int_distribution<int> _dis(1, 100);

    _result.reserve(_length);
    for(int _i = 0; _i < _length; ++_i)
        _result.push_back(_dis(_gen));

    return _result;
}

std::vector<int> GenerateShuffleNumbers(int _length)
{
    std::vector<int> _result;

    _result.reserve(_length);
    for(int _i = 0; _i < _length; ++_i)
        _result.push_back(_i);

    std::random_device _rd;
    std::mt19937 _gen(_rd());
    std::shuffle(_result.begin(), _result.end(), _gen);

    return _result;
}

void InsertionSort(std::vector<int>& _arr)
{
    int _i, _j, _key;
    for(_i = 1; _i < _arr.size(); ++_i)
    {
        _key = _arr[_i];
        _j = _i - 1;

        while(_j >= 0 && _arr[_j] > _key)
        {
            _arr[_j+1] = _arr[_j];
            _j--;
        }

        _arr[_j+1] = _key;
    }
}

void Sort(std::vector<int>& _arr, int& _i, int& _j, int& _key, bool& _isSorting)
{
    if(_i != _arr.size())
    {
        if(!_isSorting)
        {
            _key = _arr[_i];
            _j = _i - 1;

            _isSorting = true;
        }

        if(_j >= 0 && _arr[_j] > _key)
        {
            _arr[_j+1] = _arr[_j];
            _j--;
        }
        else
        {
            _arr[_j+1] = _key;
            _i++;
            _isSorting = false;
        }
    }
}

void DrawDataGraph(SDL_Renderer* _render, int x, int y, int _dataSize, const Color& _color)
{
    SDL_Rect _rect {x, y, COLUMN_WIDTH, _dataSize };
    SDL_SetRenderDrawColor(_render, _color.r, _color.g, _color.b, _color.a);
    SDL_RenderFillRect(_render, &_rect);
}

void DrawGraph(SDL_Renderer* _render, const std::vector<int>& _sequences)
{
    int _offsetX = SORT_FRAME.x;
    int _offsetY = SORT_FRAME.y + SORT_FRAME.h;

    for(int _i = 0; _i < _sequences.size(); ++_i)
    {
        float _percent = (float)_sequences[_i] / 100.f;
        int _columnHeight = static_cast<int>(_percent * (float)SORT_FRAME.h);

        DrawDataGraph(_render,
                      _offsetX + _i * COLUMN_WIDTH,
                      _offsetY - _columnHeight,
                      _columnHeight,
                      WHITE);
    }
}

int main()
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Window* _window = SDL_CreateWindow
            (
                    "Sorting Visualizer",
                    SDL_WINDOWPOS_UNDEFINED,
                    SDL_WINDOWPOS_UNDEFINED,
                    SCREEN_WIDTH,
                    SCREEN_HEIGHT,
                    SDL_WINDOW_SHOWN
            );

    if(_window == nullptr)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Renderer* _render = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_Event _event;

    bool _isRunning = true;
    int _i = 1;
    int _j = 0;
    int _key = -1;
    bool _isSorting = false;

    std::vector<int> _sequences = GenerateShuffleNumbers(DATA_COUNT);

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

        Sort(_sequences, _i, _j, _key, _isSorting);

        SDL_SetRenderDrawColor(_render, 0, 0, 0, 255);
        SDL_RenderClear(_render);

        SDL_SetRenderDrawColor(_render, 255, 255, 255, 255);
        SDL_RenderDrawRect(_render, &SORT_FRAME);

        int _offsetX = SORT_FRAME.x;
        int _offsetY = SORT_FRAME.y + SORT_FRAME.h;

        for(int _k = 0; _k < _sequences.size(); ++_k)
        {
            float _percent = (float)_sequences[_k] / 100.f;
            int _columnHeight = static_cast<int>(_percent * (float)SORT_FRAME.h);

            Color _color = WHITE;

            if(_k == _j)
                _color = YELLOW;
            else if(_k == _i)
                _color = BLUE;

            DrawDataGraph(_render,
                          _offsetX + _k * COLUMN_WIDTH,
                          _offsetY - _columnHeight,
                          _columnHeight,
                          _color);
        }

        SDL_RenderPresent(_render);
        SDL_Delay(10);
    }

    SDL_DestroyRenderer(_render);
    SDL_DestroyWindow(_window);

    return 0;
}