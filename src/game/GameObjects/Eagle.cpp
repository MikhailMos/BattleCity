#include "Eagle.h"

#include "../../resources/ResourceManager.h"
#include "../../renderer/Sprite.h"

Eagle::Eagle(const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer)
	: IGameObject(IGameObject::EObjectType::Eagle, position, size, rotation, layer)
    , sprites_ { ResourceManager::GetSprite("eagle"), 
                 ResourceManager::GetSprite("eagle_dead") }
    , eCurrentState_(EEagleState::Alive)
{
    colliders_.emplace_back(glm::vec2(0), size_);
}

void Eagle::Render() const
{
    sprites_[static_cast<size_t>(eCurrentState_)]->Render(position_, size_, rotation_, layer_);
}

void Eagle::Update(const double delta)
{
}
