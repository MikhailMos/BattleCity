#pragma once

#include "iGameObject.h"

#include <memory>
#include <array>

namespace RenderEngine {
	class Sprite;
}

class BetonWall : public IGameObjcect
{
public:

    enum class EBetonWallType : uint8_t {
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

    enum class EBlockState : uint8_t {
        Enabled = 0,
        Destroyed
    };

    enum class EBlockLocation : uint8_t {
        TopLeft,
        TopRight,
        BottomLeft,
        BottomRight
    };

    BetonWall(const EBetonWallType eBetonWallType, const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer);
	virtual void Render() const override;
	virtual void Update(const uint64_t delta) override;

private:
    std::array<EBlockState, 4> eCurrentBlockState_;
	std::shared_ptr<RenderEngine::Sprite> sprite_;
    std::array<glm::vec2, 4> blockOffsets_;

    void RenderBlock(const EBlockLocation eBlockLocation) const;
};

