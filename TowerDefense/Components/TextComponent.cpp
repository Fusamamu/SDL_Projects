#include "Components/TextComponent.h"
#include "Application.h"

TextComponent::TextComponent()
{

}

void TextComponent::Init()
{
    TTF_Font* _font = TTF_OpenFont("path/to/your/_font.ttf", 28);

    if (_font == nullptr)
    {
        std::cout << "Failed to load _font! TTF_Error: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Color _textColor = {255, 255, 255 };

    SDL_Surface* _textSurface = TTF_RenderText_Solid(_font, "Hello, SDL_ttf!", _textColor);
    if (_textSurface == nullptr)
    {
        std::cout << "Unable to render text surface! TTF_Error: " << TTF_GetError() << std::endl;
        return;
    }

    TextTexture = SDL_CreateTextureFromSurface(Application::GetInstance().Renderer, _textSurface);
    if (TextTexture == nullptr)
    {
        std::cout << "Unable to create texture from rendered text! SDL_Error: " << SDL_GetError() << std::endl;
        return;
    }

    TextWidth  = _textSurface->w;
    TextHeight = _textSurface->h;

    Dest.w = TextWidth;
    Dest.h = TextHeight;

    SDL_FreeSurface(_textSurface);
}

void TextComponent::Update()
{
    auto* Transform = Owner->Transform;

    Dest.x = static_cast<int>(Transform->Position.x - Dest.w / 2);
    Dest.y = static_cast<int>(Transform->Position.y - Dest.h / 2);
    Dest.w *= static_cast<int>(Transform->ScaleX);
    Dest.h *= static_cast<int>(Transform->ScaleY);

    SDL_RenderCopy(Application::GetInstance().Renderer, TextTexture, nullptr, &Dest);
}