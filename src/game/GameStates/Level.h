#pragma once

#include <vector>
#include <string>
#include <memory>
#include <glm/vec2.hpp>

#include "IGameState.h"
#include "../Game.h"

class IGameObject;
class Tank;

class Level : public IGameState {
public:
	static constexpr unsigned int BLOCK_SIZE = 16;

	Level(const std::vector<std::string>& levelDescription, const Game::EGameMode eGameMode);
	virtual void Render() const override;
	virtual void Update(const double delta) override;
	virtual unsigned int GetStateWidth() const override;
	virtual unsigned int GetStateHeight() const override;
	virtual void ProcessInput(const std::array<bool, 349>& keys) override;

	const glm::ivec2& GetPlayerRespawn_1() const;
	const glm::ivec2& GetPlayerRespawn_2() const;
	const glm::ivec2& GetEnemyRespawn_1() const;
	const glm::ivec2& GetEnemyRespawn_2() const;
	const glm::ivec2& GetEnemyRespawn_3() const;

	std::vector<std::shared_ptr<IGameObject>> GetObjectsInArea(const glm::vec2& bottomLeft, glm::vec2& tomRight);
	void InitLevel();

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
	std::shared_ptr<Tank> pTank1_;
	std::shared_ptr<Tank> pTank2_;
	Game::EGameMode eGameMode_;
};