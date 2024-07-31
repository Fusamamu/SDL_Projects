#pragma once
#include "Components/Component.h"

class RendererComponent : public Component, public IRenderable
{
public:
    bool FlipVertical;
    bool UsePrimitive;
    bool RenderBound;
    bool UseSrc;

    SDL_Renderer* Renderer;
    SDL_Color Color;
    SDL_Color BoundColor;

    SDL_Rect Src;
    SDL_Rect Dest;
    SDL_Texture* Texture;

public:
    explicit RendererComponent();
    void Init() override;
    void Update() override;
    void Render() override;
};