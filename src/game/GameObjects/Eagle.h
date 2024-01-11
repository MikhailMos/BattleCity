#pragma once

#include "iGameObject.h"

#include <memory>
#include <array>

namespace RenderEngine {
	class Sprite;
}

class Eagle : public IGameObjcect
{
public:

	enum class EEagleState : uint8_t {
		Alive = 0,
		Dead
	};

	Eagle(const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer);
	virtual void Render() const override;
	void Update(const uint64_t delta) override;

private:
	std::array<std::shared_ptr<RenderEngine::Sprite>, 2> sprites_;
	EEagleState eCurrentState_;
};

