#include "Trees.h"

#include "../../resources/ResourceManager.h"
#include "../../renderer/Sprite.h"

Trees::Trees(const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer)
	: IGameObjcect(position, size, rotation, layer)
    , sprite_(ResourceManager::GetSprite("trees"))
    , blockOffsets_{ glm::vec2(0, size_.y / 2.f),
                     glm::vec2(size_.x / 2.f, size_.y / 2.f),
                     glm::vec2(0, 0),
                     glm::vec2(size_.x / 2.f, 0) }

{
}

void Trees::RenderBlock(const EBlockLocation eBlockLocation) const
{
    sprite_->Render(position_ + blockOffsets_[static_cast<size_t>(eBlockLocation)], size_ / 2.f, rotation_, layer_);
}

void Trees::Render() const
{
    RenderBlock(EBlockLocation::TopLeft);
    RenderBlock(EBlockLocation::TopRight);
    RenderBlock(EBlockLocation::BottomLeft);
    RenderBlock(EBlockLocation::BottomRight);
}
