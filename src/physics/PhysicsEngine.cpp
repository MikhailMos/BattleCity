#include "PhysicsEngine.h"

#include "../game/GameObjects/iGameObject.h"

std::unordered_set<std::shared_ptr<IGameObjcect>> PhysicsEngine::dynamicObjects_;

void PhysicsEngine::Init()
{
}

void PhysicsEngine::Terminate()
{
	dynamicObjects_.clear();
}

void PhysicsEngine::Update(const double delta)
{
	for (auto& currentObject : dynamicObjects_)
	{
		if (currentObject->GetCurrentVelocity() > 0)
		{
			currentObject->GetCurrentPosition() += currentObject->GetCurrentDirection() * static_cast<float>(currentObject->GetCurrentVelocity() * delta);
		}
	}
}

void PhysicsEngine::AddDynamicGameObject(std::shared_ptr<IGameObjcect> pGameObject)
{
	dynamicObjects_.insert(std::move(pGameObject));
}
