#pragma once

#include <memory>

namespace RenderEngine {

	class Sprite;

	class SpriteAnimator
	{
	public:
		SpriteAnimator(std::shared_ptr<Sprite> pSprite);

		size_t GetCurrentFrame() const;
		void Update(const uint64_t delta);

	private:
		std::shared_ptr<Sprite> pSprite_;
		size_t current_frame_;
		uint64_t current_frame_duration_;
		uint64_t current_animation_time_;
	};

}