#include "GameObject.h"
#include "Application.h"

GameObject::GameObject():
Layer(0),
Transform(nullptr),
Renderer(nullptr)
{
    auto& _app = Application::GetInstance();

    Transform = AddComponent<TransformComponent>();
    Renderer  = AddComponent<RendererComponent>();

    Transform->Position.x = 0;
    Transform->Position.y = 0;

    Renderer->Render = _app.Renderer;

    ID = _app.GenerateUUID();

    auto _iter = _app.GameObjects.find(ID);
    if(_iter == _app.GameObjects.end())
        _app.GameObjects[ID] = this;

    _app.AddToLayer(this);
}

GameObject::~GameObject()
{
    auto& _app = Application::GetInstance();
    auto _iter = _app.GameObjects.find(ID);
    if(_iter != _app.GameObjects.end())
        _app.GameObjects.erase(_iter);

    _app.RemoveFromLayer(this);
}

void GameObject::SetLayer(int _layer)
{
    Layer = _layer;
    Application::GetInstance().AddToLayer(this);
}

void GameObject::UpdateComponents()
{
    for(auto& _pair : m_Components)
        _pair.second->Update();
}