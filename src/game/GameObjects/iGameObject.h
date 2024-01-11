#pragma once

#include "glm/vec2.hpp"

class IGameObjcect {
public:
	IGameObjcect(const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer);
	virtual void Render() const = 0;
	virtual void Update(const uint64_t delta) {};
	virtual ~IGameObjcect();

protected:
	glm::vec2 position_; // текущая позиция танка
	glm::vec2 size_;
	float rotation_;
	float layer_;
};