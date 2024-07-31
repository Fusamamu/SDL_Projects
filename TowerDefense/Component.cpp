#include "Timer.h"
#include "Components/Component.h"
#include "GameObject.h"
#include "Application.h"

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

SpriteAnimationComponent::SpriteAnimationComponent():
AnimationSpeed(1),
Elapsed(0)
{
}

CircleCollider2D::CircleCollider2D()
{

}

void CircleCollider2D::Init()
{

}

void CircleCollider2D::Update()
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
