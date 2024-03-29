﻿#include "Tank.h"

#include "../../renderer/Sprite.h"
#include "../../resources/ResourceManager.h"
#include "../../physics/PhysicsEngine.h"
#include "../AIComponent.h"
#include "Bullet.h"


const std::string& Tank::GetTankSpriteFromType(const ETankType eType)
{
	return TankTypeToSpriteString[static_cast<size_t>(eType)];
}

Tank::Tank(const Tank::ETankType eType, const bool bHasAI, const bool bShieldOnSpawn, const EOrientation eOrientation, const double maxVelocity, const glm::vec2& position, const glm::vec2& size, const float layer)
	: IGameObject(IGameObject::EObjectType::Tank, position, size, 0.f, layer)
	, e_orientation_(eOrientation)
	, pCurrentBullet_(std::make_shared<Bullet>(0.1, position_ + size_ / 4.f, size_ / 2.f, size_, layer))
	, pSprite_top_(ResourceManager::GetSprite(GetTankSpriteFromType(eType) + "_top"))
	, pSprite_bottom_(ResourceManager::GetSprite(GetTankSpriteFromType(eType) + "_bottom"))
	, pSprite_left_(ResourceManager::GetSprite(GetTankSpriteFromType(eType) + "_left"))
	, pSprite_right_(ResourceManager::GetSprite(GetTankSpriteFromType(eType) + "_right"))
	, spriteAnimator_top_(pSprite_top_)
	, spriteAnimator_bottom_(pSprite_bottom_)
	, spriteAnimator_left_(pSprite_left_)
	, spriteAnimator_right_(pSprite_right_)
	, pSprite_respawn_(ResourceManager::GetSprite("respawn"))
	, spriteAnimator_respawn_(pSprite_respawn_)
	, pSprite_shield_(ResourceManager::GetSprite("shield"))
	, spriteAnimator_shield_(pSprite_shield_)
	, maxVelocity_(maxVelocity)
	, isSpawning_(true)
	, hasShield_(false)
	, bShieldOnSpawn_(bShieldOnSpawn)
{
	SetOrientation(eOrientation);
	
	respawnTimer_.SetCallback([&]()
		{
			isSpawning_ = false;

			if (pAIComponent_)
			{
				velocity_ = maxVelocity_;
			}

			if (bShieldOnSpawn_)
			{
				hasShield_ = true;
				shieldTimer_.Start(2000);
			}
			
		}
	);

	shieldTimer_.SetCallback([&]()
		{
			hasShield_ = false;			
		}
	);

	respawnTimer_.Start(1500);

	colliders_.emplace_back(glm::vec2(0), size_);
	
	pCurrentBullet_->SetOwner(this);
	Physics::PhysicsEngine::AddDynamicGameObject(pCurrentBullet_);

	if (bHasAI)
	{
		pAIComponent_ = std::make_unique<AIComponent>(this);
	}
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
			pSprite_shield_->Render(position_, size_, rotation_, layer_ + 0.1f, spriteAnimator_shield_.GetCurrentFrame());
		}
	}
	
	if (pCurrentBullet_->IsActive())
	{
		pCurrentBullet_->Render();
	}

}

void Tank::SetOrientation(const EOrientation eOrientation)
{
	e_orientation_ = eOrientation;

	switch (e_orientation_)
	{
	case Tank::EOrientation::Top:
		direction_.x = 0.f;
		direction_.y = 1.f;
		break;
	case Tank::EOrientation::Bottom:
		direction_.x = 0.f;
		direction_.y = -1.f;
		break;
	case Tank::EOrientation::Left:
		direction_.x = -1.f;
		direction_.y = 0.f;
		break;
	case Tank::EOrientation::Right:
		direction_.x = 1.f;
		direction_.y = 0.f;
		break;
	}
}

void Tank::Update(const double delta)
{
	if (pCurrentBullet_->IsActive())
	{
		pCurrentBullet_->Update(delta);
	}
	
	if (isSpawning_) 
	{
		spriteAnimator_respawn_.Update(delta);
		respawnTimer_.Update(delta);
	}
	else 
	{
		if (pAIComponent_)
		{
			pAIComponent_->Update(delta);
		}
		
		if (hasShield_)
		{
			spriteAnimator_shield_.Update(delta);
			shieldTimer_.Update(delta);
		}

		if (velocity_ > 0)
		{
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

double Tank::GetMaxVelocity() const
{
	return maxVelocity_;
}

void Tank::SetVelocity(const double velocity)
{
	if (!isSpawning_)
	{
		velocity_ = velocity;
	}	
}

void Tank::Fire()
{
	if (!isSpawning_ && !pCurrentBullet_->IsActive())
	{
		pCurrentBullet_->Fire(position_ + size_ / 4.f + size_ * direction_ / 4.f, direction_);
	}
}


