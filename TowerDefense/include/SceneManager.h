#pragma once
#include <memory>
#include <typeindex>
#include <unordered_map>
#include "Scene.h"
#include "GameplayScene.h"
#include <iostream>

class SceneManager
{
public:
    static Scene& CurrentScene()
    {
        return *GetInstance().m_CurrentScene;
    }

    static SceneManager& GetInstance()
    {
        static SceneManager _instance;
        return _instance;
    }

    template<typename T>
    void CreateScene()
    {
        if(m_SceneMap.find(typeid(T)) != m_SceneMap.end())
            return;
        m_SceneMap[typeid(T)] = std::make_unique<T>();
    }

    template<typename T>
    void LoadScene()
    {
        if(m_SceneMap.find(typeid(T)) == m_SceneMap.end())
            return;

        if(m_CurrentScene)
            m_CurrentScene->Exit();

        m_CurrentScene = m_SceneMap[typeid(T)].get();
        m_CurrentScene->Enter();
    }

private:
    SceneManager();
    ~SceneManager();

    Scene* m_CurrentScene;
    std::unordered_map<std::type_index, std::unique_ptr<Scene>> m_SceneMap;
};