#pragma once

#include <array>
#include <memory>

#include "glm/vec2.hpp"

class IGameState;

namespace RenderEngine {
	class ShaderProgram;
}

class Game {
public:
	
	enum class EGameMode {
		OnePlayer,
		TwoPlayers
	};
	
	Game(const glm::uvec2& windowSize);
	~Game();

	void Render();
	void Update(const double delta);
	void SetKey(const int key, const int action);
	bool Init();
	unsigned int GetCurrentWidth() const;
	unsigned int GetCurrentHeight() const;
	void StartNewLevel(const size_t level, const EGameMode eGameMode);
	void NextLevel(const EGameMode eGameMode);
	void UpdateViewport();
	void SetWindowSize(const glm::uvec2& windowSize);

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

	EGameState eCurrentGameState_; // текущее состояние игрового мира
	glm::ivec2 windowSize_;		 // размер окна
	
	std::shared_ptr<IGameState> pCurrentGameState_;
	std::shared_ptr<RenderEngine::ShaderProgram> pSpriteShaderProgram_;
	size_t currentLevelIndex_;		 // индекс текущего уровня
};