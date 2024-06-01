#pragma once
#include <string>
#include "include/GameObject.h"

enum class ObjectType
{
    ENEMY,
    TOWER,
    ARROW
};

class Scene;
GameObject* Create(ObjectType _type, std::string _name, Scene* _scene);
