#pragma once

#include <glm/vec2.hpp>
#include <memory>

#include "iGameObject.h"
#include "../../renderer/SpriteAnimator.h"
#include "../../game_system/Timer.h"

namespace RenderEngine {
	class Sprite;
}

class Bullet : public IGameObject {
public:
	enum class EOrientation {
		Top,
		Bottom,
		Left,
		Right
	};

	Bullet(const double velocity,
		   const glm::vec2& position,
		   const glm::vec2& size,
		   const glm::vec2& explosionSize,
		   const float layer);

	void Render() const override;
	void Update(const double delta) override;
	bool IsActive() const;
	void Fire(const glm::vec2& position, const glm::vec2& direction);
	void OnCollision() override;

private:
	EOrientation e_orientation_;
	
	std::shared_ptr<RenderEngine::Sprite> pSprite_top_;
	std::shared_ptr<RenderEngine::Sprite> pSprite_bottom_;
	std::shared_ptr<RenderEngine::Sprite> pSprite_left_;
	std::shared_ptr<RenderEngine::Sprite> pSprite_right_;
	std::shared_ptr<RenderEngine::Sprite> pSprite_explosion_;
	
	RenderEngine::SpriteAnimator spriteAnimator_explosion_;
	
	Timer explosionTimer_;

	double maxVelocity_; // скорость пули
	bool isActive_;
	bool isExplosion_;
	glm::vec2 explosionSize_;
	glm::vec2 explosionOffset_;
};