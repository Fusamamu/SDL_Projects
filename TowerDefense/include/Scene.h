#pragma once
#include <map>
#include <unordered_map>
#include "GameObject.h"

#define CREATE_OBJECT(x) new GameObject(#x, this)

class Scene
{
public:
    std::unordered_map<std::string, GameObject*> GameObjects;
    std::map<int, std::vector<GameObject*>> ObjectLayers;
    std::map<int, std::vector<IRenderable*>> RenderAbleLayers;
public:
    virtual ~Scene() = default;

    virtual void Enter() = 0;
    virtual void HandleEvents() = 0;
    virtual void Update() = 0;
    virtual void Render() = 0;
    virtual void Exit() = 0;

    void AddGameObject   (GameObject* _object);
    void RemoveGameObject(GameObject* _object);
};