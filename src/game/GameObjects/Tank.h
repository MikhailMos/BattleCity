#pragma once

#include <glm/vec2.hpp>
#include <memory>

#include "iGameObject.h"

namespace RenderEngine {
	class AnimatedSprite;
}

class Tank : IGameObjcect {
public:
	enum class EOrientation {
		Top,
		Bottom,
		Left,
		Right
	};

	Tank(std::shared_ptr<RenderEngine::AnimatedSprite> pSprite, const float velocity, const glm::vec2& position, const glm::vec2& size);

	void Render() const override;
	void SetOrientation(const EOrientation eOrientation);
	void Move(const bool move);
	void Update(const uint64_t delta) override;

private:
	EOrientation e_orientation_;
	std::shared_ptr<RenderEngine::AnimatedSprite> p_sprite_;
	bool move_;
	float velocity_; // скорость танка
	glm::vec2 move_offset_; // вектор направления танка

};