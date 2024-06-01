#include "GameplayScene.h"
#include "include/Application.h"
#include "include/GameObject.h"

GameplayScene::GameplayScene()
{
    Grid = new TileGrid();

    ObjectLayers =
            {
                    { 0, std::vector<GameObject*>() },
                    { 1, std::vector<GameObject*>() },
                    { 2, std::vector<GameObject*>() },
                    { 3, std::vector<GameObject*>() },
            };
}

GameplayScene::~GameplayScene()
{

}

void GameplayScene::Enter()
{
    const char* MAP_1 =
            "- - - - - - - - - - - - - - - - - - - - - - - - - -\n"
            "- - - - - - - - - - - - - - - - - - - - - - - - - -\n"
            "o o o o o o o o o o o o o o o - - - - - - - - - - -\n"
            "- - - - - - - - - - - - - - - - - - - - - - - - - -\n"
            "- - - - - - - - - - - - - - - - - - - - - - - - - -\n"
            "- - - o - - - - - - o o o o o o o o o o o o o o - -\n"
            "- - - o - - - - - - - - - - - - - - - - - - - - - -\n"
            "- - - o o o o o o o o o o o o o o o - - - - - - - -\n"
            "- - - o - - - - - - - - - - - - - - - - - - - - - -\n"
            "- - - - - - - - - - - - - - - - - - - - - - - - - -\n"
            "- - - - o o o o o o o o o o o o o o o - - - - - - -\n"
            "- - - - - - - - o - - - - - - - - - - - - - - - - -\n"
            "- - - - - - - - o - - - - - - - - - - - - - - - - -\n"
            "- - - - - - - - - - - - - - - - - - - - - - - - - -\n"
            "- - - o o o o o - - - - - - o - - - - - - - - - - -\n"
            "- - - - - - - - - - - - - - o - - - - - - - - - - -\n"
            "- - - - - - - - - o o o o o o o o o o o o o - - - -\n"
            "- - - - - - - - - - - - - - - - - - - - - - - - - -\n"
            "- - - - - - o - - - - - - - - - - - - - - - - - - -\n"
            "- - - - - - o - - - - - - - - - - - - - - - - - - -\n"
            "- - - - - - o - - - - - - - o o o o o o o - - - - -\n"
            "- - - - - - o - - - - - - - - - - - - - - - - - - -\n"
            "- - - - - - o - - - - - - - - - - - - - - - - - - -\n";

    Grid->LoadMap(MAP_1, this);

    Tile* _start = Grid->GetTile(0, 0);
    Tile* _end   = Grid->GetTile(15, 10);
    std::vector<Tile*> _path = TileGrid::GetAStarPath(*Grid, _start, _end);

    for(Tile* _tile : _path)
        _tile->Object->Renderer->Color = { 255, 0, 0, SDL_ALPHA_OPAQUE };

    auto* _enemy = Create(ObjectType::ENEMY, "Orc", this);
    _enemy->SetLayer(1);
    _enemy->SET_POSITION(Grid->GetTilePosition(0, 0));
    _enemy->GetComponent<FollowPathComponent>()->SetTargetPath(_path);

    auto* _tower = Create(ObjectType::TOWER, "Tower", this);
    _tower->SetLayer(1);
    _tower->SET_POSITION(Vector2(500, 500));
}

void GameplayScene::HandleEvents()
{

}

void GameplayScene::Update()
{
    for(auto& _gameObject : GameObjects)
        _gameObject.second->UpdateComponents();
}

void GameplayScene::Render()
{
    for(auto& _kvp : ObjectLayers)
        for(auto& _object : _kvp.second)
            _object->Renderer->Update();
}

void GameplayScene::Exit()
{

}