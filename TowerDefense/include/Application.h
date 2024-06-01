#pragma once
#include <map>
#include <unordered_map>
#include <random>
#include <sstream>
#include <iomanip>
#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"
#include "SDL2_ttf/SDL_ttf.h"
#include "GameObject.h"
#include "../Tile.h"
#include "../TileGrid.h"
#include "../Timer.h"
#include "../SceneManager.h"

class Application
{
public:
    SDL_Event Event;
    SDL_Window* Window;
    SDL_Renderer* Renderer;

    std::unordered_map<std::string, SDL_Texture*> Textures;
    const unsigned int SCREEN_WIDTH  = 1600;
    const unsigned int SCREEN_HEIGHT = 900;
public:
    Application();
    ~Application();

    static Application& GetInstance()
    {
        static Application _instance;
        return _instance;
    }

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    void Start();
    void Update();
    void Quit();

    std::string GenerateUUID();

    SDL_Texture* LoadTexture(const std::string& _textureName, const std::string& _path);
    SDL_Texture* GetTexture(const std::string& _textureName);
private:
    bool m_IsRunning;
};
