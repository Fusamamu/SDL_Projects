#pragma once
#include <SDL2/SDL.h>

#ifdef IMGUI_SRC_ID
#define GEN_ID ((IMGUI_SRC_ID) + (__LINE__))
#else
#define GEN_ID (__LINE__)
#endif

namespace MGUI
{
    struct UIState
    {
        int mouseX;
        int mouseY;
        int mouseDown;

        int hotItem;
        int activeItem;

        int keyboardItem;
        int keyEnterd;
        int keyModifier;
        int lastWidget;

    }_UIState {0, 0, 0, 0, 0, 0, 0, 0, 0};

    void UpdateState(SDL_Event& _event, UIState& _uiState)
    {
        switch(_event.type)
        {
            case SDL_MOUSEMOTION:
                _uiState.mouseX = _event.motion.x;
                _uiState.mouseY = _event.motion.y;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if(_event.button.button == 1)
                    _uiState.mouseDown = 1;
                break;
            case SDL_MOUSEBUTTONUP:
                if(_event.button.button == 1)
                    _uiState.mouseDown = 0;
                break;
        }
    }

    // Simple button IMGUI widget
    bool Button(SDL_Renderer* _render, int _id, int _x, int _y)
    {
        SDL_Rect _frame {_x, _y, 100, 30 };
        SDL_Point _mousePoint { _UIState.mouseX, _UIState.mouseY };

        if(SDL_PointInRect(&_mousePoint, &_frame))
        {
            _UIState.hotItem = _id;

            if (_UIState.activeItem == 0 && _UIState.mouseDown)
                _UIState.activeItem = _id;
        }

        SDL_SetRenderDrawColor(_render, 50, 50, 50, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(_render, &_frame);

        if(_UIState.hotItem == _id)
        {
            if(_UIState.activeItem == _id)
                SDL_SetRenderDrawColor(_render, 255, 255, 255, SDL_ALPHA_OPAQUE);
            else
                SDL_SetRenderDrawColor(_render, 130, 130, 130, SDL_ALPHA_OPAQUE);

            SDL_RenderFillRect(_render, &_frame);
        }
        else
        {
            SDL_SetRenderDrawColor(_render, 200, 130, 130, SDL_ALPHA_OPAQUE);
            SDL_RenderFillRect(_render, &_frame);
        }

        // If button is hot and active, but mouse button is not
        // down, the user must have clicked the button.
        if (_UIState.mouseDown  == 0   &&
            _UIState.hotItem    == _id &&
            _UIState.activeItem == _id)
            return true;

        // Otherwise, no click
        return false;
    }


    // Simple scroll bar IMGUI widget
    bool Slide(SDL_Renderer* _render, int _id, int _x, int _y, int _max, int &_value)
    {
        SDL_Rect _frame {_x, _y, 100, 30 };
        SDL_Point _mousePoint { _UIState.mouseX, _UIState.mouseY };

        // Calculate mouse cursor's relative y offset
        int _ypos = ((256 - 16) * _value) / _max;

        if(SDL_PointInRect(&_mousePoint, &_frame))
        {
            _UIState.hotItem = _id;
            if(_UIState.activeItem == 0 && _UIState.mouseDown)
                _UIState.activeItem = _id;
        }

        if(_UIState.activeItem == _id)
        {
            int _mousePosY = _UIState.mouseY;

            if(_mousePosY < 0)
                _mousePosY = 0;
            if(_mousePosY > 255)
                _mousePosY = 255;

            int _v = (_mousePosY * _max) / 255;
            if(_v != _value)
            {
                _value = _v;
                return true;
            }
        }

        // Check for hotness
//        if (regionhit(x+8, y+8, 16, 255))
//        {
//            uistate.hotitem = id;
//            if (uistate.activeitem == 0 && uistate.mousedown)
//                uistate.activeitem = id;
//        }
//
//        // Render the scrollbar
//        drawrect(x, y, 32, 256+16, 0x777777);
//
//        if (uistate.activeitem == id || uistate.hotitem == id)
//        {
//            drawrect(x+8, y+8 + ypos, 16, 16, 0xffffff);
//        }
//        else
//        {
//            drawrect(x+8, y+8 + ypos, 16, 16, 0xaaaaaa);
//        }





//        // Update widget value
//        if (uistate.activeitem == id)
//        {
//            int mousepos = uistate.mousey - (y + 8);
//            if (mousepos < 0) mousepos = 0;
//            if (mousepos > 255) mousepos = 255;
//            int v = (mousepos * max) / 255;
//            if (v != value)
//            {
//                value = v;
//                return true;
//            }
//        }

        return false;
    }

    void BeginFrame()
    {
        _UIState.hotItem = 0;
    }

    void EndFrame()
    {
        if(_UIState.mouseDown == 0)
        {
            _UIState.activeItem = 0;
        }
        else
        {
            if(_UIState.activeItem == 0)
                _UIState.activeItem = -1;
        }
    }
}