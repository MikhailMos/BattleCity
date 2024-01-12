#include "Tank.h"

#include "../../renderer/Sprite.h"
#include "../../resources/ResourceManager.h"


Tank::Tank(const double velocity, const glm::vec2& position, const glm::vec2& size, const float layer)
	: IGameObjcect(position, size, 0.f, layer)
	, e_orientation_(EOrientation::Top)
	, pSprite_top_(ResourceManager::GetSprite("player1_yellow_tank_type1_sprite_top"))
	, pSprite_bottom_(ResourceManager::GetSprite("player1_yellow_tank_type1_sprite_bottom"))
	, pSprite_left_(ResourceManager::GetSprite("player1_yellow_tank_type1_sprite_left"))
	, pSprite_right_(ResourceManager::GetSprite("player1_yellow_tank_type1_sprite_right"))
	, spriteAnimator_top_(pSprite_top_)
	, spriteAnimator_bottom_(pSprite_bottom_)
	, spriteAnimator_left_(pSprite_left_)
	, spriteAnimator_right_(pSprite_right_)
	, pSprite_respawn_(ResourceManager::GetSprite("respawn"))
	, spriteAnimator_respawn_(pSprite_respawn_)
	, pSprite_shield_(ResourceManager::GetSprite("shield"))
	, spriteAnimator_shield_(pSprite_shield_)
	, move_(false)
	, velocity_(velocity)
	, move_offset_(glm::vec2(0.f, 1.f))
	, isSpawning_(true)
	, hasShield_(false)
{
	respawnTimer_.SetCalback([&]()
		{
			isSpawning_ = false;
			hasShield_ = true;
			shieldTimer_.Start(2000);
		}
	);

	shieldTimer_.SetCalback([&]()
		{
			hasShield_ = false;			
		}
	);

	respawnTimer_.Start(1500);
}

void Tank::Render() const
{
	if (isSpawning_)
	{
		pSprite_respawn_->Render(position_, size_, rotation_, layer_, spriteAnimator_respawn_.GetCurrentFrame());
	}
	else
	{
		switch (e_orientation_)
		{
		case Tank::EOrientation::Top:
			pSprite_top_->Render(position_, size_, rotation_, layer_, spriteAnimator_top_.GetCurrentFrame());
			break;
		case Tank::EOrientation::Bottom:
			pSprite_bottom_->Render(position_, size_, rotation_, layer_, spriteAnimator_bottom_.GetCurrentFrame());
			break;
		case Tank::EOrientation::Left:
			pSprite_left_->Render(position_, size_, rotation_, layer_, spriteAnimator_left_.GetCurrentFrame());
			break;
		case Tank::EOrientation::Right:
			pSprite_right_->Render(position_, size_, rotation_, layer_, spriteAnimator_right_.GetCurrentFrame());
			break;
		}

		if (hasShield_) {
			pSprite_shield_->Render(position_, size_, rotation_, layer_, spriteAnimator_shield_.GetCurrentFrame());
		}
	}
	
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
		move_offset_.x = 0.f;
		move_offset_.y = 1.f;
		break;
	case Tank::EOrientation::Bottom:
		move_offset_.x = 0.f;
		move_offset_.y = -1.f;
		break;
	case Tank::EOrientation::Left:
		move_offset_.x = -1.f;
		move_offset_.y = 0.f;
		break;
	case Tank::EOrientation::Right:
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

void Tank::Update(const double delta)
{
	if (isSpawning_) 
	{
		spriteAnimator_respawn_.Update(delta);
		respawnTimer_.Update(delta);
	}
	else 
	{
		if (hasShield_)
		{
			spriteAnimator_shield_.Update(delta);
			shieldTimer_.Update(delta);
		}

		if (move_)
		{
			position_.x += static_cast<float>(delta * velocity_ * move_offset_.x);
			position_.y += static_cast<float>(delta * velocity_ * move_offset_.y);

			switch (e_orientation_)
			{
			case Tank::EOrientation::Top:
				spriteAnimator_top_.Update(delta);
				break;
			case Tank::EOrientation::Bottom:
				spriteAnimator_bottom_.Update(delta);
				break;
			case Tank::EOrientation::Left:
				spriteAnimator_left_.Update(delta);
				break;
			case Tank::EOrientation::Right:
				spriteAnimator_right_.Update(delta);
				break;
			}

		}
	}
	
}
