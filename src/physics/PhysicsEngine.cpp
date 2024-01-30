#include "PhysicsEngine.h"

#include "../game/GameObjects/iGameObject.h"
#include "../game/GameStates/Level.h"

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
		CalculateTargetPositions(dynamicObjects_, delta);

		// check collision
		for (auto it1 = dynamicObjects_.begin(); it1 != dynamicObjects_.end();)
		{
			auto pObject1 = *it1;
			for (auto it2 = ++it1; it2 != dynamicObjects_.end(); ++it2)
			{
				auto pObject2 = *it2;
				if (pObject1->GetOwner() == pObject2.get() || pObject2->GetOwner() == pObject1.get())
				{
					continue;
				}

				if (!HasPositionIntersection(pObject1, pObject1->GetTargetPosition(), pObject2, pObject2->GetTargetPosition()))
				{
					continue;
				}

				if (HasPositionIntersection(pObject1, pObject1->GetTargetPosition(), pObject2, pObject2->GetCurrentPosition()))
				{
					pObject1->GetTargetPosition() = pObject1->GetCurrentPosition();
				}

				if (HasPositionIntersection(pObject1, pObject1->GetCurrentPosition(), pObject2, pObject2->GetTargetPosition()))
				{
					pObject2->GetTargetPosition() = pObject2->GetCurrentPosition();
				}
				
			}
		}

		UpdatePositions(dynamicObjects_);
	}

	void PhysicsEngine::AddDynamicGameObject(std::shared_ptr<IGameObject> pGameObject)
	{
		dynamicObjects_.insert(std::move(pGameObject));
	}

	void PhysicsEngine::SetCurrentLevel(std::shared_ptr<Level> pLevel)
	{
		pCurrentLevel_.swap(pLevel);
		dynamicObjects_.clear();
		pCurrentLevel_->InitLevel();
	}

	bool PhysicsEngine::HasColliderIntersection(const Collider& collider1, const glm::vec2& position1,
												const Collider& collider2, const glm::vec2& position2)
	{
		const glm::vec2 currentCollider1_bottomLeft_word = collider1.boundingBox.bottomLeft + position1;
		const glm::vec2 currentCollider1_topRight_word   = collider1.boundingBox.topRight   + position1;

		const glm::vec2 currentCollider2_bottomLeft_word = collider2.boundingBox.bottomLeft + position2;
		const glm::vec2 currentCollider2_topRight_word	 = collider2.boundingBox.topRight   + position2;

		if (currentCollider1_bottomLeft_word.x >= currentCollider2_topRight_word.x)
		{
			return false;
		}

		if (currentCollider1_topRight_word.x <= currentCollider2_bottomLeft_word.x)
		{
			return false;
		}

		if (currentCollider1_bottomLeft_word.y >= currentCollider2_topRight_word.y)
		{
			return false;
		}

		if (currentCollider1_topRight_word.y <= currentCollider2_bottomLeft_word.y)
		{
			return false;
		}

		return true;
	}

	bool PhysicsEngine::HasPositionIntersection(const std::shared_ptr<IGameObject>& pObject1, const glm::vec2& position1, const std::shared_ptr<IGameObject>& pObject2, const glm::vec2& position2)
	{
		const auto& currentObjectColliders = pObject1->GetColliders();
		const auto& otherObjectColliders = pObject2->GetColliders();
		
		for (const auto& currentObjectCoolider : currentObjectColliders)
		{
			for (const auto& otherObjectCoolider : otherObjectColliders)
			{
				if (HasColliderIntersection(currentObjectCoolider, position1, otherObjectCoolider, position2))
				{
					return true;
				}
			}
		}
		
		return false;
	}

	void PhysicsEngine::CalculateTargetPositions(std::unordered_set<std::shared_ptr<IGameObject>>& dynamicObjects, const double delta)
	{
		for (auto& currentDynamicObject : dynamicObjects)
		{
			if (currentDynamicObject->GetCurrentVelocity() > 0)
			{
				// allign position to multiple of 4 pixels
				if (currentDynamicObject->GetCurrentDirection().x != 0.f) // right and left
				{
					currentDynamicObject->GetTargetPosition() = glm::vec2(currentDynamicObject->GetCurrentPosition().x, static_cast<unsigned int>(currentDynamicObject->GetCurrentPosition().y / 4.f + 0.5f) * 4.f);
				}
				else if (currentDynamicObject->GetCurrentDirection().y != 0.f) // top and bottom
				{
					currentDynamicObject->GetTargetPosition() = glm::vec2(static_cast<unsigned int>(currentDynamicObject->GetCurrentPosition().x / 4.f + 0.5f) * 4.f, currentDynamicObject->GetCurrentPosition().y);
				}

				const auto newPosition = currentDynamicObject->GetTargetPosition() + currentDynamicObject->GetCurrentDirection() * static_cast<float>(currentDynamicObject->GetCurrentVelocity() * delta);
				std::vector<std::shared_ptr<IGameObject>> objectToCheck = pCurrentLevel_->GetObjectsInArea(newPosition, newPosition + currentDynamicObject->GetSize());

				const auto& colliders = currentDynamicObject->GetColliders();
				bool hasCollision = false;

				ECollisionDirection dynamicObjectCollisionDirection = ECollisionDirection::Right;
				if (currentDynamicObject->GetCurrentDirection().x < 0)
				{
					dynamicObjectCollisionDirection = ECollisionDirection::Left;
				}
				else if (currentDynamicObject->GetCurrentDirection().y > 0)
				{
					dynamicObjectCollisionDirection = ECollisionDirection::Top;
				}
				else if (currentDynamicObject->GetCurrentDirection().y < 0)
				{
					dynamicObjectCollisionDirection = ECollisionDirection::Bottom;
				}

				ECollisionDirection objectCollisionDirection = ECollisionDirection::Left;
				if (currentDynamicObject->GetCurrentDirection().x < 0)
				{
					objectCollisionDirection = ECollisionDirection::Right;
				}
				else if (currentDynamicObject->GetCurrentDirection().y > 0)
				{
					objectCollisionDirection = ECollisionDirection::Bottom;
				}
				else if (currentDynamicObject->GetCurrentDirection().y < 0)
				{
					objectCollisionDirection = ECollisionDirection::Top;
				}

				for (const auto& currentDynamicObjectCollider : colliders)
				{
					for (const auto& currentObjectToCheck : objectToCheck)
					{
						const auto& collidersToCheck = currentObjectToCheck->GetColliders();
						if (currentObjectToCheck->Collides(currentDynamicObject->GetObjectType()) && !collidersToCheck.empty())
						{
							for (const auto& currentObjectCollider : currentObjectToCheck->GetColliders())
							{
								if (currentObjectCollider.isActive && HasColliderIntersection(currentDynamicObjectCollider, newPosition, currentObjectCollider, currentObjectToCheck->GetCurrentPosition()))
								{
									hasCollision = true;
									if (currentObjectCollider.onCollisionCallback)
									{
										currentObjectCollider.onCollisionCallback(*currentDynamicObject, objectCollisionDirection);
									}

									if (currentDynamicObjectCollider.onCollisionCallback)
									{
										currentDynamicObjectCollider.onCollisionCallback(*currentObjectToCheck, dynamicObjectCollisionDirection);
									}
								}
							}
						}
					}
				}

				if (!hasCollision)
				{
					currentDynamicObject->GetTargetPosition() = newPosition;
				}
				else
				{
					// allign position to multiple of 8 pixels
					if (currentDynamicObject->GetCurrentDirection().x != 0.f) // right and left
					{
						currentDynamicObject->GetTargetPosition() = glm::vec2(static_cast<unsigned int>(currentDynamicObject->GetTargetPosition().x / 8.f + 0.5f) * 8.f, currentDynamicObject->GetTargetPosition().y);
					}
					else if (currentDynamicObject->GetCurrentDirection().y != 0.f) // top and bottom
					{
						currentDynamicObject->GetTargetPosition() = glm::vec2(currentDynamicObject->GetTargetPosition().x, static_cast<unsigned int>(currentDynamicObject->GetTargetPosition().y / 8.f + 0.5f) * 8.f);
					}

					currentDynamicObject->OnCollision();
				}
			}
		}
	}

	void PhysicsEngine::UpdatePositions(std::unordered_set<std::shared_ptr<IGameObject>>& dynamicObjects)
	{
		for (auto& currentDynamicObject : dynamicObjects)
		{
			currentDynamicObject->GetCurrentPosition() = currentDynamicObject->GetTargetPosition();
		}
	}

}