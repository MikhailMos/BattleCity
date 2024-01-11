#include "Water.h"

#include "../../resources/ResourceManager.h"
#include "../../renderer/Sprite.h"

Water::Water(const glm::vec2& position, const glm::vec2& size, const float rotation)
	: IGameObjcect(position, size, rotation)
    , sprite_(ResourceManager::GetSprite("water"))
    , spriteAnimator_(sprite_)
    , blockOffsets_{ glm::vec2(0, size_.y / 2.f),
                     glm::vec2(size_.x / 2.f, size_.y / 2.f),
                     glm::vec2(0, 0),
                     glm::vec2(size_.x / 2.f, 0) }

{
}

void Water::RenderBlock(const EBlockLocation eBlockLocation) const
{
    sprite_->Render(position_ + blockOffsets_[static_cast<size_t>(eBlockLocation)], size_ / 2.f, rotation_, spriteAnimator_.GetCurrentFrame());
}

void Water::Render() const
{
    RenderBlock(EBlockLocation::TopLeft);
    RenderBlock(EBlockLocation::TopRight);
    RenderBlock(EBlockLocation::BottomLeft);
    RenderBlock(EBlockLocation::BottomRight);
}

void Water::Update(const uint64_t delta)
{
    spriteAnimator_.Update(delta);
}
