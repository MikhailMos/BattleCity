#pragma once

#include <array>

#include "glm/vec2.hpp"

class Game {
public:
	Game(const glm::ivec2& windowSize);
	~Game();

	void Render();
	void Update(const uint64_t delta);
	void SetKey(const int key, const int action);
	bool Init();

private:
	std::array<bool, 349> m_keys_;

	enum EGameState {
		Active,
		Pause
	};

	EGameState m_eCurrentGameState_;
	glm::ivec2 m_windowSize_;
};