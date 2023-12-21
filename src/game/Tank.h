#pragma once

#include <glm/vec2.hpp>
#include <memory>

namespace Renderer {
	class AnimatedSprite;
}

class Tank {
public:
	enum class EOrientation {
		Top,
		Bottom,
		Left,
		Right
	};

	Tank(std::shared_ptr<Renderer::AnimatedSprite> pSprite, const float velocity, const glm::vec2& position);

	void Render() const;
	void SetOrientation(const EOrientation eOrientation);
	void Move(const bool move);
	void Update(const uint64_t delta);

private:
	EOrientation e_orientation_;
	std::shared_ptr<Renderer::AnimatedSprite> p_sprite_;
	bool move_;
	float velocity_; // скорость танка
	glm::vec2 position_; // текущая позиция танка
	glm::vec2 move_offset_; // вектор направления танка

};