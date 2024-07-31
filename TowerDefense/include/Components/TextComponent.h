#pragma once
#include "Component.h"

enum class TextHorizontalAlignment
{
    LEFT, CENTER, RIGHT
};

enum class TextVerticalAlignment
{
    TOP, CENTER, BOTTOM
};

class TextComponent: public Component
{
public:
    int FontSize;
    SDL_Texture* TextTexture;
    SDL_Color TextColor;
    TextHorizontalAlignment HorizontalAlignment;
    TextVerticalAlignment   VerticalAlignment;
public:
    explicit TextComponent();
    ~TextComponent();
    void Init() override;
    void Update() override;

    int Width() const;
    int Height() const;
};