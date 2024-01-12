#pragma once

#include <glm/vec2.hpp>
#include <memory>

#include "iGameObject.h"
#include "../../renderer/SpriteAnimator.h"
#include "../../game_system/Timer.h"

namespace RenderEngine {
	class Sprite;
}

class Tank : public IGameObjcect {
public:
	enum class EOrientation {
		Top,
		Bottom,
		Left,
		Right
	};

	Tank(const double maxVelocity,
		const glm::vec2& position, 
		const glm::vec2& size,
		const float layer);

	void Render() const override;
	void SetOrientation(const EOrientation eOrientation);
	void Update(const double delta) override;
	double GetMaxVelocity() const;
	void SetVelocity(const double velocity) override;

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

	std::shared_ptr<RenderEngine::Sprite> pSprite_respawn_;
	RenderEngine::SpriteAnimator spriteAnimator_respawn_;

	std::shared_ptr<RenderEngine::Sprite> pSprite_shield_;
	RenderEngine::SpriteAnimator spriteAnimator_shield_;
	
	Timer respawnTimer_;
	Timer shieldTimer_;

	double maxVelocity_; // скорость танка
	bool isSpawning_;
	bool hasShield_;

};