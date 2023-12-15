#include "Game.h"

#include "../resources/ResourceManager.h"
#include "../renderer/Texture2D.h"
#include "../renderer/ShaderProgram.h"
#include "../renderer/Sprite.h"
#include "../renderer/AnimatedSprite.h"

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
    ResourceManager::GetAnimatedSprite("NewAnimatedSprite")->Render();
}

void Game::Update(const uint64_t delta)
{
    ResourceManager::GetAnimatedSprite("NewAnimatedSprite")->Update(delta);
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

    return true;
}
