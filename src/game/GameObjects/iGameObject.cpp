#include "iGameObject.h"

IGameObjcect::IGameObjcect(const glm::vec2& position, const glm::vec2& size, const float rotation)
	: position_(position)
	, size_(size)
	, rotation_(rotation)
{
}

IGameObjcect::~IGameObjcect()
{
}
