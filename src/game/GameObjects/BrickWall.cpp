#include "BrickWall.h"

#include "../../resources/ResourceManager.h"
#include "../../renderer/Sprite.h"

BrickWall::BrickWall(const EBrickWallType eBrickWallType, const glm::vec2& position, const glm::vec2& size, const float rotation)
	: IGameObjcect(position, size, rotation)
	, eCurrentBrickState_{ EBrickState::Destroyed, EBrickState::Destroyed, EBrickState::Destroyed, EBrickState::Destroyed }
    , blockOffsets_{ glm::vec2(0, size_.y / 2.f),
                     glm::vec2(size_.x / 2.f, size_.y / 2.f),
                     glm::vec2(0, 0),
                     glm::vec2(size_.x / 2.f, 0) }

{
	sprites_[static_cast<size_t>(EBrickState::All)]                 = ResourceManager::GetSprite("brickWall_All");
    sprites_[static_cast<size_t>(EBrickState::TopLeft)]             = ResourceManager::GetSprite("brickWall_TopLeft");
    sprites_[static_cast<size_t>(EBrickState::TopRight)]            = ResourceManager::GetSprite("brickWall_TopRight");
    sprites_[static_cast<size_t>(EBrickState::Top)]                 = ResourceManager::GetSprite("brickWall_Top");
    sprites_[static_cast<size_t>(EBrickState::BottomLeft)]          = ResourceManager::GetSprite("brickWall_BottomLeft");
    sprites_[static_cast<size_t>(EBrickState::Left)]                = ResourceManager::GetSprite("brickWall_Left");
    sprites_[static_cast<size_t>(EBrickState::TopRight_BottomLeft)] = ResourceManager::GetSprite("brickWall_TopRight_BottomLeft");
    sprites_[static_cast<size_t>(EBrickState::Top_BottomLeft)]      = ResourceManager::GetSprite("brickWall_Top_BottomLeft");
    sprites_[static_cast<size_t>(EBrickState::BottomRight)]         = ResourceManager::GetSprite("brickWall_BottomRight");
    sprites_[static_cast<size_t>(EBrickState::TopLeft_BottomRight)] = ResourceManager::GetSprite("brickWall_TopLeft_BottomRight");
    sprites_[static_cast<size_t>(EBrickState::Right)]               = ResourceManager::GetSprite("brickWall_Right");
    sprites_[static_cast<size_t>(EBrickState::Top_BottomRight)]     = ResourceManager::GetSprite("brickWall_Top_BottomRight");
    sprites_[static_cast<size_t>(EBrickState::Bottom)]              = ResourceManager::GetSprite("brickWall_Bottom");
    sprites_[static_cast<size_t>(EBrickState::TopLeft_Bottom)]      = ResourceManager::GetSprite("brickWall_TopLeft_Bottom");
    sprites_[static_cast<size_t>(EBrickState::TopRight_Bottom)]     = ResourceManager::GetSprite("brickWall_TopRight_Bottom");

    switch (eBrickWallType)
    {
    case EBrickWallType::All:
        eCurrentBrickState_.fill(EBrickState::All);
        break;
    case EBrickWallType::Top:
        eCurrentBrickState_[static_cast<size_t>(EBrickLocation::TopLeft)]  = EBrickState::All;
        eCurrentBrickState_[static_cast<size_t>(EBrickLocation::TopRight)] = EBrickState::All;
        break;
    case EBrickWallType::Bottom:
        eCurrentBrickState_[static_cast<size_t>(EBrickLocation::BottomLeft)]  = EBrickState::All;
        eCurrentBrickState_[static_cast<size_t>(EBrickLocation::BottomRight)] = EBrickState::All;
        break;
    case EBrickWallType::Left:
        eCurrentBrickState_[static_cast<size_t>(EBrickLocation::TopLeft)]    = EBrickState::All;
        eCurrentBrickState_[static_cast<size_t>(EBrickLocation::BottomLeft)] = EBrickState::All;
        break;
    case EBrickWallType::Right:
        eCurrentBrickState_[static_cast<size_t>(EBrickLocation::TopRight)]    = EBrickState::All;
        eCurrentBrickState_[static_cast<size_t>(EBrickLocation::BottomRight)] = EBrickState::All;
        break;
    case EBrickWallType::TopLeft:
        eCurrentBrickState_[static_cast<size_t>(EBrickLocation::TopLeft)] = EBrickState::All;
        break;
    case EBrickWallType::TopRight:
        eCurrentBrickState_[static_cast<size_t>(EBrickLocation::TopRight)] = EBrickState::All;
        break;
    case EBrickWallType::BottomLeft:
        eCurrentBrickState_[static_cast<size_t>(EBrickLocation::BottomLeft)] = EBrickState::All;
        break;
    case EBrickWallType::BottomRight:
        eCurrentBrickState_[static_cast<size_t>(EBrickLocation::BottomRight)] = EBrickState::All;
        break;
    
    }
}

void BrickWall::RenderBrick(const EBrickLocation eBrickLocation) const
{
    const EBrickState state = eCurrentBrickState_[static_cast<size_t>(eBrickLocation)];
    if (state != EBrickState::Destroyed)
    {
        sprites_[static_cast<size_t>(state)]->Render(position_ + blockOffsets_[static_cast<size_t>(eBrickLocation)], size_ / 2.f, rotation_);
    }
}

void BrickWall::Render() const
{
    RenderBrick(EBrickLocation::TopLeft);
    RenderBrick(EBrickLocation::TopRight);
    RenderBrick(EBrickLocation::BottomLeft);
    RenderBrick(EBrickLocation::BottomRight);
}

void BrickWall::Update(const uint64_t delta)
{
}
