#pragma once
#include <memory>
#include <string>
#include <typeindex>
#include <algorithm>
#include <unordered_map>
#include "Components/Component.h"
#include "Components/RendererComponent.h"

#define SET_POSITION(x) Transform->Position = x

class Application;
class Scene;

class GameObject
{
public:
    std::string ID;
    std::string Name;

    int Layer;
    bool IsActive;

    Scene* OwnScene;

    TransformComponent* Transform;
    RendererComponent*  Renderer;
public:
    GameObject(const std::string& _name);
    GameObject();
    ~GameObject();

    void SetLayer(int _layer);

    template<typename T>
    T* AddComponent()
    {
        m_Components[typeid(T)] = std::make_unique<T>();
        m_Components[typeid(T)]->Owner = this;
        m_Components[typeid(T)]->Init();
        return static_cast<T*>(m_Components[typeid(T)].get());
    }

    template<typename T>
    T* GetComponent()
    {
        auto _iter = m_Components.find(typeid(T));
        if(_iter != m_Components.end())
            return dynamic_cast<T*>(_iter->second.get());

        return nullptr;
    }

    void UpdateComponents();

private:
    std::unordered_map<std::type_index, std::unique_ptr<Component>> m_Components;
};