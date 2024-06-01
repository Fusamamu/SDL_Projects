#pragma once
#include "Component.h"

class TextComponent: public Component
{
public:
    int FontSize;
    int TextWidth;
    int TextHeight;
    SDL_Texture* TextTexture;
    SDL_Color TextColor;

    SDL_Rect Dest;
public:
    explicit TextComponent();
    void Init() override;
    void Update() override;
};