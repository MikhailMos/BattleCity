#include "BrickWall.h"

#include "../../resources/ResourceManager.h"
#include "../../renderer/Sprite.h"
#include <iostream>

BrickWall::BrickWall(const EBrickWallType eBrickWallType, const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer)
	: IGameObject(IGameObject::EObjectType::BrickWall, position, size, rotation, layer)
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

    auto onCollisionCallbackTopLeft = [&](const IGameObject& object, const Physics::ECollisionDirection direction)
        {
            onCollisionCallback(EBrickLocation::TopLeft, object, direction);
        };
    auto onCollisionCallbackTopRight = [&](const IGameObject& object, const Physics::ECollisionDirection direction)
        {
            onCollisionCallback(EBrickLocation::TopRight, object, direction);
        };
    auto onCollisionCallbackBottomLeft = [&](const IGameObject& object, const Physics::ECollisionDirection direction)
        {
            onCollisionCallback(EBrickLocation::BottomLeft, object, direction);
        };
    auto onCollisionCallbackBottomRight = [&](const IGameObject& object, const Physics::ECollisionDirection direction)
        {
            onCollisionCallback(EBrickLocation::BottomRight, object, direction);
        };

    brickLocationToColliderMap_.fill(nullptr);
    colliders_.reserve(4);

    switch (eBrickWallType)
    {
    case EBrickWallType::All:
        eCurrentBrickState_.fill(EBrickState::All);
        brickLocationToColliderMap_[static_cast<size_t>(EBrickLocation::TopLeft)] = &(colliders_.emplace_back(GetAABBForBrickState(EBrickLocation::TopLeft, EBrickState::All, size_), onCollisionCallbackTopLeft));
        brickLocationToColliderMap_[static_cast<size_t>(EBrickLocation::TopRight)] = &(colliders_.emplace_back(GetAABBForBrickState(EBrickLocation::TopRight, EBrickState::All, size_), onCollisionCallbackTopRight));
        brickLocationToColliderMap_[static_cast<size_t>(EBrickLocation::BottomLeft)] = &(colliders_.emplace_back(GetAABBForBrickState(EBrickLocation::BottomLeft, EBrickState::All, size_), onCollisionCallbackBottomLeft));
        brickLocationToColliderMap_[static_cast<size_t>(EBrickLocation::BottomRight)] = &(colliders_.emplace_back(GetAABBForBrickState(EBrickLocation::BottomRight, EBrickState::All, size_), onCollisionCallbackBottomRight));
        break;
    case EBrickWallType::Top:
        eCurrentBrickState_[static_cast<size_t>(EBrickLocation::TopLeft)]  = EBrickState::All;
        eCurrentBrickState_[static_cast<size_t>(EBrickLocation::TopRight)] = EBrickState::All;
        brickLocationToColliderMap_[static_cast<size_t>(EBrickLocation::TopLeft)] = &(colliders_.emplace_back(GetAABBForBrickState(EBrickLocation::TopLeft, EBrickState::All, size_), onCollisionCallbackTopLeft));
        brickLocationToColliderMap_[static_cast<size_t>(EBrickLocation::TopRight)] = &(colliders_.emplace_back(GetAABBForBrickState(EBrickLocation::TopRight, EBrickState::All, size_), onCollisionCallbackTopRight));
        break;
    case EBrickWallType::Bottom:
        eCurrentBrickState_[static_cast<size_t>(EBrickLocation::BottomLeft)]  = EBrickState::All;
        eCurrentBrickState_[static_cast<size_t>(EBrickLocation::BottomRight)] = EBrickState::All;
        brickLocationToColliderMap_[static_cast<size_t>(EBrickLocation::BottomLeft)] = &(colliders_.emplace_back(GetAABBForBrickState(EBrickLocation::BottomLeft, EBrickState::All, size_), onCollisionCallbackBottomLeft));
        brickLocationToColliderMap_[static_cast<size_t>(EBrickLocation::BottomRight)] = &(colliders_.emplace_back(GetAABBForBrickState(EBrickLocation::BottomRight, EBrickState::All, size_), onCollisionCallbackBottomRight));
        break;
    case EBrickWallType::Left:
        eCurrentBrickState_[static_cast<size_t>(EBrickLocation::TopLeft)]    = EBrickState::All;
        eCurrentBrickState_[static_cast<size_t>(EBrickLocation::BottomLeft)] = EBrickState::All;
        brickLocationToColliderMap_[static_cast<size_t>(EBrickLocation::TopLeft)] = &(colliders_.emplace_back(GetAABBForBrickState(EBrickLocation::TopLeft, EBrickState::All, size_), onCollisionCallbackTopLeft));
        brickLocationToColliderMap_[static_cast<size_t>(EBrickLocation::BottomLeft)] = &(colliders_.emplace_back(GetAABBForBrickState(EBrickLocation::BottomLeft, EBrickState::All, size_), onCollisionCallbackBottomLeft));
        break;
    case EBrickWallType::Right:
        eCurrentBrickState_[static_cast<size_t>(EBrickLocation::TopRight)]    = EBrickState::All;
        eCurrentBrickState_[static_cast<size_t>(EBrickLocation::BottomRight)] = EBrickState::All;
        brickLocationToColliderMap_[static_cast<size_t>(EBrickLocation::TopRight)] = &(colliders_.emplace_back(GetAABBForBrickState(EBrickLocation::TopRight, EBrickState::All, size_), onCollisionCallbackTopRight));
        brickLocationToColliderMap_[static_cast<size_t>(EBrickLocation::BottomRight)] = &(colliders_.emplace_back(GetAABBForBrickState(EBrickLocation::BottomRight, EBrickState::All, size_), onCollisionCallbackBottomRight));
        break;
    case EBrickWallType::TopLeft:
        eCurrentBrickState_[static_cast<size_t>(EBrickLocation::TopLeft)] = EBrickState::All;
        brickLocationToColliderMap_[static_cast<size_t>(EBrickLocation::TopLeft)] = &(colliders_.emplace_back(GetAABBForBrickState(EBrickLocation::TopLeft, EBrickState::All, size_), onCollisionCallbackTopLeft));
        break;
    case EBrickWallType::TopRight:
        eCurrentBrickState_[static_cast<size_t>(EBrickLocation::TopRight)] = EBrickState::All;
        brickLocationToColliderMap_[static_cast<size_t>(EBrickLocation::TopRight)] = &(colliders_.emplace_back(GetAABBForBrickState(EBrickLocation::TopRight, EBrickState::All, size_), onCollisionCallbackTopRight));
        break;
    case EBrickWallType::BottomLeft:
        eCurrentBrickState_[static_cast<size_t>(EBrickLocation::BottomLeft)] = EBrickState::All;
        brickLocationToColliderMap_[static_cast<size_t>(EBrickLocation::BottomLeft)] = &(colliders_.emplace_back(GetAABBForBrickState(EBrickLocation::BottomLeft, EBrickState::All, size_), onCollisionCallbackBottomLeft));
        break;
    case EBrickWallType::BottomRight:
        eCurrentBrickState_[static_cast<size_t>(EBrickLocation::BottomRight)] = EBrickState::All;
        brickLocationToColliderMap_[static_cast<size_t>(EBrickLocation::BottomRight)] = &(colliders_.emplace_back(GetAABBForBrickState(EBrickLocation::BottomRight, EBrickState::All, size_), onCollisionCallbackBottomRight));
        break;
    }
}

void BrickWall::RenderBrick(const EBrickLocation eBrickLocation) const
{
    const EBrickState state = eCurrentBrickState_[static_cast<size_t>(eBrickLocation)];
    if (state != EBrickState::Destroyed)
    {
        sprites_[static_cast<size_t>(state)]->Render(position_ + blockOffsets_[static_cast<size_t>(eBrickLocation)], size_ / 2.f, rotation_, layer_);
    }
}

BrickWall::EBrickState BrickWall::GetBrickStateAfterCollision(const EBrickState currentState, const Physics::ECollisionDirection direction)
{
    switch (currentState)
    {
    case EBrickState::All:
        switch (direction)
        {
        case Physics::ECollisionDirection::Left:
            return EBrickState::Right;
        case Physics::ECollisionDirection::Right:
            return EBrickState::Left;
        case Physics::ECollisionDirection::Top:
            return EBrickState::Bottom;
        case Physics::ECollisionDirection::Bottom:
            return EBrickState::Top;
        }
        break;

    case EBrickState::Top:
        switch (direction)
        {
        case Physics::ECollisionDirection::Left:
            return EBrickState::TopRight;
        case Physics::ECollisionDirection::Right:
            return EBrickState::TopLeft;
        case Physics::ECollisionDirection::Top:
            return EBrickState::Destroyed;
        case Physics::ECollisionDirection::Bottom:
            return EBrickState::Destroyed;
        }
        break;

    case EBrickState::Bottom:
        switch (direction)
        {
        case Physics::ECollisionDirection::Left:
            return EBrickState::BottomRight;
        case Physics::ECollisionDirection::Right:
            return EBrickState::BottomLeft;
        case Physics::ECollisionDirection::Top:
            return EBrickState::Destroyed;
        case Physics::ECollisionDirection::Bottom:
            return EBrickState::Destroyed;
        }
        break;

    case EBrickState::Left:
        switch (direction)
        {
        case Physics::ECollisionDirection::Left:
            return EBrickState::Destroyed;
        case Physics::ECollisionDirection::Right:
            return EBrickState::Destroyed;
        case Physics::ECollisionDirection::Top:
            return EBrickState::BottomLeft;
        case Physics::ECollisionDirection::Bottom:
            return EBrickState::TopLeft;
        }
        break;

    case EBrickState::Right:
        switch (direction)
        {
        case Physics::ECollisionDirection::Left:
            return EBrickState::Destroyed;
        case Physics::ECollisionDirection::Right:
            return EBrickState::Destroyed;
        case Physics::ECollisionDirection::Top:
            return EBrickState::BottomRight;
        case Physics::ECollisionDirection::Bottom:
            return EBrickState::TopRight;
        }
        break;

    default:
        return EBrickState::Destroyed;
    }
}

Physics::AABB BrickWall::GetAABBForBrickState(const EBrickLocation location, const EBrickState eBrickState, const glm::vec2& size)
{
    glm::vec2 blockOffset(0);
    switch (location)
    {
    case EBrickLocation::BottomLeft:
        break;
    case EBrickLocation::BottomRight:
        blockOffset.x += size.x / 2;
        break;
    case EBrickLocation::TopLeft:
        blockOffset.y += size.y / 2;
        break;
    case EBrickLocation::TopRight:
        blockOffset += size / 2.f;
        break;
    }

    glm::vec2 bottomLeft(0);
    glm::vec2 topRight(0);
    switch (eBrickState)
    {
    case EBrickState::All:
        topRight = glm::vec2(size.x / 2, size.y / 2);
        break;
    case EBrickState::TopLeft:
        bottomLeft = glm::vec2(0, size.y / 4);
        topRight = glm::vec2(size.x / 4, size.y / 2);
        break;
    case EBrickState::TopRight:
        bottomLeft = glm::vec2(size.x / 4, size.y / 4);
        topRight = glm::vec2(size.x / 2, size.y / 2);
        break;
    case EBrickState::Top:
        bottomLeft = glm::vec2(0, size.y / 4);
        topRight = glm::vec2(size.x / 2, size.y / 2);
        break;
    case EBrickState::BottomLeft:
        topRight = glm::vec2(size.x / 4, size.y / 4);
        break;
    case EBrickState::Left:
        topRight = glm::vec2(size.x / 4, size.y / 2);
        break;
    case EBrickState::TopRight_BottomLeft:
        topRight = glm::vec2(size.x / 2, size.y / 2);
        break;
    case EBrickState::Top_BottomLeft:
        topRight = glm::vec2(size.x / 2, size.y / 2);
        break;
    case EBrickState::BottomRight:
        bottomLeft = glm::vec2(size.x / 4, 0);
        topRight = glm::vec2(size.x / 2, size.y / 4);
        break;
    case EBrickState::TopLeft_BottomRight:
        topRight = glm::vec2(size.x / 2, size.y / 2);
        break;
    case EBrickState::Right:
        bottomLeft = glm::vec2(size.x / 4, 0);
        topRight = glm::vec2(size.x / 2, size.y / 2);
        break;
    case EBrickState::Top_BottomRight:
        topRight = glm::vec2(size.x / 2, size.y / 2);
        break;
    case EBrickState::Bottom:
        topRight = glm::vec2(size.x / 2, size.y / 4);
        break;
    case EBrickState::TopLeft_Bottom:
        topRight = glm::vec2(size.x / 2, size.y / 2);
        break;
    case EBrickState::TopRight_Bottom:
        topRight = glm::vec2(size.x / 2, size.y / 2);
        break;
    case EBrickState::Destroyed:
        break;
    }

    return { bottomLeft + blockOffset, topRight + blockOffset };
}

void BrickWall::onCollisionCallback(const EBrickLocation location, const IGameObject& object, const Physics::ECollisionDirection direction)
{
    if (object.GetObjectType() != IGameObject::EObjectType::Bullet) return;
    const EBrickState newBrickState = GetBrickStateAfterCollision(eCurrentBrickState_[static_cast<size_t>(location)], direction);
    eCurrentBrickState_[static_cast<size_t>(location)] = newBrickState;
    if (newBrickState != EBrickState::Destroyed)
    {
        brickLocationToColliderMap_[static_cast<size_t>(location)]->boundingBox = GetAABBForBrickState(location, newBrickState, size_);
    }
    else
    {
        brickLocationToColliderMap_[static_cast<size_t>(location)]->isActive = false;
    }
}

void BrickWall::Render() const
{
    RenderBrick(EBrickLocation::TopLeft);
    RenderBrick(EBrickLocation::TopRight);
    RenderBrick(EBrickLocation::BottomLeft);
    RenderBrick(EBrickLocation::BottomRight);
}

