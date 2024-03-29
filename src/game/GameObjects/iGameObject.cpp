﻿#include "iGameObject.h"

IGameObject::IGameObject(const EObjectType objectType, const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer)
	: pOwner_(nullptr)
	, position_(position)
	, targetPosition_(position)
	, size_(size)
	, rotation_(rotation)
	, layer_(layer)
	, objectType_(objectType)
	, direction_(0, 1.f)
	, velocity_(0)
{
}

void IGameObject::SetOwner(IGameObject* pOwner)
{
	pOwner_ = pOwner;
}

IGameObject* IGameObject::GetOwner() const
{
	return pOwner_;
}

IGameObject::~IGameObject()
{
}

glm::vec2& IGameObject::GetCurrentPosition()
{
	return position_;
}

glm::vec2& IGameObject::GetTargetPosition()
{
	return targetPosition_;
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

const std::vector<Physics::Collider>& IGameObject::GetColliders() const
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
