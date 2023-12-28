#include "Game.h"

#include "../resources/ResourceManager.h"
#include "../renderer/Texture2D.h"
#include "../renderer/ShaderProgram.h"
#include "../renderer/Sprite.h"
#include "../renderer/AnimatedSprite.h"

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
    
    auto pTextureAtlas = ResourceManager::GetTexture("mapTextureAtlas");
    if (!pTextureAtlas)
    {
        std::cerr << "Can't find texture atlas: " << "mapTextureAtlas" << std::endl;
        return false;
    }
    
    auto pTanksTextureAtlas = ResourceManager::GetTexture("tanksTextureAtlas");
    if (!pTanksTextureAtlas)
    {
        std::cerr << "Can't find texture atlas: " << "tanksTextureAtlas" << std::endl;
        return false;
    }
   
    glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(m_windowSize_.x), 0.f, static_cast<float>(m_windowSize_.y), -100.f, 100.f);

    pSpriteShaderProgram->Use();
    pSpriteShaderProgram->SetInt("tex", 0);
    pSpriteShaderProgram->SetMatrix4("projectionMat", projectionMatrix);

    // INITIALIZING TANK
    auto pTanksAnimatedSprite = ResourceManager::GetAnimatedSprite("tankAnimatedSprite");
    if (!pTanksAnimatedSprite)
    {
        std::cerr << "Can't find animated sprite: " << "tankAnimatedSprite" << std::endl;
        return false;
    }

    m_p_tank_ = std::make_unique<Tank>(pTanksAnimatedSprite, 0.0000001f, glm::vec2(0), glm::vec2(16.f, 16.f));

    // INITIALIZING LEVEL
    m_p_level_ = std::make_unique<Level>(ResourceManager::GetLevels()[0]);

    return true;
}
