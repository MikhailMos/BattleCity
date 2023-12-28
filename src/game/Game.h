#pragma once

#include <array>
#include <memory>

#include "glm/vec2.hpp"

class Tank;
class Level;

class Game {
public:
	Game(const glm::ivec2& windowSize);
	~Game();

	void Render();
	void Update(const uint64_t delta);
	void SetKey(const int key, const int action);
	bool Init();

private:
	std::array<bool, 349> m_keys_; // нажатые клавиши

	enum EGameState {
		Active,
		Pause
	};

	EGameState m_eCurrentGameState_; // текущее состояние игрового мира
	glm::ivec2 m_windowSize_;		 // размер окна
	std::unique_ptr<Tank> m_p_tank_; // игровой объект
	std::unique_ptr<Level> m_p_level_; // уровень
};