#include "Tank.h"

#include "../renderer/AnimatedSprite.h"

Tank::Tank(std::shared_ptr<RenderEngine::AnimatedSprite> pSprite, const float velocity, const glm::vec2& position)
	: e_orientation_(EOrientation::Top)
	, p_sprite_(std::move(pSprite))
	, move_(false)
	, velocity_(velocity)
	, position_(position)
	, move_offset_(glm::vec2(0.f, 1.f))
{
	p_sprite_->SetPosition(position_);
}

void Tank::Render() const
{
	p_sprite_->Render();
}

void Tank::SetOrientation(const EOrientation eOrientation)
{
	if (e_orientation_ == eOrientation)
	{
		return;
	}

	e_orientation_ = eOrientation;
	switch (e_orientation_)
	{
	case Tank::EOrientation::Top:
		p_sprite_->SetState("tankTopState");
		move_offset_.x = 0.f;
		move_offset_.y = 1.f;
		break;
	case Tank::EOrientation::Bottom:
		p_sprite_->SetState("tankBottomState");
		move_offset_.x = 0.f;
		move_offset_.y = -1.f;
		break;
	case Tank::EOrientation::Left:
		p_sprite_->SetState("tankLeftState");
		move_offset_.x = -1.f;
		move_offset_.y = 0.f;
		break;
	case Tank::EOrientation::Right:
		p_sprite_->SetState("tankRightState");
		move_offset_.x = 1.f;
		move_offset_.y = 0.f;
		break;
	default:
		break;
	}
}

void Tank::Move(const bool move)
{
	move_ = move;
}

void Tank::Update(const uint64_t delta)
{
	if (move_) 
	{
		position_ += delta * velocity_ * move_offset_;
		p_sprite_->SetPosition(position_);
		p_sprite_->Update(delta);
	}
}
