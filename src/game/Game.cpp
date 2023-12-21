#include "Game.h"

#include "../resources/ResourceManager.h"
#include "../renderer/Texture2D.h"
#include "../renderer/ShaderProgram.h"
#include "../renderer/Sprite.h"
#include "../renderer/AnimatedSprite.h"

#include "Tank.h"

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
    //ResourceManager::GetAnimatedSprite("NewAnimatedSprite")->Render();
    if (m_p_tank_)
    {
        m_p_tank_->Render();
    }
}

void Game::Update(const uint64_t delta)
{
    //ResourceManager::GetAnimatedSprite("NewAnimatedSprite")->Update(delta);
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
    auto pDefaultShaderProgram = ResourceManager::LoadShaders("DefaultShader", "resources/shaders/vertext.txt", "resources/shaders/fragment.txt");
    if (!pDefaultShaderProgram)
    {
        std::cerr << "Can't creatre shader program: " << "DefaultShader" << std::endl;
        return false;
    }

    auto pSpriteShaderProgram = ResourceManager::LoadShaders("SpriteShader", "resources/shaders/vSprite.txt", "resources/shaders/fSprite.txt");
    if (!pSpriteShaderProgram)
    {
        std::cerr << "Can't creatre sprite shader program: " << "SpriteShader" << std::endl;
        return false;
    }

    auto tex = ResourceManager::LoadTexture("DefaultTexture", "resources/textures/map_16x16.png");

    std::vector<std::string> subTexturesNames = {
        /*"block", "topBlock", "bottomBlock", "leftBlock", "rightBlock", "topLeftBlock", "topRigthBlock",
        "bottomLeftBlock", "bottomRightBlock", "beton",*/
        "eagle",                "deadEagle",             "respawn1",             "respawn2",              "respawn3",   "respawn4",   "shield1",    "shield2",
        "explosionTopLeftBig1", "explosionTopRightBig1", "explosionTopLeftBig2", "explosionTopRightBig2", "explosion1", "explosion2", "explosion3", "nothing"
    };
    auto pTextureAtlas = ResourceManager::LoadTextureAtlas("DefaultTextureAtlas", "resources/textures/map_16x16.png", std::move(subTexturesNames), 16, 16);

    auto pAnimatedSprite = ResourceManager::LoadAnimatedSprite("NewAnimatedSprite", "DefaultTextureAtlas", "SpriteShader", 100, 100, "eagle");
    pAnimatedSprite->SetPosition(glm::vec2(300, 300));

    std::vector<std::pair<std::string, uint64_t>> respawnState;
    respawnState.emplace_back(std::make_pair<std::string, uint64_t>("respawn1", 1000000000));
    respawnState.emplace_back(std::make_pair<std::string, uint64_t>("respawn2", 1000000000));
    respawnState.emplace_back(std::make_pair<std::string, uint64_t>("respawn3", 1000000000));
    respawnState.emplace_back(std::make_pair<std::string, uint64_t>("respawn4", 1000000000));

    std::vector<std::pair<std::string, uint64_t>> eagleState;
    eagleState.emplace_back(std::make_pair<std::string, uint64_t>("eagle", 1000000000));
    eagleState.emplace_back(std::make_pair<std::string, uint64_t>("deadEagle", 1000000000));

    pAnimatedSprite->InsertState("RespawnState", std::move(respawnState));
    pAnimatedSprite->InsertState("EagleState", std::move(eagleState));

    pAnimatedSprite->SetState("RespawnState");

    pDefaultShaderProgram->Use();
    pDefaultShaderProgram->SetInt("tex", 0);

    glm::mat4 modelMatrix1 = glm::mat4(1.f);
    modelMatrix1 = glm::translate(modelMatrix1, glm::vec3(50.f, 50.f, 0.f));

    glm::mat4 modelMatrix2 = glm::mat4(1.f);
    modelMatrix2 = glm::translate(modelMatrix2, glm::vec3(590.f, 50.f, 0.f));

    glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(m_windowSize_.x), 0.f, static_cast<float>(m_windowSize_.y), -100.f, 100.f);

    pDefaultShaderProgram->SetMatrix4("projectionMat", projectionMatrix);

    pSpriteShaderProgram->Use();
    pSpriteShaderProgram->SetInt("tex", 0);
    pSpriteShaderProgram->SetMatrix4("projectionMat", projectionMatrix);

    // INITIALIZING TANK
    std::vector<std::string> tanksSubTexturesNames = {
        "tankTop1",
        "tankTop2",
        "tankLeft1",
        "tankLeft2",
        "tankBottom1",
        "tankBottom2",
        "tankRight1",
        "tankRight2"
    };

    auto pTanksTextureAtlas = ResourceManager::LoadTextureAtlas("TanksTextureAtlas", "resources/textures/tanks.png", std::move(tanksSubTexturesNames), 16, 16);
    auto pTanksAnimatedSprite = ResourceManager::LoadAnimatedSprite("TanksAnimatedSprite", "TanksTextureAtlas", "SpriteShader", 100, 100, "tankTop1");
    //pTanksAnimatedSprite->SetPosition(glm::vec2(300, 300));

    std::vector<std::pair<std::string, uint64_t>> tankTopState;
    tankTopState.emplace_back(std::make_pair<std::string, uint64_t>("tankTop1", 500000000)); //0.5c
    tankTopState.emplace_back(std::make_pair<std::string, uint64_t>("tankTop2", 500000000));
    
    std::vector<std::pair<std::string, uint64_t>> tankLeftState;
    tankLeftState.emplace_back(std::make_pair<std::string, uint64_t>("tankLeft1", 500000000)); //0.5c
    tankLeftState.emplace_back(std::make_pair<std::string, uint64_t>("tankLeft2", 500000000));

    std::vector<std::pair<std::string, uint64_t>> tankBottomState;
    tankBottomState.emplace_back(std::make_pair<std::string, uint64_t>("tankBottom1", 500000000)); //0.5c
    tankBottomState.emplace_back(std::make_pair<std::string, uint64_t>("tankBottom2", 500000000));

    std::vector<std::pair<std::string, uint64_t>> tankRightState;
    tankRightState.emplace_back(std::make_pair<std::string, uint64_t>("tankRight1", 500000000)); //0.5c
    tankRightState.emplace_back(std::make_pair<std::string, uint64_t>("tankRight2", 500000000));

    pTanksAnimatedSprite->InsertState("tankTopState", std::move(tankTopState));
    pTanksAnimatedSprite->InsertState("tankLeftState", std::move(tankLeftState));
    pTanksAnimatedSprite->InsertState("tankBottomState", std::move(tankBottomState));
    pTanksAnimatedSprite->InsertState("tankRightState", std::move(tankRightState));

    pTanksAnimatedSprite->SetState("tankTopState");

    m_p_tank_ = std::make_unique<Tank>(pTanksAnimatedSprite, 0.0000001f, glm::vec2(100.f, 100.f));

    return true;
}
