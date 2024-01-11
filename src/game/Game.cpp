#include "Game.h"

#include "../resources/ResourceManager.h"
#include "../renderer/Texture2D.h"
#include "../renderer/ShaderProgram.h"
#include "../renderer/Sprite.h"

#include "GameObjects/Tank.h"
#include "Level.h"

#include "GLFW/glfw3.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/mat4x4.hpp"

#include <iostream>

Game::Game(const glm::ivec2& windowSize)
	: m_eCurrentGameState_(EGameState::Active)
    , m_windowSize_(windowSize)
{
	m_keys_.fill(false);
}

Game::~Game()
{
}

void Game::Render()
{
    if (m_p_tank_)
    {
        m_p_tank_->Render();
    }

    if (m_p_level_)
    {
        m_p_level_->Render();
    }
}

void Game::Update(const uint64_t delta)
{
    if (m_p_level_)
    {
        m_p_level_->Update(delta);
    }
    
    if (m_p_tank_) 
    {
        if (m_keys_[GLFW_KEY_W])
        {
            m_p_tank_->SetOrientation(Tank::EOrientation::Top);
            m_p_tank_->Move(true);
        }
        else if (m_keys_[GLFW_KEY_A])
        {
            m_p_tank_->SetOrientation(Tank::EOrientation::Left);
            m_p_tank_->Move(true);
        }
        else if (m_keys_[GLFW_KEY_S])
        {
            m_p_tank_->SetOrientation(Tank::EOrientation::Bottom);
            m_p_tank_->Move(true);
        }
        else if (m_keys_[GLFW_KEY_D])
        {
            m_p_tank_->SetOrientation(Tank::EOrientation::Right);
            m_p_tank_->Move(true);
        }
        else 
        {
            m_p_tank_->Move(false);
        }

        m_p_tank_->Update(delta);
    }
}

void Game::SetKey(const int key, const int action)
{
	m_keys_[key] = action;
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
    
    // INITIALIZING LEVEL
    m_p_level_ = std::make_unique<Level>(ResourceManager::GetLevels()[0]);
    
    m_windowSize_.x = static_cast<int>(m_p_level_->GetLevelWidth());
    m_windowSize_.y = static_cast<int>(m_p_level_->GetLevelHeight());

    glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(m_windowSize_.x), 0.f, static_cast<float>(m_windowSize_.y), -100.f, 100.f);

    pSpriteShaderProgram->Use();
    pSpriteShaderProgram->SetInt("tex", 0);
    pSpriteShaderProgram->SetMatrix4("projectionMat", projectionMatrix);

    // INITIALIZING TANK
    m_p_tank_ = std::make_unique<Tank>(0.0000001f, m_p_level_->GetPlayerRespawn_1(), glm::vec2(Level::BLOCK_SIZE, Level::BLOCK_SIZE), 0.f);

    

    return true;
}

size_t Game::GetCurrentLevelWidth() const
{
    return m_p_level_->GetLevelWidth();
}

size_t Game::GetCurrentLevelHeight() const
{
    return m_p_level_->GetLevelHeight();
}
