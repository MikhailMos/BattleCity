#pragma once

#include <vector>
#include <string>
#include <memory>
#include <glm/vec2.hpp>

#include "IGameState.h"

class IGameObject;

class Level : public IGameState {
public:
	static constexpr unsigned int BLOCK_SIZE = 16;

	Level(const std::vector<std::string>& levelDescription);
	void Render() const;
	void Update(const double delta);
	virtual unsigned int GetStateWidth() const override;
	virtual unsigned int GetStateHeight() const override;

	const glm::ivec2& GetPlayerRespawn_1() const;
	const glm::ivec2& GetPlayerRespawn_2() const;
	const glm::ivec2& GetEnemyRespawn_1() const;
	const glm::ivec2& GetEnemyRespawn_2() const;
	const glm::ivec2& GetEnemyRespawn_3() const;

	std::vector<std::shared_ptr<IGameObject>> GetObjectsInArea(const glm::vec2& bottomLeft, glm::vec2& tomRight);

private:
	size_t widthBlocks_ = 0;
	size_t heightBlocks_ = 0;
	unsigned int widthPixels_ = 0;
	unsigned int heightPixels_ = 0;

	glm::ivec2 playerRespawn_1_;
	glm::ivec2 playerRespawn_2_;
	glm::ivec2 enemyRespawn_1_;
	glm::ivec2 enemyRespawn_2_;
	glm::ivec2 enemyRespawn_3_;


	std::vector<std::shared_ptr<IGameObject>> mapObjects_;
};