#include "Application.h"

Application::Application():
        m_IsRunning(true),
        Window(nullptr),
        Renderer(nullptr)
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

    Window = SDL_CreateWindow
            (
                    "Tower Defense",
                    SDL_WINDOWPOS_UNDEFINED,
                    SDL_WINDOWPOS_UNDEFINED,
                    (int)SCREEN_WIDTH,
                    (int)SCREEN_HEIGHT,
                    SDL_WINDOW_SHOWN
            );


    Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    LoadTexture("TileSet", "../Resource/Textures/FieldsTileset.png");
    LoadTexture("OrcWalk", "../Resource/Textures/free-field-enemies-pixel-art-for-tower-defense/2/S_Walk.png");
    LoadTexture("Tower"  , "../Resource/Textures/free-archer-towers-pixel-art-for-tower-defense/2 Idle/7.png");
}

Application::~Application()
{
    SDL_DestroyRenderer(Renderer);
    SDL_DestroyWindow(Window);
}

void Application::Start()
{
    SceneManager::GetInstance().LoadScene<GameplayScene>();
}

void Application::Update()
{
    while(m_IsRunning)
    {
        Timer::GetInstance().Tick();

        while(SDL_PollEvent(&Event))
        {
            switch(Event.type)
            {
                case SDL_QUIT:
                    m_IsRunning = false;
                    break;
                case SDL_KEYDOWN:
                    if(Event.key.keysym.sym == SDLK_ESCAPE)
                        m_IsRunning = false;
                    break;
            }
        }

        SceneManager::CurrentScene().HandleEvents();
        SceneManager::CurrentScene().Update();
        SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 0);
        SDL_RenderClear(Renderer);
        SceneManager::CurrentScene().Render();
        SDL_RenderPresent(Renderer);
    }
}

void Application::Quit()
{
}

SDL_Texture* Application::LoadTexture(const std::string& _textureName, const std::string& _path)
{
    SDL_Texture* _texture = IMG_LoadTexture(Renderer, _path.c_str());
    Textures[_textureName] = _texture;
    return _texture;
}

SDL_Texture* Application::GetTexture(const std::string& _textureName)
{
    auto _iter = Textures.find(_textureName);
    if(_iter != Textures.end())
        return Textures[_textureName];

    return nullptr;
}

std::string Application::GenerateUUID()
{
    std::random_device _rd;
    std::mt19937 gen(_rd());
    std::uniform_int_distribution<uint32_t> _dis(0, 0xFFFFFFFF);

    std::stringstream _ss;

    // Generate 4 random 32-bit integers and format them as hexadecimal
    for (int i = 0; i < 4; ++i)
        _ss << std::hex << std::setw(8) << std::setfill('0') << _dis(gen);

    std::string _uuid = _ss.str();

    // Insert dashes to format it as UUID
    _uuid.insert(8, "-");
    _uuid.insert(13, "-");
    _uuid.insert(18, "-");
    _uuid.insert(23, "-");

    return _uuid;
}