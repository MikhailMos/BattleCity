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

#include "GLFW/glfw3.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/mat4x4.hpp"

#include <iostream>

Game::Game(const glm::ivec2& windowSize)
	: m_eCurrentGameState_(EGameState::StartScreen)
    , m_windowSize_(windowSize)
{
	keys_.fill(false);
}

Game::~Game()
{
}

void Game::Render()
{
    switch (m_eCurrentGameState_)
    {
    case EGameState::StartScreen:
        pStartScreen_->Render();
        break;    
    case EGameState::Level:
        
        if (m_p_tank_)
        {
            m_p_tank_->Render();
        }

        if (m_p_level_)
        {
            m_p_level_->Render();
        }

        break;
    }
    
}

void Game::Update(const double delta)
{
    switch (m_eCurrentGameState_)
    {
    case Game::EGameState::StartScreen:
        if (keys_[GLFW_KEY_ENTER])
        {
            m_eCurrentGameState_ = EGameState::Level;
        }
        break;
    
    case Game::EGameState::Level:
        if (m_p_level_)
        {
            m_p_level_->Update(delta);
        }

        if (m_p_tank_)
        {
            if (keys_[GLFW_KEY_W])
            {
                m_p_tank_->SetOrientation(Tank::EOrientation::Top);
                m_p_tank_->SetVelocity(m_p_tank_->GetMaxVelocity());
            }
            else if (keys_[GLFW_KEY_A])
            {
                m_p_tank_->SetOrientation(Tank::EOrientation::Left);
                m_p_tank_->SetVelocity(m_p_tank_->GetMaxVelocity());
            }
            else if (keys_[GLFW_KEY_S])
            {
                m_p_tank_->SetOrientation(Tank::EOrientation::Bottom);
                m_p_tank_->SetVelocity(m_p_tank_->GetMaxVelocity());
            }
            else if (keys_[GLFW_KEY_D])
            {
                m_p_tank_->SetOrientation(Tank::EOrientation::Right);
                m_p_tank_->SetVelocity(m_p_tank_->GetMaxVelocity());
            }
            else
            {
                m_p_tank_->SetVelocity(0);
            }

            if (m_p_tank_ && keys_[GLFW_KEY_SPACE])
            {
                m_p_tank_->Fire();
            }

            m_p_tank_->Update(delta);
        }
        break;
    }
    
    
}

void Game::SetKey(const int key, const int action)
{
	keys_[key] = action;
}

bool Game::Init()
{
    ResourceManager::LoadJSONResources("resources/resources.json");
    
    auto pSpriteShaderProgram = ResourceManager::GetShaderProgram("spriteShader");
    if (!pSpriteShaderProgram)
    {
        std::cerr << "Can't find shader program: " << "spriteShader" << std::endl;
        return false;
    }

    pStartScreen_ = std::make_shared<StartScreen>(ResourceManager::GetStartScreen(), this);
    
    // INITIALIZING LEVEL
    m_p_level_ = std::make_shared<Level>(ResourceManager::GetLevels()[0]);
    
    m_windowSize_.x = static_cast<int>(m_p_level_->GetStateWidth());
    m_windowSize_.y = static_cast<int>(m_p_level_->GetStateHeight());

    Physics::PhysicsEngine::SetCurrentLevel(m_p_level_);

    glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(m_windowSize_.x), 0.f, static_cast<float>(m_windowSize_.y), -100.f, 100.f);

    pSpriteShaderProgram->Use();
    pSpriteShaderProgram->SetInt("tex", 0);
    pSpriteShaderProgram->SetMatrix4("projectionMat", projectionMatrix);

    // INITIALIZING TANK
    m_p_tank_ = std::make_shared<Tank>(0.05, m_p_level_->GetPlayerRespawn_1(), glm::vec2(Level::BLOCK_SIZE, Level::BLOCK_SIZE), 0.f);

    Physics::PhysicsEngine::AddDynamicGameObject(m_p_tank_);

    return true;
}

unsigned int Game::GetCurrentWidth() const
{
    switch (m_eCurrentGameState_)
    {
    case Game::EGameState::StartScreen:
        return pStartScreen_->GetStateWidth();
    
    case Game::EGameState::Level:
        return m_p_level_->GetStateWidth();
    
    }
    
}

unsigned int Game::GetCurrentHeight() const
{
    switch (m_eCurrentGameState_)
    {
    case Game::EGameState::StartScreen:
        return pStartScreen_->GetStateHeight();

    case Game::EGameState::Level:
        return m_p_level_->GetStateHeight();
    }
}

void Game::StartNewLevel(const size_t level, const EGameMode eGameMode)
{
   /* currentLevelIndex_ = level;
    auto pLevel = std::make_shared<Level>(ResourceManager::getLevels()[m_currentLevelIndex], eGameMode);
    m_eCurrentGameState_ = pLevel;
    Physics::PhysicsEngine::SetCurrentLevel(pLevel);
    UpdateViewport();*/
}
