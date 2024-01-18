#include "SpriteAnimator.h"

#include "Sprite.h"

namespace RenderEngine {
	SpriteAnimator::SpriteAnimator(std::shared_ptr<Sprite> pSprite)
		: pSprite_(std::move(pSprite))
		, current_frame_(0)
		, current_frame_duration_(pSprite_->GetFrameDuration(0))
		, current_animation_time_(0)
		, total_duration_(0)
	{
		for (size_t currentFrameId = 0; currentFrameId < pSprite_->GetFramesCount(); ++currentFrameId)
		{
			total_duration_ += pSprite_->GetFrameDuration(currentFrameId);
		}
	}

	size_t SpriteAnimator::GetCurrentFrame() const
	{
		return current_frame_;
	}

	void SpriteAnimator::Update(const double delta)
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

	double SpriteAnimator::GetTotalDuration() const
	{
		return total_duration_;
	}

	void SpriteAnimator::Reset()
	{
		current_frame_ = 0;
		current_frame_duration_ = pSprite_->GetFrameDuration(0);
		current_animation_time_ = 0;
	}
}