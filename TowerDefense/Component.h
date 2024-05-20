#pragma once
#include <vector>
#include "SDL2/SDL.h"
#include "Tile.h"
#include "Vector2.h"

class GameObject;

class Component
{
public:
    GameObject* Owner;
public:
    virtual ~Component() = default;
    virtual void Init()   = 0;
    virtual void Update() = 0;
};

class TransformComponent : public Component
{
public:
    Vector2 Position;
    float ScaleX, ScaleY;
public:
    explicit TransformComponent();
    void Init() override;
    void Update() override;
};

class RendererComponent : public Component
{
public:
    bool FlipVertical;
    bool UsePrimitive;
    bool RenderBound;

    SDL_Renderer* Render;
    SDL_Color Color;
    SDL_Color BoundColor;
    SDL_Rect Src;
    SDL_Rect Dest;
    SDL_Texture* Texture;

public:
    explicit RendererComponent();
    void Init() override;
    void Update() override;
};

class SpriteAnimationComponent: public Component
{
public:
    int ClipIndex;
    int SheetRowCount;
    int SheetColumnCount;
    int SrcWidth;
    int SrcHeight;

    float AnimationSpeed;
    float Elapsed;
public:
    explicit SpriteAnimationComponent();
    void Init() override;
    void Update() override;

    void SetSheetDimension(int _row, int _column);
    void StartAnimation();
    void StopAnimation();
};

class FollowPathComponent: public Component
{
public:
    float Speed;
    bool IsMoving;
    int Index;
    std::vector<Tile*> CurrentPath;
public:
    explicit FollowPathComponent();
    void Init() override;
    void Update() override;
    void SetTargetPath(std::vector<Tile*> _path);
};