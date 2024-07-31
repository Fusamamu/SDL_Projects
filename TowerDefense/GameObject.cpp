#include "SceneManager.h"
#include "GameObject.h"
#include "Scene.h"

GameObject::GameObject(const std::string& _name):
Layer(0),
IsActive(true),
Transform(nullptr),
Renderer(nullptr),
OwnScene(&SceneManager::CurrentScene())
{
    Name = _name;

    Transform = AddComponent<TransformComponent>();
    Renderer  = AddComponent<RendererComponent>();

    //Need to set default in Transform instead
    Transform->Position.x = 0;
    Transform->Position.y = 0;

    OwnScene->AddGameObject(this);
}

GameObject::GameObject()
{

}

GameObject::~GameObject()
{
    OwnScene->RemoveGameObject(this);
}

void GameObject::SetLayer(int _layer)
{
    Layer = _layer;
}

void GameObject::UpdateComponents()
{
    for(auto& _pair : m_Components)
        _pair.second->Update();
}