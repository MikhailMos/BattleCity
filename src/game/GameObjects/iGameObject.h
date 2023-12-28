#pragma once

#include "glm/vec2.hpp"

class IGameObjcect {
public:
	IGameObjcect(const glm::vec2& position, const glm::vec2& size, const float rotation);
	virtual void Render() const = 0;
	virtual void Update(const uint64_t delta) = 0;
	virtual ~IGameObjcect();

protected:
	glm::vec2 position_; // текущая позиция танка
	glm::vec2 size_;
	float rotation_ = 0.f;
};