#pragma once

#include "iGameObject.h"

#include <memory>
#include <array>

namespace RenderEngine {
	class Sprite;
}

class Ice : public IGameObjcect
{
public:

   enum class EBlockLocation : uint8_t {
        TopLeft,
        TopRight,
        BottomLeft,
        BottomRight
    };

   Ice(const glm::vec2& position, const glm::vec2& size, const float rotation);
	virtual void Render() const override;

private:
   std::shared_ptr<RenderEngine::Sprite> sprite_;
    std::array<glm::vec2, 4> blockOffsets_;

    void RenderBlock(const EBlockLocation eBlockLocation) const;
};

