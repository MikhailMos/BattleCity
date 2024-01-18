#include "Border.h"

#include "../../resources/ResourceManager.h"
#include "../../renderer/Sprite.h"

Border::Border(const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer)
	: IGameObject(IGameObject::EObjectType::Border, position, size, rotation, layer)
    , sprite_(ResourceManager::GetSprite("border"))
{
    colliders_.emplace_back(glm::vec2(0), size_);
}

void Border::Render() const
{
    sprite_->Render(position_, size_, rotation_, layer_);
}
