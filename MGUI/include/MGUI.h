#pragma once
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>

namespace MGUI
{
    SDL_Renderer* gRender;

    SDL_Color WHITE { 255, 255, 255, SDL_ALPHA_OPAQUE };
    SDL_Color GRAY  { 100, 100, 100, SDL_ALPHA_OPAQUE };
    SDL_Color BLACK {   0,   0,   0, SDL_ALPHA_OPAQUE };

    SDL_Color GUI_WINDOW_TITLE_BAR_DEFAULT_COLOR  {  61,  51,  43, SDL_ALPHA_OPAQUE };
    SDL_Color GUI_WINDOW_FRAME_OUTLINE_COLOR      {  61,  51,  43, SDL_ALPHA_OPAQUE };
    SDL_Color GUI_WINDOW_TITLE_BAR_HOT_COLOR      { 150, 150, 150, SDL_ALPHA_OPAQUE };
    SDL_Color GUI_WINDOW_TITLE_BAR_ACTIVE_COLOR   { 211, 211, 211, SDL_ALPHA_OPAQUE };
    SDL_Color GUI_WINDOW_TITLE_BAR_INACTIVE_COLOR { 125, 125, 125, SDL_ALPHA_OPAQUE };

    SDL_Color GUI_WINDOW_BODY_COLOR      {  30,  30,  30, SDL_ALPHA_OPAQUE };
    SDL_Color BUTTON_COLOR               {  97,  79, 64, SDL_ALPHA_OPAQUE };
    SDL_Color INPUT_FIELD_COLOR          {  88,  88,  88, SDL_ALPHA_OPAQUE };

    TTF_Font* gDefaultFont;

    int LineHeight = 22;

    struct UIState
    {
        int MouseX;
        int MouseY;

        SDL_Point MousePos;

        bool MouseDown;

        std::size_t HotItem;    //Mouse pos over ui
        std::size_t ActiveItem; //Mouse pos over ui and mouse pointer down
    }UI_State = { 0, 0, { 0, 0 }, false, 0, 0};

    void boxBlur(SDL_Texture* texture, SDL_Renderer* renderer, int blurRadius) {
        int width, height;
        SDL_QueryTexture(texture, NULL, NULL, &width, &height);

        // Create a new texture to store the blurred image
        SDL_Texture* blurredTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
        if (!blurredTexture) {
            std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
            return;
        }

        // Lock the original texture for reading
        void* pixels;
        int pitch;
        if (SDL_LockTexture(texture, NULL, &pixels, &pitch) != 0) {
            std::cerr << "Failed to lock texture: " << SDL_GetError() << std::endl;
            return;
        }

        // Create a vector to store the blurred pixel data
        std::vector<Uint32> blurredPixels(width * height);

        // Apply box blur
        Uint32* srcPixels = static_cast<Uint32*>(pixels);
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int r = 0, g = 0, b = 0, a = 0, count = 0;
                for (int ky = -blurRadius; ky <= blurRadius; ++ky) {
                    for (int kx = -blurRadius; kx <= blurRadius; ++kx) {
                        int nx = x + kx;
                        int ny = y + ky;
                        if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                            Uint32 pixel = srcPixels[ny * (pitch / 4) + nx];
                            r += (pixel & 0xFF000000) >> 24;
                            g += (pixel & 0x00FF0000) >> 16;
                            b += (pixel & 0x0000FF00) >> 8;
                            a += (pixel & 0x000000FF);
                            ++count;
                        }
                    }
                }
                r /= count;
                g /= count;
                b /= count;
                a /= count;
                blurredPixels[y * width + x] = (r << 24) | (g << 16) | (b << 8) | a;
            }
        }

        SDL_UnlockTexture(texture);

        // Copy the blurred pixel data to the new texture
        SDL_UpdateTexture(blurredTexture, NULL, blurredPixels.data(), width * sizeof(Uint32));

        // Render the blurred texture
        SDL_SetRenderTarget(renderer, blurredTexture);
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, blurredTexture, NULL, NULL);
        SDL_SetRenderTarget(renderer, NULL);

        // Clean up
        SDL_DestroyTexture(blurredTexture);
    }

    void DrawRect(int _xPos, int _yPos, int _w, int _h, SDL_Color _col)
    {
        SDL_SetRenderDrawColor(gRender, _col.r, _col.g, _col.b, SDL_ALPHA_OPAQUE);
        SDL_Rect _rect {_xPos, _yPos, _w, _h };
        SDL_RenderFillRect(gRender, &_rect);
    }

    enum class TextAlignment
    {
        LEFT,
        CENTER,
        RIGHT
    };

    std::size_t GenerateID(const std::string& _label, std::size_t _parent_id = 0)
    {
        std::hash<std::string> _hasher;
        return _hasher(std::to_string(_parent_id) + _label);
    }

    //Not efficient
    void DrawText(int _xPos, int _yPos, int _w, int _h, const std::string& _text, SDL_Color _col = WHITE, const TextAlignment& _alignment = TextAlignment::LEFT)
    {
        SDL_Surface* _textSurface = TTF_RenderText_Blended(gDefaultFont, _text.c_str(), _col);

        if (_textSurface == nullptr)
        {
            std::cout << "Unable to render text surface! TTF_Error: " << TTF_GetError() << std::endl;
            return;
        }

        SDL_Texture* _textTexture = SDL_CreateTextureFromSurface(gRender, _textSurface);
        if (_textTexture == nullptr)
        {
            std::cout << "Unable to create texture from rendered text! SDL_Error: " << SDL_GetError() << std::endl;
            return;
        }

        int _targetPosX = _xPos;
        int _targetPosY = _yPos;
        int _width  = _textSurface->w;
        int _height = _textSurface->h;

        switch(_alignment)
        {
            case TextAlignment::LEFT:
                _targetPosX = _xPos;
                break;
            case TextAlignment::RIGHT:
                _targetPosX = _xPos;
                break;
            case TextAlignment::CENTER:
                _targetPosX = _xPos + _w / 2 - _width / 2;
                break;
        }

        SDL_Rect _dest { _targetPosX, _targetPosY, _width, _height };
        SDL_RenderCopy(gRender, _textTexture, nullptr, &_dest);

        //Debug
//        SDL_SetRenderDrawColor(gRender, 255, 0, 0, SDL_ALPHA_OPAQUE);
//        SDL_RenderDrawRect(gRender, &_dest);

        SDL_DestroyTexture(_textTexture);
        SDL_FreeSurface(_textSurface);
    }




    bool RegionHit(int _xPos, int _yPos, int _w, int _h)
    {
        return UI_State.MouseX > _xPos      &&
               UI_State.MouseX < _xPos + _w &&
               UI_State.MouseY > _yPos      &&
               UI_State.MouseY < _yPos + _h;
    }


    void BeginFrame()
    {
        UI_State.HotItem = 0;
    }

    void EndFrame()
    {
        if(!UI_State.MouseDown)
        {
            UI_State.ActiveItem = 0;
        }
        else
        {
            if(UI_State.ActiveItem == 0)
                UI_State.ActiveItem = -1;
        }
    }



    void BeginHorizontalLayout()
    {

    }

    void EndHorizontalLayout()
    {

    }


    struct DockPanel;
    DockPanel* gDockPanel;






    //Can be base of some kind
    //This can be docked
    struct GUIWindow
    {
        std::size_t ID;
        std::string Title;

        int PosX;
        int PosY;

        int BodyWidth;
        int BodyHeight;
        int TitleBarHeight;
        int Padding;

        //Common properties//
        bool Draggable;
        bool IsDocked;
        bool IsResizing;

        SDL_Point PosHandle;
        SDL_Point DragOffset;

        SDL_Rect ResizeHandle;

        //Should consider use some structure and differentiate between SDL_Window and GUI_Window
        static std::unordered_map<std::string, GUIWindow*> Windows;

        GUIWindow(const std::string& _title):
                Title(_title),
                PosX(100),
                PosY(100),
                BodyWidth(320),
                BodyHeight(500),
                TitleBarHeight(27),
                Padding(6),
                Draggable(false),
                IsDocked(false),
                IsResizing(false)
        {
            PosHandle = { PosX, PosY + TitleBarHeight };
        }

        ~GUIWindow()
        {
            Windows.clear();
        }

        void ResetPosHandle()
        {
            PosHandle = { PosX, PosY + TitleBarHeight };
        }

        static GUIWindow* GetWindow(const std::string& _title)
        {
            if(Windows.find(_title) == Windows.end())
            {
                auto _newWindow = new GUIWindow(_title);
                _newWindow->ID = GenerateID(_title);
                Windows[_title] = _newWindow;
                return _newWindow;
            }

            return Windows[_title];
        }


        void Update()
        {
            if(RegionHit(PosX, PosY, BodyWidth, TitleBarHeight))
            {
                UI_State.HotItem = ID;
                if(UI_State.MouseDown)
                {
                    if(UI_State.ActiveItem == 0)
                        UI_State.ActiveItem = ID;
                }
            }

            if(UI_State.MouseDown)
            {
                if(UI_State.HotItem    == ID &&
                   UI_State.ActiveItem == ID)
                {
                    if(!Draggable)
                    {
                        Draggable = true;
                        DragOffset.x = UI_State.MouseX - PosX;
                        DragOffset.y = UI_State.MouseY - PosY;
                    }
                }
            }
            else
            {
                Draggable = false;
            }

            if(Draggable)
            {
                PosX = UI_State.MouseX - DragOffset.x;
                PosY = UI_State.MouseY - DragOffset.y;
                PosHandle = { PosX, PosY + TitleBarHeight };
            }

//            if(SDL_PointInRect(&UI_State.MousePos, &ResizeHandle))
//            {
//                UI_State.HotItem = ID;
//                if(UI_State.MouseDown)
//                {
//                    if(UI_State.ActiveItem == 0)
//                        UI_State.ActiveItem = ID;
//                }
//            }
//
//            if(UI_State.MouseDown)
//            {
//                if(UI_State.HotItem    == ID &&
//                   UI_State.ActiveItem == ID)
//                {
//                    if(!IsResizing)
//                    {
//                        IsResizing = true;
//                    }
//                }
//            }
//            else
//            {
//                Draggable = false;
//            }
        }

        //Push to queue to render
        void Render()
        {
            DrawRect(PosX - 2, PosY - 2, BodyWidth + 4, BodyHeight + 4, GUI_WINDOW_FRAME_OUTLINE_COLOR );
            DrawRect(PosX, PosY, BodyWidth, BodyHeight    , GUI_WINDOW_BODY_COLOR     );
            DrawRect(PosX, PosY, BodyWidth, TitleBarHeight, GUI_WINDOW_TITLE_BAR_DEFAULT_COLOR);

            if(UI_State.HotItem == ID)
            {
                if(UI_State.ActiveItem == ID)
                    DrawRect(PosX, PosY, BodyWidth, TitleBarHeight, GUI_WINDOW_TITLE_BAR_ACTIVE_COLOR);
                else
                    DrawRect(PosX, PosY, BodyWidth, TitleBarHeight, GUI_WINDOW_TITLE_BAR_HOT_COLOR);
            }

            DrawText(PosX + Padding, PosY + Padding - 2, BodyWidth, TitleBarHeight - Padding * 2, Title, { 97, 79, 64, SDL_ALPHA_OPAQUE });
        }

        int GetContainerWidthWithPadding()
        {
            return BodyWidth - 2 * Padding;
        }

        SDL_Point PosHandleWithPadding()
        {
            return { PosHandle.x + Padding, PosHandle.y + Padding };
        }
    };

    //Can combine with GUIWindow
    struct DockPanel
    {
        enum class DockArea
        {
            LEFT, RIGHT, TOP, BOTTOM
        };

        std::unordered_map<DockArea, SDL_Rect> DockAreas;

        SDL_Window* Window;

        DockPanel(SDL_Window* _window): Window(_window)
        {
            int _screenWidth, _screenHeight;
            SDL_GetWindowSize(_window, &_screenWidth, &_screenHeight);

            DockAreas[DockArea::LEFT]   = { 0, 0, 250, _screenHeight };
            DockAreas[DockArea::RIGHT]  = { _screenWidth - 250, 0, 250, _screenHeight };
            DockAreas[DockArea::TOP]    = { 0, 0, _screenWidth, 250 };
            DockAreas[DockArea::BOTTOM] = { 0, _screenHeight - 250, _screenWidth, 250 };
        }

        void TryDock(GUIWindow* _guiWindow)
        {
            for(const auto& _area : DockAreas)
            {
                const SDL_Rect& _panelArea = _area.second;

                switch(_area.first)
                {
                    case DockArea::LEFT:

                        int _threshold = _panelArea.x + _panelArea.w;

                        if(_guiWindow->PosX < _threshold)
                        {
                            SDL_RenderDrawRect(gRender, &_panelArea);
                        }

                        break;
//                    case DockArea::RIGHT:
//                        break;
//                    case DockArea::TOP:
//                        break;
//                    case DockArea::BOTTOM:
//                        break;
                }
            }
        }
    };

    std::unordered_map<std::string, GUIWindow*> GUIWindow::Windows;
    GUIWindow* gCurrentWindow;


    //Util function
    SDL_Texture* LoadTexture(const std::string& _filePath)
    {
        SDL_Surface* _loadedImage = IMG_Load(_filePath.c_str());

        if (!_loadedImage) {
            std::cerr << "IMG_Load Error: " << IMG_GetError() << std::endl;
            return nullptr;
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(gRender, _loadedImage);

        if (!texture)
            std::cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;

        SDL_FreeSurface(_loadedImage);
        return texture;
    }

    SDL_Texture* gObjectIconTexture;




    //------App initialization---------//
    void Init(SDL_Window* _window, SDL_Renderer* _render)
    {
        //Init font
        //Init Texture


        gRender = _render;
        gDefaultFont = TTF_OpenFont("../Res/Fonts/open-sans/OpenSans-Bold.ttf", 13);
        if (gDefaultFont == nullptr)
        {
            std::cout << "Failed to load _font! TTF_Error: " << TTF_GetError() << std::endl;
            return;
        }

        gDockPanel = new DockPanel(_window);

        gObjectIconTexture = LoadTexture("../Res/Images/ObjectIcon.png");
    }

    void Quit()
    {
        TTF_CloseFont(gDefaultFont);
    }
    //------------------------------//





    void BeginWindow(const std::string& _title = "Debug")
    {
        gCurrentWindow = GUIWindow::GetWindow(_title);
        gCurrentWindow->Update();
        gCurrentWindow->Render();//Update at the end of the frame??

        gDockPanel->TryDock(gCurrentWindow);
    }

    void EndWindow()
    {
        gCurrentWindow->ResetPosHandle();
    }




    SDL_Point gMenuBarPosHandle;

    void BeginMenuBar(SDL_Window* _window)
    {
        int _screenWidth, _screenHeight;
        SDL_GetWindowSize(_window, &_screenWidth, &_screenHeight);
        DrawRect(0, 0, _screenWidth, LineHeight, BLACK);
    }

    void EndMenuBar()
    {
        gMenuBarPosHandle.x = 50;
        gMenuBarPosHandle.y = 0;
    }

    void MenuBarItem(const std::string& _text)
    {
        std::size_t _id = GenerateID(_text);

        int _xPos  = gMenuBarPosHandle.x;
        int _yPos  = 0;
        int _width = 50;

        if(RegionHit(gMenuBarPosHandle.x, _yPos, _width, LineHeight))
        {
            UI_State.HotItem = _id;

            if(UI_State.MouseDown)
            {
                if(UI_State.ActiveItem == 0)
                    UI_State.ActiveItem = _id;
            }
        }

        if(UI_State.HotItem == _id)
        {
            if(UI_State.ActiveItem == _id)
                DrawText(gMenuBarPosHandle.x, 0, 50, LineHeight, _text, WHITE);
            else
                DrawText(gMenuBarPosHandle.x, 0, 50, LineHeight, _text, WHITE);
        }
        else
        {
            DrawText(gMenuBarPosHandle.x, 0, 50, LineHeight, _text, GRAY);
        }

        gMenuBarPosHandle.x += 50;
    }

    void MenuBar(SDL_Window* _window)
    {
        int _screenWidth, _screenHeight;
        SDL_GetWindowSize(_window, &_screenWidth, &_screenHeight);

        DrawRect(0, 0, _screenWidth, LineHeight, GUI_WINDOW_TITLE_BAR_DEFAULT_COLOR);
    }





    void Label(const std::string& _text, bool _withBackground = false)
    {
        int _xPos  = gCurrentWindow->PosHandleWithPadding().x;
        int _yPos  = gCurrentWindow->PosHandleWithPadding().y;
        int _width = gCurrentWindow->GetContainerWidthWithPadding();

        if(_withBackground)
        {
            DrawRect(gCurrentWindow->PosHandle.x, _yPos, gCurrentWindow->BodyWidth, LineHeight, { 47, 48 , 43, SDL_ALPHA_OPAQUE });
        }

        DrawText(_xPos, _yPos, _width, LineHeight, _text, { 100, 100, 100, SDL_ALPHA_OPAQUE }, TextAlignment::LEFT);

        gCurrentWindow->PosHandle.y += LineHeight + gCurrentWindow->Padding;
    }

    void InputFloatField(const std::string& _header, float _value)
    {
        int _xPos  = gCurrentWindow->PosHandleWithPadding().x;
        int _yPos  = gCurrentWindow->PosHandleWithPadding().y;
        int _width = gCurrentWindow->GetContainerWidthWithPadding();

        float rounded = std::round(_value * 100.0) / 100.0;
        std::string myString = std::to_string(rounded);

        // Remove trailing zeros and decimal point if not needed
        myString.erase(myString.find_last_not_of('0') + 1, std::string::npos);
        if (myString.back() == '.') {
            myString.pop_back();
        }

        DrawText(_xPos         , _yPos, 15, LineHeight, _header, WHITE, TextAlignment::LEFT);
        DrawRect(_xPos + 15    , _yPos, 60, LineHeight, BLACK);
        DrawText(_xPos + 15 + 3, _yPos, 60, LineHeight + 5, myString, WHITE, TextAlignment::LEFT);

        gCurrentWindow->PosHandle.y += LineHeight + gCurrentWindow->Padding;
    }

    void InputFieldVec3(const std::string& _header, float _value)
    {
        int _xPos  = gCurrentWindow->PosHandleWithPadding().x;
        int _yPos  = gCurrentWindow->PosHandleWithPadding().y;
        int _width = gCurrentWindow->GetContainerWidthWithPadding();

        float rounded = std::round(_value * 100.0) / 100.0;
        std::string myString = std::to_string(rounded);

        // Remove trailing zeros and decimal point if not needed
        myString.erase(myString.find_last_not_of('0') + 1, std::string::npos);
        if (myString.back() == '.') {
            myString.pop_back();
        }

        DrawText(_xPos                  , _yPos, 100, LineHeight, _header, WHITE, TextAlignment::LEFT);

        _xPos += 60;

        int _xPosHandle = 15;

        DrawText(_xPos                  , _yPos, _xPosHandle, LineHeight, "X", GRAY, TextAlignment::LEFT);
        DrawRect(_xPos + _xPosHandle    , _yPos, 60, LineHeight, BLACK);
        DrawText(_xPos + _xPosHandle + 3, _yPos, 60, LineHeight + 5, myString, WHITE, TextAlignment::LEFT);

        _xPos += _xPosHandle + 60 + 5;
        DrawText(_xPos                  , _yPos, _xPosHandle, LineHeight, "Y", GRAY, TextAlignment::LEFT);
        DrawRect(_xPos + _xPosHandle    , _yPos, 60, LineHeight, BLACK);
        DrawText(_xPos + _xPosHandle + 3, _yPos, 60, LineHeight + 5, myString, WHITE, TextAlignment::LEFT);

        _xPos += _xPosHandle + 60 + 5;
        DrawText(_xPos                  , _yPos, _xPosHandle, LineHeight, "Z", GRAY, TextAlignment::LEFT);
        DrawRect(_xPos + _xPosHandle    , _yPos, 60, LineHeight, BLACK);
        DrawText(_xPos + _xPosHandle + 3, _yPos, 60, LineHeight + 5, myString, WHITE, TextAlignment::LEFT);



        gCurrentWindow->PosHandle.y += LineHeight + gCurrentWindow->Padding;
    }

    bool Button(const std::string& _text)
    {
        std::size_t _id = GenerateID(_text, gCurrentWindow->ID);

        int _xPos  = gCurrentWindow->PosHandleWithPadding().x;
        int _yPos  = gCurrentWindow->PosHandleWithPadding().y;
        int _width = gCurrentWindow->GetContainerWidthWithPadding();

        gCurrentWindow->PosHandle.y += 50 + gCurrentWindow->Padding;

        //Check state
        if(RegionHit(_xPos, _yPos, _width, 50))
        {
            UI_State.HotItem = _id;

            if(UI_State.MouseDown)
            {
                if(UI_State.ActiveItem == 0)
                    UI_State.ActiveItem = _id;
            }
        }

        //Render
        if(UI_State.HotItem == _id)
        {
            if(UI_State.ActiveItem == _id)
                DrawRect(_xPos, _yPos, _width, 50, WHITE);
            else
                DrawRect(_xPos, _yPos, _width, 50, GRAY);
        }
        else
        {
            DrawRect(_xPos, _yPos, _width, 50, BUTTON_COLOR);
        }

        DrawText(_xPos, _yPos, _width, 50, _text, WHITE, TextAlignment::CENTER);

        //Check action
        if(UI_State.MouseDown)
        {
            if(UI_State.HotItem    == _id &&
               UI_State.ActiveItem == _id)
            {
                return true;
            }
        }
        return false;
    }

    int gItemLevel = 0;

    void SelectableItem(const std::string& _text, const int& _level = 0)
    {
        std::size_t _id = GenerateID(_text, gCurrentWindow->ID);

        int _xPos  = gCurrentWindow->PosHandle.x;
        int _yPos  = gCurrentWindow->PosHandle.y;
        int _width = gCurrentWindow->BodyWidth;

        if(RegionHit(_xPos, _yPos, _width, LineHeight))
        {
            UI_State.HotItem = _id;
            if(UI_State.MouseDown)
            {
                if(UI_State.ActiveItem == 0)
                    UI_State.ActiveItem = _id;
            }
        }

        int _targetLevel = _level * 20;

        if(UI_State.HotItem == _id)
        {
            if(UI_State.ActiveItem == _id)
                DrawRect(_xPos, _yPos, _width, LineHeight, WHITE);
            else
                DrawRect(_xPos, _yPos, _width, LineHeight, GRAY);

            DrawText(_xPos + 30 + _targetLevel, _yPos, _width, LineHeight, _text);
        }
        else
        {
            DrawText(_xPos + 30 + _targetLevel, _yPos, _width, LineHeight, _text);
        }

        SDL_Rect _targetDest {_xPos + 5 + _targetLevel, _yPos, LineHeight - 3, LineHeight - 3};
        SDL_RenderCopy(gRender, gObjectIconTexture, nullptr, &_targetDest );
        //DrawRect(_xPos + 5 + _targetLevel, _yPos + 4, 10, 10, WHITE);


        gCurrentWindow->PosHandle.y += LineHeight;
    }
}

