#include "StartScreen.h"

#include <iostream>
#include <array>

#include "../../Resources/ResourceManager.h"
#include "../../Renderer/Sprite.h"
#include "../Game.h"

#include <GLFW/glfw3.h>

std::shared_ptr<RenderEngine::Sprite> GetSpriteForDescription(const char description)
{
    switch (description)
    {
    case '0':
        return ResourceManager::GetSprite("brickWall_All");
    case '1':
        return ResourceManager::GetSprite("brickWall_TopLeft");
    case '2':
        return ResourceManager::GetSprite("brickWall_TopRight");
    case '3':
        return ResourceManager::GetSprite("brickWall_Top");
    case '4':
        return ResourceManager::GetSprite("brickWall_BottomLeft");
    case '5':
        return ResourceManager::GetSprite("brickWall_Left");
    case '6':
        return ResourceManager::GetSprite("brickWall_TopRight_BottomLeft");
    case '7':
        return ResourceManager::GetSprite("brickWall_Top_BottomLeft");
    case '8':
        return ResourceManager::GetSprite("brickWall_BottomRight");
    case '9':
        return ResourceManager::GetSprite("brickWall_TopLeft_BottomRight");
    case 'A':
        return ResourceManager::GetSprite("brickWall_Right");
    case 'B':
        return ResourceManager::GetSprite("brickWall_Top_BottomRight");
    case 'C':
        return ResourceManager::GetSprite("brickWall_Bottom");
    case 'D':
        return ResourceManager::GetSprite("brickWall_TopLeft_Bottom");
    case 'E':
        return ResourceManager::GetSprite("brickWall_TopRight_Bottom");
    case 'F':
        return nullptr;
    default:
        std::cerr << "Unknown Start Level description: " << description << std::endl;
    }
    return nullptr;
}

StartScreen::StartScreen(const std::vector<std::string>& startScreenDescription, Game* pGame)
    : pGame_(pGame)
    , currentMenuSelection_(0)
    , keyReleased_(true)
    , menuSprite_(std::make_pair(ResourceManager::GetSprite("menu"), glm::vec2(11 * BLOCK_SIZE, STARTSCREEN_HEIGHT - startScreenDescription.size() * BLOCK_SIZE - MENU_HEIGHT - 5 * BLOCK_SIZE)))
    , tankSprite_(std::make_pair(ResourceManager::GetSprite("player1_yellow_tank_type1_sprite_right"), glm::vec2(8 * BLOCK_SIZE, menuSprite_.second.y + 6 * BLOCK_SIZE - currentMenuSelection_ * 2 * BLOCK_SIZE)))
    , tankSpriteAnimator_(tankSprite_.first)
{
    if (startScreenDescription.empty())
    {
        std::cerr << "Empty start screen description!" << std::endl;
    }

    auto leftOffsetPixels = 4 * BLOCK_SIZE;
    auto bottomOffset  = STARTSCREEN_HEIGHT - 4 * BLOCK_SIZE;

    unsigned int currentBottomOffset = bottomOffset;
    for (const std::string& currentRow : startScreenDescription)
    {
        unsigned int currentLeftOffset = leftOffsetPixels;
        for (const char currentElement : currentRow)
        {
            sprites_.emplace_back(std::make_pair(GetSpriteForDescription(currentElement), glm::vec2(currentLeftOffset, currentBottomOffset)));
            currentLeftOffset += BLOCK_SIZE;
        }
        currentBottomOffset -= BLOCK_SIZE;
    }
}

unsigned int StartScreen::GetStateWidth() const
{
    return STARTSCREEN_WIDTH;
}

unsigned int StartScreen::GetStateHeight() const
{
    return STARTSCREEN_HEIGHT;
}

void StartScreen::Render() const
{
    for (const auto& current : sprites_)
    {
        if (current.first)
        {
            current.first->Render(current.second, glm::vec2(BLOCK_SIZE), 0.f);
        }
    }
    //menuSprite_.first->Render(menuSprite_.second, glm::vec2(MENU_WIDTH, MENU_HEIGHT), 0.f);
    //tankSprite_.first->Render(glm::vec2(tankSprite_.second.x, tankSprite_.second.y - currentMenuSelection_ * 2 * BLOCK_SIZE), glm::vec2(TANK_SIZE), 0.f, 0.f, tankSpriteAnimator_.GetCurrentFrame());
}

void StartScreen::Update(const double delta)
{
    tankSpriteAnimator_.Update(delta);
}

void StartScreen::ProcessInput(const std::array<bool, 349>& keys)
{
    if (!keys[GLFW_KEY_W] && !keys[GLFW_KEY_S])
    {
        keyReleased_ = true;
    }

    if (keyReleased_)
    {
        if (keys[GLFW_KEY_W])
        {
            keyReleased_ = false;
            --currentMenuSelection_;
            if (currentMenuSelection_ < 0)
            {
                currentMenuSelection_ = 2;
            }
        }
        else if (keys[GLFW_KEY_S])
        {
            keyReleased_ = false;
            ++currentMenuSelection_;
            if (currentMenuSelection_ > 2)
            {
                currentMenuSelection_ = 0;
            }
        }
    }

    if (keys[GLFW_KEY_ENTER])
    {
        switch (currentMenuSelection_)
        {
        case 0:
            pGame_->StartNewLevel(0, Game::EGameMode::OnePlayer);
            break;
        case 1:
            pGame_->StartNewLevel(0, Game::EGameMode::TwoPlayers);
            break;
        default:
            break;
        }
    }
}