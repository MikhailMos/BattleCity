#include "SpriteAnimator.h"

#include "Sprite.h"

namespace RenderEngine {
	SpriteAnimator::SpriteAnimator(std::shared_ptr<Sprite> pSprite)
		: pSprite_(std::move(pSprite))
		, current_frame_(0)
		, current_frame_duration_(pSprite_->GetFrameDuration(0))
		, current_animation_time_(0)
	{
	}

	size_t SpriteAnimator::GetCurrentFrame() const
	{
		return current_frame_;
	}

	void SpriteAnimator::Update(const uint64_t delta)
	{
		current_animation_time_ += delta;

		while (current_animation_time_ >= current_frame_duration_)
		{
			current_animation_time_ -= current_frame_duration_;
			++current_frame_;

			if (current_frame_ == pSprite_->GetFramesCount()) {
				current_frame_ = 0;
			}

			current_frame_duration_ = pSprite_->GetFrameDuration(current_frame_);
		}
	}
}