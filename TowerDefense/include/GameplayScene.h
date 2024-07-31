#pragma once
#include "Scene.h"
#include "TileGrid.h"
#include "ObjectFactory.h"

class GameplayScene : public Scene
{
public:
    TileGrid* Grid;
public:
    GameplayScene();
    ~GameplayScene();

    void Enter() override;
    void HandleEvents() override;
    void Update() override;
    void Render() override;
    void Exit() override;
};