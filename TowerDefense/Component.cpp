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
        Render(nullptr),
        UsePrimitive(false),
        RenderBound(false)
{

}

void RendererComponent::Init()
{
     Transform = Owner->GetComponent<TransformComponent>();
}

void RendererComponent::Update()
{
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
        SDL_RenderCopy(Render, Texture, &Src, &Dest);
    }

    if(RenderBound)
    {
        SDL_SetRenderDrawColor(Render, BoundColor.r, BoundColor.g, BoundColor.b, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawRect(Render, &Dest);
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
