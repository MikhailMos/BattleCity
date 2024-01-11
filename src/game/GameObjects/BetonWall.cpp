#include "BetonWall.h"

#include "../../resources/ResourceManager.h"
#include "../../renderer/Sprite.h"

BetonWall::BetonWall(const EBetonWallType eBetonWallType, const glm::vec2& position, const glm::vec2& size, const float rotation)
	: IGameObjcect(position, size, rotation)
	, eCurrentBlockState_{ EBlockState::Destroyed, EBlockState::Destroyed, EBlockState::Destroyed, EBlockState::Destroyed }
    , sprite_(ResourceManager::GetSprite("betonWall"))
    , blockOffsets_{ glm::vec2(0, size_.y / 2.f),
                     glm::vec2(size_.x / 2.f, size_.y / 2.f),
                     glm::vec2(0, 0),
                     glm::vec2(size_.x / 2.f, 0) }

{
   
    switch (eBetonWallType)
    {
    case EBetonWallType::All:
        eCurrentBlockState_.fill(EBlockState::Enabled);
        break;
    case EBetonWallType::Top:
        eCurrentBlockState_[static_cast<size_t>(EBlockLocation::TopLeft)]  = EBlockState::Enabled;
        eCurrentBlockState_[static_cast<size_t>(EBlockLocation::TopRight)] = EBlockState::Enabled;
        break;
    case EBetonWallType::Bottom:
        eCurrentBlockState_[static_cast<size_t>(EBlockLocation::BottomLeft)]  = EBlockState::Enabled;
        eCurrentBlockState_[static_cast<size_t>(EBlockLocation::BottomRight)] = EBlockState::Enabled;
        break;
    case EBetonWallType::Left:
        eCurrentBlockState_[static_cast<size_t>(EBlockLocation::TopLeft)]    = EBlockState::Enabled;
        eCurrentBlockState_[static_cast<size_t>(EBlockLocation::BottomLeft)] = EBlockState::Enabled;
        break;
    case EBetonWallType::Right:
        eCurrentBlockState_[static_cast<size_t>(EBlockLocation::TopRight)]    = EBlockState::Enabled;
        eCurrentBlockState_[static_cast<size_t>(EBlockLocation::BottomRight)] = EBlockState::Enabled;
        break;
    case EBetonWallType::TopLeft:
        eCurrentBlockState_[static_cast<size_t>(EBlockLocation::TopLeft)] = EBlockState::Enabled;
        break;
    case EBetonWallType::TopRight:
        eCurrentBlockState_[static_cast<size_t>(EBlockLocation::TopRight)] = EBlockState::Enabled;
        break;
    case EBetonWallType::BottomLeft:
        eCurrentBlockState_[static_cast<size_t>(EBlockLocation::BottomLeft)] = EBlockState::Enabled;
        break;
    case EBetonWallType::BottomRight:
        eCurrentBlockState_[static_cast<size_t>(EBlockLocation::BottomRight)] = EBlockState::Enabled;
        break;
    
    }
}

void BetonWall::RenderBlock(const EBlockLocation eBlockLocation) const
{
    const EBlockState state = eCurrentBlockState_[static_cast<size_t>(eBlockLocation)];
    if (state != EBlockState::Destroyed)
    {
        sprite_->Render(position_ + blockOffsets_[static_cast<size_t>(eBlockLocation)], size_ / 2.f, rotation_);
    }
}

void BetonWall::Render() const
{
    RenderBlock(EBlockLocation::TopLeft);
    RenderBlock(EBlockLocation::TopRight);
    RenderBlock(EBlockLocation::BottomLeft);
    RenderBlock(EBlockLocation::BottomRight);
}

void BetonWall::Update(const uint64_t delta)
{
}
