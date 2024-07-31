#include "Components/RendererComponent.h"
#include "Application.h"

RendererComponent::RendererComponent():
        FlipVertical(false),
        Renderer(Application::GetInstance().Renderer),
        UsePrimitive(false),
        RenderBound(false),
        UseSrc(true)
{

}

void RendererComponent::Init()
{
}

void RendererComponent::Update()
{
    if(!Owner->IsActive)
        return;

    auto* Transform = Owner->Transform;

    Dest.x = static_cast<int>(Transform->Position.x - Dest.w / 2);
    Dest.y = static_cast<int>(Transform->Position.y - Dest.h / 2);
    Dest.w *= static_cast<int>(Transform->ScaleX);
    Dest.h *= static_cast<int>(Transform->ScaleY);

    if(UsePrimitive)
    {
        SDL_SetRenderDrawColor(Renderer, Color.r, Color.g, Color.b, SDL_ALPHA_OPAQUE);
        SDL_RenderFillRect(Renderer, &Dest);
    }
    else
    {
        SDL_Point center { 0, 0 };
        if(!FlipVertical)
        {
            if(UseSrc)
                SDL_RenderCopy(Renderer, Texture, &Src, &Dest);
            else
                SDL_RenderCopy(Renderer, Texture, nullptr, &Dest);
        }
        else
            SDL_RenderCopyEx(Renderer, Texture, &Src, &Dest, 0, &center, SDL_FLIP_HORIZONTAL);
    }

    if(RenderBound)
    {
        SDL_SetRenderDrawColor(Renderer, BoundColor.r, BoundColor.g, BoundColor.b, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawRect(Renderer, &Dest);
    }
}

void RendererComponent::Render()
{
//    if(!Owner->IsActive)
//        return;
//
//    auto* Transform = Owner->Transform;
//
//    Dest.x = static_cast<int>(Transform->Position.x - Dest.w / 2);
//    Dest.y = static_cast<int>(Transform->Position.y - Dest.h / 2);
//    Dest.w *= static_cast<int>(Transform->ScaleX);
//    Dest.h *= static_cast<int>(Transform->ScaleY);
//
//    if(UsePrimitive)
//    {
//        SDL_SetRenderDrawColor(Renderer, Color.r, Color.g, Color.b, SDL_ALPHA_OPAQUE);
//        SDL_RenderFillRect(Renderer, &Dest);
//    }
//    else
//    {
//        SDL_Point center { 0, 0 };
//        if(!FlipVertical)
//            SDL_RenderCopy(Renderer, Texture, &Src, &Dest);
//        else
//            SDL_RenderCopyEx(Renderer, Texture, &Src, &Dest, 0, &center, SDL_FLIP_HORIZONTAL);
//    }
//
//    if(RenderBound)
//    {
//        SDL_SetRenderDrawColor(Renderer, BoundColor.r, BoundColor.g, BoundColor.b, SDL_ALPHA_OPAQUE);
//        SDL_RenderDrawRect(Renderer, &Dest);
//    }
}