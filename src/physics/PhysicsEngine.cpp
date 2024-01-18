#include "PhysicsEngine.h"

#include "../game/GameObjects/iGameObject.h"
#include "../game/Level.h"

namespace Physics {

	std::unordered_set<std::shared_ptr<IGameObject>> PhysicsEngine::dynamicObjects_;
	std::shared_ptr<Level> PhysicsEngine::pCurrentLevel_;

	void PhysicsEngine::Init()
	{
	}

	void PhysicsEngine::Terminate()
	{
		dynamicObjects_.clear();
		pCurrentLevel_.reset();
	}

	void PhysicsEngine::Update(const double delta)
	{
		for (auto& currentObject : dynamicObjects_)
		{
			if (currentObject->GetCurrentVelocity() > 0)
			{
				// allign position to multiple of 4 pixels
				if (currentObject->GetCurrentDirection().x != 0.f) // right and left
				{
					currentObject->GetCurrentPosition() = glm::vec2(currentObject->GetCurrentPosition().x, static_cast<unsigned int>(currentObject->GetCurrentPosition().y / 4.f + 0.5f) * 4.f);
				}
				else if (currentObject->GetCurrentDirection().y != 0.f) // top and bottom
				{
					currentObject->GetCurrentPosition() = glm::vec2(static_cast<unsigned int>(currentObject->GetCurrentPosition().x / 4.f + 0.5f) * 4.f, currentObject->GetCurrentPosition().y);
				}

				const auto newPosition = currentObject->GetCurrentPosition() + currentObject->GetCurrentDirection() * static_cast<float>(currentObject->GetCurrentVelocity() * delta);
				const auto& colliders = currentObject->GetColliders();
				std::vector<std::shared_ptr<IGameObject>> objectToCheck = pCurrentLevel_->GetObjectsInArea(newPosition, newPosition + currentObject->GetSize());
				
				bool hasCollision = false;

				for (const auto& currentObjectToCheck : objectToCheck)
				{
					const auto& collidersToCheck = currentObjectToCheck->GetColliders();
					//if (!collidersToCheck.empty())
					if (currentObjectToCheck->Collides(currentObject->GetObjectType()) && !collidersToCheck.empty())
					{
						if (HasIntersection(colliders, newPosition, collidersToCheck, currentObjectToCheck->GetCurrentPosition()))
						{
							hasCollision = true;
							currentObjectToCheck->OnCollision();
							break;
						}
					}
				}

				if (!hasCollision)
				{
					currentObject->GetCurrentPosition() = newPosition;
				}
				else
				{
					// allign position to multiple of 8 pixels
					if (currentObject->GetCurrentDirection().x != 0.f) // right and left
					{
						currentObject->GetCurrentPosition() = glm::vec2(static_cast<unsigned int>(currentObject->GetCurrentPosition().x / 8.f + 0.5f) * 8.f, currentObject->GetCurrentPosition().y);
					}
					else if (currentObject->GetCurrentDirection().y != 0.f) // top and bottom
					{
						currentObject->GetCurrentPosition() = glm::vec2(currentObject->GetCurrentPosition().x, static_cast<unsigned int>(currentObject->GetCurrentPosition().y / 8.f + 0.5f) * 8.f);
					}

					currentObject->OnCollision();
				}
			}
		}
	}

	void PhysicsEngine::AddDynamicGameObject(std::shared_ptr<IGameObject> pGameObject)
	{
		dynamicObjects_.insert(std::move(pGameObject));
	}

	void PhysicsEngine::SetCurrentLevel(std::shared_ptr<Level> pLevel)
	{
		pCurrentLevel_.swap(pLevel);
	}

	bool PhysicsEngine::HasIntersection(const std::vector<AABB>& colliders1, const glm::vec2& position1,
										const std::vector<AABB>& colliders2, const glm::vec2& position2)
	{
		for (const auto& currentCollider1 : colliders1)
		{
			const glm::vec2 currentCollider1_bottomLeft_word = currentCollider1.bottomLeft + position1;
			const glm::vec2 currentCollider1_topRight_word   = currentCollider1.topRight   + position1;

			for (const auto& currentCollider2 : colliders2)
			{
				const glm::vec2 currentCollider2_bottomLeft_word = currentCollider2.bottomLeft + position2;
				const glm::vec2 currentCollider2_topRight_word	 = currentCollider2.topRight   + position2;

				if (currentCollider1_bottomLeft_word.x >= currentCollider2_topRight_word.x)
				{
					continue;
				}

				if (currentCollider1_topRight_word.x <= currentCollider2_bottomLeft_word.x)
				{
					continue;
				}

				if (currentCollider1_bottomLeft_word.y >= currentCollider2_topRight_word.y)
				{
					continue;
				}

				if (currentCollider1_topRight_word.y <= currentCollider2_bottomLeft_word.y)
				{
					continue;
				}

				return true;
			}
		}

		return false;
	}

}