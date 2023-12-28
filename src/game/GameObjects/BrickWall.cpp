#include "BrickWall.h"

#include "../../renderer/Sprite.h"

BrickWall::BrickWall(const std::shared_ptr<RenderEngine::Sprite> pSprite, const glm::vec2& position, const glm::vec2& size, const float rotation)
	: IGameObjcect(position, size, rotation)
	, pCurrentSprite_(pSprite)
{
}

void BrickWall::Render() const
{
	pCurrentSprite_->Render(position_, size_, rotation_);
}

void BrickWall::Update(const uint64_t delta)
{
}
