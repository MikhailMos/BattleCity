#pragma once

#include "glm/vec2.hpp"

#include "../../physics/PhysicsEngine.h"

class IGameObject {
public:

	enum class EObjectType {
		BetonWall,
		Border,
		BrickWall,
		Bullet,
		Eagle,
		Ice,
		Tank,
		Trees,
		Water,

		Unknown
	};

	IGameObject(const EObjectType objectType, const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer);
	virtual void Render() const = 0;
	virtual void Update(const double delta) {};
	virtual ~IGameObject();
	virtual glm::vec2& GetCurrentPosition();
	virtual glm::vec2& GetCurrentDirection();
	virtual double GetCurrentVelocity();
	virtual void SetVelocity(const double velocity);

	const glm::vec2& GetSize() const;
	const std::vector<Physics::AABB>& GetColliders() const;
	EObjectType GetObjectType() const;
	virtual bool Collides(const EObjectType objectType);
	virtual void OnCollision() {}

protected:
	glm::vec2 position_;	// текущая позиция танка
	glm::vec2 size_;		// размер объекта
	float rotation_;		
	float layer_;			// глубина слоя
	EObjectType objectType_;

	glm::vec2 direction_;	// вектор направления
	double velocity_;		// скорость
	std::vector<Physics::AABB> colliders_;
};