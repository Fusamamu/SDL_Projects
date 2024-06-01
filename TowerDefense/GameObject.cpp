#include "include/GameObject.h"

#include <utility>
#include "include/Application.h"
#include "Scene.h"

GameObject::GameObject(const std::string& _name, Scene* _scene):
Layer(0),
IsActive(true),
Transform(nullptr),
Renderer(nullptr),
OwnScene(_scene)
{
    Name = _name;

    Transform = AddComponent<TransformComponent>();
    Renderer  = AddComponent<RendererComponent>();

    Transform->Position.x = 0;
    Transform->Position.y = 0;

    Renderer->Render = Application::GetInstance().Renderer;

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