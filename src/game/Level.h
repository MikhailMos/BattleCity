#pragma once

#include <vector>
#include <string>
#include <memory>
#include <glm/vec2.hpp>

class IGameObjcect;

class Level {
public:
	static constexpr unsigned int BLOCK_SIZE = 16;

	Level(const std::vector<std::string>& levelDescription);
	void Render() const;
	void Update(const double delta);
	size_t GetLevelWidth() const;
	size_t GetLevelHeight() const;

	const glm::ivec2& GetPlayerRespawn_1() const;
	const glm::ivec2& GetPlayerRespawn_2() const;
	const glm::ivec2& GetEnemyRespawn_1() const;
	const glm::ivec2& GetEnemyRespawn_2() const;
	const glm::ivec2& GetEnemyRespawn_3() const;

private:
	size_t width_;
	size_t height_;

	glm::ivec2 playerRespawn_1_;
	glm::ivec2 playerRespawn_2_;
	glm::ivec2 enemyRespawn_1_;
	glm::ivec2 enemyRespawn_2_;
	glm::ivec2 enemyRespawn_3_;


	std::vector<std::shared_ptr<IGameObjcect>> mapObjects_;
};