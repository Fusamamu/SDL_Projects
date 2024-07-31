#include "Application.h"
#include "SceneManager.h"
#include "ObjectFactory.h"

GameObject* Create(ObjectType _type, std::string _name)
{
    GameObject* _result = new GameObject(_name);
    switch(_type)
    {
        case ObjectType::ENEMY:
            {
                _result->Renderer->Texture = Application::GetInstance().GetTexture("OrcWalk");
                _result->Renderer->Src  = {0, 0, 48, 48 };
                _result->Renderer->Dest = {0, 0, 48, 48 };
                _result->Renderer->FlipVertical = true;

                _result->AddComponent<CircleCollider2D>();
                _result->AddComponent<FollowPathComponent>();

                auto* _spriteAnimation = _result->AddComponent<SpriteAnimationComponent>();
                _spriteAnimation->SetSheetDimension(1, 6);
                _spriteAnimation->AnimationSpeed = 25;
            }
            break;
        case ObjectType::TOWER:
            {
                _result->Renderer->Texture = Application::GetInstance().GetTexture("Tower");
                _result->Renderer->Src = { 0, 0, 70, 130 };
                _result->Renderer->Dest= { 0, 0, 70, 130 };

                _result->AddComponent<CircleCollider2D>();

                auto* _spriteAnimation = _result->AddComponent<SpriteAnimationComponent>();
                _spriteAnimation->SetSheetDimension(1, 6);
                _spriteAnimation->AnimationSpeed = 10;
            }
            break;
    }
    return _result;
}