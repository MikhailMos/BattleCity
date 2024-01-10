#pragma once

#include <glm/vec2.hpp>
#include <memory>

#include "iGameObject.h"
#include "../../renderer/SpriteAnimator.h"

namespace RenderEngine {
	class Sprite;
}

class Tank : IGameObjcect {
public:
	enum class EOrientation {
		Top,
		Bottom,
		Left,
		Right
	};

	Tank(const float velocity, 
		const glm::vec2& position, 
		const glm::vec2& size);

	void Render() const override;
	void SetOrientation(const EOrientation eOrientation);
	void Move(const bool move);
	void Update(const uint64_t delta) override;

private:
	EOrientation e_orientation_;
	std::shared_ptr<RenderEngine::Sprite> pSprite_top_;
	std::shared_ptr<RenderEngine::Sprite> pSprite_bottom_;
	std::shared_ptr<RenderEngine::Sprite> pSprite_left_;
	std::shared_ptr<RenderEngine::Sprite> pSprite_right_;
	RenderEngine::SpriteAnimator spriteAnimator_top_;
	RenderEngine::SpriteAnimator spriteAnimator_bottom_;
	RenderEngine::SpriteAnimator spriteAnimator_left_;
	RenderEngine::SpriteAnimator spriteAnimator_right_;
	
	bool move_;
	float velocity_; // скорость танка
	glm::vec2 move_offset_; // вектор направления танка

};