#include "Application.h"
#include "Components/RendererComponent.h"
#include "Components/TextComponent.h"

TextComponent::TextComponent():
TextColor({255, 255, 255, SDL_ALPHA_OPAQUE}),
HorizontalAlignment(TextHorizontalAlignment::CENTER),
VerticalAlignment  (TextVerticalAlignment::CENTER)
{

}

TextComponent::~TextComponent()
{
    SDL_DestroyTexture(TextTexture);
    TextTexture = nullptr;
}

void TextComponent::Init()
{
    TTF_Font* _font = TTF_OpenFont("../Resource/Fonts/open-sans/OpenSans-Bold.ttf", 28);
    if (_font == nullptr)
    {
        std::cout << "Failed to load _font! TTF_Error: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Surface* _textSurface = TTF_RenderText_Solid(_font, "Hello, SDL_ttf!", TextColor);
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

    Owner->Renderer->UseSrc  = false;
    Owner->Renderer->Texture = TextTexture;
    Owner->Renderer->Dest.w  = _textSurface->w;
    Owner->Renderer->Dest.h  = _textSurface->h;

    SDL_FreeSurface(_textSurface);
}

void TextComponent::Update()
{
}

int TextComponent::Width() const
{
    ///return Dest.w * Owner->Transform->ScaleX;
    return -1;
}

int TextComponent::Height() const
{
    return -1;
    //return Dest.h * Owner->Transform->ScaleY;
}