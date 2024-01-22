#pragma once

#include <array>
#include <memory>

#include "glm/vec2.hpp"

class Tank;
class Level;
class StartScreen;

class Game {
public:
	
	enum class EGameMode {
		OnePlayer,
		TwoPlayers
	};
	
	Game(const glm::ivec2& windowSize);
	~Game();

	void Render();
	void Update(const double delta);
	void SetKey(const int key, const int action);
	bool Init();
	unsigned int GetCurrentWidth() const;
	unsigned int GetCurrentHeight() const;
	void StartNewLevel(const size_t level, const EGameMode eGameMode);

private:
	
	enum class EGameState {
		StartScreen,
		LevelStart,
		Level,
		Pause,
		Scores,
		GameOver
	};

	std::array<bool, 349> keys_; // нажатые клавиши

	EGameState m_eCurrentGameState_; // текущее состояние игрового мира
	glm::ivec2 m_windowSize_;		 // размер окна
	std::shared_ptr<Tank> m_p_tank_; // игровой объект
	std::shared_ptr<Level> m_p_level_; // уровень

	std::shared_ptr<StartScreen> pStartScreen_;
};