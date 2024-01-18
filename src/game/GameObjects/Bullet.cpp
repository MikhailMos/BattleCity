#include "Tank.h"

#include "../../renderer/Sprite.h"
#include "../../resources/ResourceManager.h"
#include "Bullet.h"


Bullet::Bullet(const double velocity,
			   const glm::vec2& position,
			   const glm::vec2& size,
			   const glm::vec2& explosionSize,
			   const float layer)
	: IGameObject(IGameObject::EObjectType::Bullet, position, size, 0.f, layer)
	, e_orientation_(EOrientation::Top)
	, pSprite_top_(ResourceManager::GetSprite("bullet_Top"))
	, pSprite_bottom_(ResourceManager::GetSprite("bullet_Bottom"))
	, pSprite_left_(ResourceManager::GetSprite("bullet_Left"))
	, pSprite_right_(ResourceManager::GetSprite("bullet_Right"))
	, pSprite_explosion_(ResourceManager::GetSprite("explosion"))
	, spriteAnimator_explosion_(pSprite_explosion_)
	, maxVelocity_(velocity)
	, isActive_(false)
	, isExplosion_(false)
	, explosionSize_(explosionSize)
	, explosionOffset_((explosionSize - size) / 2.f)
{
	auto onCollisionCallback = [&](const IGameObject& object, const Physics::ECollisionDirection)
    {
        SetVelocity(0);
        isExplosion_ = true;
        explosionTimer_.Start(spriteAnimator_explosion_.GetTotalDuration());
    };
    colliders_.emplace_back(glm::vec2(0), size_, onCollisionCallback);
	
    explosionTimer_.SetCallback([&]()
        {
            isExplosion_ = false;
            isActive_ = false;
            spriteAnimator_explosion_.Reset();
        }
    );
}

void Bullet::Render() const
{
	if (isExplosion_)
	{
		switch (e_orientation_)
		{
		case EOrientation::Top:
			pSprite_explosion_->Render(position_ - explosionOffset_ + glm::vec2(0, size_.y / 2.f), explosionSize_, rotation_, layer_ + 0.1f, spriteAnimator_explosion_.GetCurrentFrame());
			break;
		case EOrientation::Bottom:
			pSprite_explosion_->Render(position_ - explosionOffset_ - glm::vec2(0, size_.y / 2.f), explosionSize_, rotation_, layer_ + 0.1f, spriteAnimator_explosion_.GetCurrentFrame());
			break;
		case EOrientation::Left:
			pSprite_explosion_->Render(position_ - explosionOffset_ - glm::vec2(size_.x / 2.f, 0), explosionSize_, rotation_, layer_ + 0.1f, spriteAnimator_explosion_.GetCurrentFrame());
			break;
		case EOrientation::Right:
			pSprite_explosion_->Render(position_ - explosionOffset_ + glm::vec2(size_.x / 2.f, 0), explosionSize_, rotation_, layer_ + 0.1f, spriteAnimator_explosion_.GetCurrentFrame());
			break;
		}
	}
	else if (isActive_)
	{
		switch (e_orientation_)
		{
		case EOrientation::Top:
			pSprite_top_->Render(position_, size_, rotation_, layer_);
			break;
		case EOrientation::Bottom:
			pSprite_bottom_->Render(position_, size_, rotation_, layer_);
			break;
		case EOrientation::Left:
			pSprite_left_->Render(position_, size_, rotation_, layer_);
			break;
		case EOrientation::Right:
			pSprite_right_->Render(position_, size_, rotation_, layer_);
			break;
		}
	}
	
}

void Bullet::Update(const double delta)
{
	if (isExplosion_)
	{
		spriteAnimator_explosion_.Update(delta);
		explosionTimer_.Update(delta);
	}
	
}

bool Bullet::IsActive() const
{
	return isActive_;
}

void Bullet::Fire(const glm::vec2& position, const glm::vec2& direction)
{
	position_ = position;
	direction_ = direction;
	if (direction_.x == 0.f)
	{
		e_orientation_ = (direction_.y < 0) ? EOrientation::Bottom : EOrientation::Top;
	}
	else
	{
		e_orientation_ = (direction_.x < 0) ? EOrientation::Left : EOrientation::Right;
	}
	isActive_ = true;
	SetVelocity(maxVelocity_);
}



