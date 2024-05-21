#include "GameplayScene.h"
#include "Application.h"

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

    auto* _enemy = CREATE_OBJECT(Orc);
    _enemy->Renderer->Texture = Application::GetInstance().GetTexture("OrcWalk");
    _enemy->Renderer->Src  = {0, 0, 48, 48 };
    _enemy->Renderer->Dest = {0, 0, 48, 48 };
    _enemy->Renderer->FlipVertical = true;

    _enemy->SetLayer(1);
    _enemy->Transform->Position.x = Grid->GetTile(0, 0)->Object->Transform->Position.x;
    _enemy->Transform->Position.y = Grid->GetTile(0, 0)->Object->Transform->Position.y;

    auto* _followComp = _enemy->AddComponent<FollowPathComponent>();
    _followComp->SetTargetPath(_path);

    auto* _spriteAnimation = _enemy->AddComponent<SpriteAnimationComponent>();
    _spriteAnimation->SetSheetDimension(1, 6);
    _spriteAnimation->AnimationSpeed = 25;

    auto* _tower = CREATE_OBJECT(Tower);
    _tower->Renderer->Texture = Application::GetInstance().GetTexture("Tower");
    _tower->Renderer->Src = { 0, 0, 70, 130 };
    _tower->Renderer->Dest= { 0, 0, 70, 130 };

    _tower->SetLayer(1);
    _tower->Transform->Position.x = 500;
    _tower->Transform->Position.y = 500;

    auto* _towerSpriteAnimation = _tower->AddComponent<SpriteAnimationComponent>();
    _towerSpriteAnimation->SetSheetDimension(1, 6);
    _towerSpriteAnimation->AnimationSpeed = 10;
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