#include "Game.h"

#include "../resources/ResourceManager.h"
#include "../renderer/Texture2D.h"
#include "../renderer/ShaderProgram.h"
#include "../renderer/Sprite.h"

#include "GameObjects/Tank.h"
#include "GameObjects/Bullet.h"

#include "GameStates/StartScreen.h"
#include "GameStates/Level.h"
#include "../physics/PhysicsEngine.h"
#include "../renderer/Renderer.h"

#include "GLFW/glfw3.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/mat4x4.hpp"

#include <iostream>

Game::Game(const glm::uvec2& windowSize)
	: eCurrentGameState_(EGameState::StartScreen)
    , windowSize_(windowSize)
    , currentLevelIndex_(0)
{
	keys_.fill(false);
}

Game::~Game()
{
}

void Game::Render()
{
    pCurrentGameState_->Render();
}

void Game::Update(const double delta)
{
    pCurrentGameState_->ProcessInput(keys_);
    pCurrentGameState_->Update(delta);
}

void Game::SetKey(const int key, const int action)
{
	keys_[key] = action;
}

bool Game::Init()
{
    ResourceManager::LoadJSONResources("resources/resources.json");
    
    pSpriteShaderProgram_ = ResourceManager::GetShaderProgram("spriteShader");
    if (!pSpriteShaderProgram_)
    {
        std::cerr << "Can't find shader program: " << "spriteShader" << std::endl;
        return false;
    }

    pSpriteShaderProgram_->Use();
    pSpriteShaderProgram_->SetInt("tex", 0);

    pCurrentGameState_ = std::make_shared<StartScreen>(ResourceManager::GetStartScreen(), this);
    SetWindowSize(windowSize_);

    return true;
}

unsigned int Game::GetCurrentWidth() const
{
    return pCurrentGameState_->GetStateWidth();
}

unsigned int Game::GetCurrentHeight() const
{
    return pCurrentGameState_->GetStateHeight();
}

void Game::UpdateViewport()
{
    const float map_aspect_ratio = static_cast<float>(GetCurrentWidth()) / GetCurrentHeight();
    unsigned int viewPortWidth = windowSize_.x;
    unsigned int viewPortHeight = windowSize_.y;
    unsigned int viewPortLeftOffset = 0;
    unsigned int viewPortBottomOffset = 0;

    if (static_cast<float>(windowSize_.x) / windowSize_.y > map_aspect_ratio)
    {
        viewPortWidth = static_cast<unsigned int>(windowSize_.y * map_aspect_ratio);
        viewPortLeftOffset = (windowSize_.x - viewPortWidth) / 2;
    }
    else
    {
        viewPortHeight = static_cast<unsigned int>(windowSize_.x / map_aspect_ratio);
        viewPortBottomOffset = (windowSize_.y - viewPortHeight) / 2;
    }

    RenderEngine::Renderer::SetViewport(viewPortWidth, viewPortHeight, viewPortLeftOffset, viewPortBottomOffset);

    glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(GetCurrentWidth()), 0.f, static_cast<float>(GetCurrentHeight()), -100.f, 100.f);

    pSpriteShaderProgram_->SetMatrix4("projectionMat", projectionMatrix);
}

void Game::StartNewLevel(const size_t level, const EGameMode eGameMode)
{
    // INITIALIZING LEVEL
    currentLevelIndex_ = level;
    auto pLevel = std::make_shared<Level>(ResourceManager::GetLevels()[currentLevelIndex_], eGameMode);
    pCurrentGameState_ = pLevel;
    Physics::PhysicsEngine::SetCurrentLevel(pLevel);
    UpdateViewport();
}

void Game::NextLevel(const EGameMode eGameMode)
{
    StartNewLevel(++currentLevelIndex_, eGameMode);
}

void Game::SetWindowSize(const glm::uvec2& windowSize)
{
    windowSize_ = windowSize;
    UpdateViewport();
}


