#pragma once

#include <memory>

namespace RenderEngine {

	class Sprite;

	class SpriteAnimator
	{
	public:
		SpriteAnimator(std::shared_ptr<Sprite> pSprite);

		size_t GetCurrentFrame() const;
		void Update(const double delta);

	private:
		std::shared_ptr<Sprite> pSprite_;
		size_t current_frame_;
		double current_frame_duration_;
		double current_animation_time_;
	};

}