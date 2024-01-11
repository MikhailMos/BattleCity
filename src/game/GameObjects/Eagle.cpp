#include "Eagle.h"

#include "../../resources/ResourceManager.h"
#include "../../renderer/Sprite.h"

Eagle::Eagle(const glm::vec2& position, const glm::vec2& size, const float rotation)
	: IGameObjcect(position, size, rotation)
    , sprites_ { ResourceManager::GetSprite("eagle"), 
                 ResourceManager::GetSprite("eagle_dead") }
    , eCurrentState_(EEagleState::Alive)
{
}

void Eagle::Render() const
{
    sprites_[static_cast<size_t>(eCurrentState_)]->Render(position_, size_, rotation_);
}

void Eagle::Update(const uint64_t delta)
{
}
