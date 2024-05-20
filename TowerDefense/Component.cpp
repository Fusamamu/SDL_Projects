#include "Timer.h"
#include "Component.h"
#include "GameObject.h"

TransformComponent::TransformComponent():
ScaleX(1),
ScaleY(1)
{

}

void TransformComponent::Init()
{

}

void TransformComponent::Update()
{

}

RendererComponent::RendererComponent():
        FlipVertical(false),
        Render(nullptr),
        UsePrimitive(false),
        RenderBound(false)
{

}

void RendererComponent::Init()
{
}

void RendererComponent::Update()
{
    auto* Transform = Owner->Transform;

    Dest.x = static_cast<int>(Transform->Position.x);
    Dest.y = static_cast<int>(Transform->Position.y);
    Dest.w *= static_cast<int>(Transform->ScaleX);
    Dest.h *= static_cast<int>(Transform->ScaleY);

    if(UsePrimitive)
    {
        SDL_SetRenderDrawColor(Render, Color.r, Color.g, Color.b, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(Render, &Dest);
    }
    else
    {
        SDL_Point center { 0, 0 };
        if(!FlipVertical)
            SDL_RenderCopy(Render, Texture, &Src, &Dest);
        else
            SDL_RenderCopyEx(Render, Texture, &Src, &Dest, 0, &center, SDL_FLIP_HORIZONTAL);
    }

    if(RenderBound)
    {
        SDL_SetRenderDrawColor(Render, BoundColor.r, BoundColor.g, BoundColor.b, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawRect(Render, &Dest);
    }
}

SpriteAnimationComponent::SpriteAnimationComponent():
AnimationSpeed(1),
Elapsed(0)
{
}

void SpriteAnimationComponent::Init()
{
    SDL_Texture* _texture = Owner->Renderer->Texture;
    if(_texture)
    {
        int textureWidth  = 0;
        int textureHeight = 0;
        SDL_QueryTexture(_texture, nullptr, nullptr, &textureWidth, &textureHeight);
    }
}

void SpriteAnimationComponent::SetSheetDimension(int _row, int _column)
{
    SheetRowCount    = _row;
    SheetColumnCount = _column;

    SDL_Texture* _texture = Owner->Renderer->Texture;
    if(_texture)
    {
        int _textureWidth  = 0;
        int _textureHeight = 0;
        SDL_QueryTexture(_texture, nullptr, nullptr, &_textureWidth, &_textureHeight);

        SrcWidth  = _textureWidth / _column;
        SrcHeight = _textureHeight/ _row;
    }
}

void SpriteAnimationComponent::Update()
{
    Elapsed += Timer::GetInstance().DeltaTime * AnimationSpeed;
    if(Elapsed >= 1.0f)
    {
        ClipIndex++;
        if(ClipIndex >= SheetColumnCount)
            ClipIndex = 0;

        Owner->Renderer->Src.x = ClipIndex * SrcWidth;
        Elapsed = 0.0f;
    }
}

FollowPathComponent::FollowPathComponent():
Speed(50),
IsMoving(false),
Index(0)
{
}

void FollowPathComponent::Init()
{

}

void FollowPathComponent::SetTargetPath(std::vector<Tile*> _path)
{
    CurrentPath = _path;
}

void FollowPathComponent::Update()
{
    if(Index > CurrentPath.size() - 1)
    {
        Index = CurrentPath.size() - 1;
        IsMoving = false;
        return;
    }

    Tile* _currentTile = CurrentPath[Index];

    Vector2 _startPos  = Owner->Transform->Position;
    Vector2 _targetPos = _currentTile->GetPosition();

    Vector2 _dir = (_targetPos - _startPos).GetNorm();

    Owner->Renderer->FlipVertical = _dir.x > 0;

    if(_startPos.GetDistance(_targetPos) > 0.5f)
    {
        Owner->Transform->Position += _dir * Speed * Timer::GetInstance().DeltaTime;
    }
    else
    {
        Owner->Transform->Position = _targetPos;
        Index++;
    }
}
