#include "iGameObject.h"

IGameObjcect::IGameObjcect(const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer)
	: position_(position)
	, size_(size)
	, rotation_(rotation)
	, layer_(layer)
	, direction_(0, 1.f)
	, velocity_(0)
{
}

IGameObjcect::~IGameObjcect()
{
}

glm::vec2& IGameObjcect::GetCurrentPosition()
{
	return position_;
}

glm::vec2& IGameObjcect::GetCurrentDirection()
{
	return direction_;
}

double IGameObjcect::GetCurrentVelocity()
{
	return velocity_;
}

void IGameObjcect::SetVelocity(const double velocity)
{
	velocity_ = velocity;
}
