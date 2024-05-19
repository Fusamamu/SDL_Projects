#include "Application.h"

Application::Application():
        m_IsRunning(true),
        Window(nullptr),
        Renderer(nullptr),
        Grid(nullptr)
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return;
    }

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

    LoadTexture("TileSet", "../Resource/Textures/TileSet.png");

    ObjectLayers =
            {
                { 0, std::vector<GameObject*>() },
                { 1, std::vector<GameObject*>() },
                { 2, std::vector<GameObject*>() },
                { 3, std::vector<GameObject*>() },
            };
}

Application::~Application()
{
    SDL_DestroyRenderer(Renderer);
    SDL_DestroyWindow(Window);
}

void Application::Start()
{
    Grid = new TileGrid(10, 10);
}

void Application::Update()
{
    Tile* _start = Grid->GetTile(0, 0);
    Tile* _end   = Grid->GetTile(3, 3);
    std::vector<Tile*> _path = TileGrid::GetAStarPath(*Grid, _start, _end);

    for(Tile* _tile : _path)
        _tile->Object->Renderer->Color = { 255, 0, 0, SDL_ALPHA_OPAQUE };

    auto* _object = new GameObject();

    _object->SetLayer(1);
    _object->Transform->Position.x = Grid->GetTile(0, 0)->Object->Transform->Position.x;
    _object->Transform->Position.y = Grid->GetTile(0, 0)->Object->Transform->Position.y;
    _object->Renderer->UsePrimitive = true;
    _object->Renderer->Dest  = { 0, 0, 64, 64 };
    _object->Renderer->Color = { 0, 255, 0, SDL_ALPHA_OPAQUE };

    auto* _followComp = _object->AddComponent<FollowPathComponent>();
    _followComp->SetTargetPath(_path);

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

        _object->GetComponent<FollowPathComponent>()->Update();

        SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 0);
        SDL_RenderClear(Renderer);
        for(auto& _kvp : ObjectLayers)
            for(auto& _object : _kvp.second)
                _object->Renderer->Update();
        SDL_RenderPresent(Renderer);
    }
}

void Application::Quit()
{
    delete Grid;
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

void Application::AddToLayer(GameObject* _gameObject)
{
    RemoveFromLayer(_gameObject);
    int _layer = _gameObject->Layer;
    if(ObjectLayers.find(_layer) != ObjectLayers.end())
        ObjectLayers[_layer].push_back(_gameObject);
}

void Application::RemoveFromLayer(GameObject* _gameObject)
{
    int _layer = _gameObject->Layer;

    if(ObjectLayers.find(_layer) != ObjectLayers.end())
    {
        std::vector<GameObject*>& _vec = ObjectLayers[_layer];
        auto _iter = std::find(_vec.begin(), _vec.end(), _gameObject);
        if(_iter != _vec.end())
            _vec.erase(_iter);
    }
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