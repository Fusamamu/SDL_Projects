#include <algorithm>
#include "Application.h"
#include "Scene.h"

void Scene::AddGameObject(GameObject *_object)
{
    std::string _id = Application::GetInstance().GenerateUUID();
    if(GameObjects.find(_id) == GameObjects.end())
        GameObjects[_id] = _object;

    int _layer = _object->Layer;
    if(ObjectLayers.find(_layer) != ObjectLayers.end())
        ObjectLayers[_layer].push_back(_object);
}

void Scene::RemoveGameObject(GameObject *_object)
{
    std::string _id = _object->ID;
    auto _iter = GameObjects.find(_id);
    if(_iter != GameObjects.end())
        GameObjects.erase(_iter);

    int _layer = _object->Layer;

    if(ObjectLayers.find(_layer) != ObjectLayers.end())
    {
        std::vector<GameObject*>& _vec = ObjectLayers[_layer];
        auto _iter = std::find(_vec.begin(), _vec.end(), _object);
        if(_iter != _vec.end())
            _vec.erase(_iter);
    }
}