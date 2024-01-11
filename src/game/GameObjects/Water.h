#pragma once

#include "iGameObject.h"
#include "../../renderer/SpriteAnimator.h"

#include <memory>
#include <array>

namespace RenderEngine {
	class Sprite;
}

class Water : public IGameObjcect
{
public:

   enum class EBlockLocation : uint8_t {
        TopLeft,
        TopRight,
        BottomLeft,
        BottomRight
    };

    Water(const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer);
    virtual void Render() const override;
    void Update(const uint64_t delta) override;

private:
    std::shared_ptr<RenderEngine::Sprite> sprite_;
    RenderEngine::SpriteAnimator spriteAnimator_;
    std::array<glm::vec2, 4> blockOffsets_;

    void RenderBlock(const EBlockLocation eBlockLocation) const;
};

