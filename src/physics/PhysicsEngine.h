#pragma once

#include <unordered_set>
#include <memory>

class IGameObjcect;

class PhysicsEngine {
public:
	~PhysicsEngine() = delete;
	PhysicsEngine() = delete;
	PhysicsEngine(const PhysicsEngine&) = delete;
	PhysicsEngine& operator=(const PhysicsEngine&) = delete;
	PhysicsEngine& operator=(PhysicsEngine&&) = delete;
	PhysicsEngine(PhysicsEngine&&) = delete;

	static void Init();
	static void Terminate();

	static void Update(const double delta);
	static void AddDynamicGameObject(std::shared_ptr<IGameObjcect> pGameObject);

private:
	static std::unordered_set<std::shared_ptr<IGameObjcect>> dynamicObjects_;
	
};