﻿#pragma once

#include "iGameObject.h"

#include <memory>
#include <array>

namespace RenderEngine {
	class Sprite;
}

class BrickWall : public IGameObject
{
public:

    enum class EBrickWallType : uint8_t {
        All,
        Top,
        Bottom,
        Left,
        Right,
        TopLeft,
        TopRight,
        BottomLeft,
        BottomRight
    };

    enum class EBrickState : uint8_t {
        All = 0,
        TopLeft,
        TopRight,
        Top,
        BottomLeft,
        Left,
        TopRight_BottomLeft,
        Top_BottomLeft,
        BottomRight,
        TopLeft_BottomRight,
        Right,
        Top_BottomRight,
        Bottom,
        TopLeft_Bottom,
        TopRight_Bottom,
        Destroyed
    };

    enum class EBrickLocation : uint8_t {
        TopLeft,
        TopRight,
        BottomLeft,
        BottomRight
    };

	BrickWall(const EBrickWallType eBrickWallType, const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer);
	virtual void Render() const override;

private:
    std::array<EBrickState, 4> eCurrentBrickState_;
	std::array<std::shared_ptr<RenderEngine::Sprite>, 15> sprites_;
    std::array<glm::vec2, 4> blockOffsets_;
    std::array<Physics::Collider*, 4> brickLocationToColliderMap_;

    void RenderBrick(const EBrickLocation eBrickLocation) const;
    static EBrickState GetBrickStateAfterCollision(const EBrickState currentState, const Physics::ECollisionDirection direction);
    static Physics::AABB GetAABBForBrickState(const EBrickLocation location, const EBrickState eBrickState, const glm::vec2& size);
    void onCollisionCallback(const EBrickLocation location, const IGameObject& object, const Physics::ECollisionDirection direction);


};

