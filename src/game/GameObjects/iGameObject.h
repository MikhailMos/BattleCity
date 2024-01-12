#pragma once

#include "glm/vec2.hpp"

class IGameObjcect {
public:
	IGameObjcect(const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer);
	virtual void Render() const = 0;
	virtual void Update(const double delta) {};
	virtual ~IGameObjcect();
	virtual glm::vec2& GetCurrentPosition();
	virtual glm::vec2& GetCurrentDirection();
	virtual double GetCurrentVelocity();
	virtual void SetVelocity(const double velocity);

protected:
	glm::vec2 position_;	// текущая позиция танка
	glm::vec2 size_;
	float rotation_;
	float layer_;			// глубина слоя
	glm::vec2 direction_;	// вектор направления
	double velocity_;		// скорость
};