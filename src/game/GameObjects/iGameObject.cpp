#include "iGameObject.h"

IGameObject::IGameObject(const EObjectType objectType, const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer)
	: position_(position)
	, size_(size)
	, rotation_(rotation)
	, layer_(layer)
	, objectType_(objectType)
	, direction_(0, 1.f)
	, velocity_(0)
{
}

IGameObject::~IGameObject()
{
}

glm::vec2& IGameObject::GetCurrentPosition()
{
	return position_;
}

glm::vec2& IGameObject::GetCurrentDirection()
{
	return direction_;
}

double IGameObject::GetCurrentVelocity()
{
	return velocity_;
}

void IGameObject::SetVelocity(const double velocity)
{
	velocity_ = velocity;
}

const glm::vec2& IGameObject::GetSize() const
{
	return size_;
}

const std::vector<Physics::AABB>& IGameObject::GetColliders() const
{
	return colliders_;
}

IGameObject::EObjectType IGameObject::GetObjectType() const
{
	return objectType_;
}

bool IGameObject::Collides(const EObjectType objectType)
{
	return true;
}
