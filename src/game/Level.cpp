#include "Level.h"

#include <iostream>
#include <algorithm>
#include <cmath>

#include "GameObjects/BrickWall.h"
#include "GameObjects/BetonWall.h"
#include "GameObjects/Trees.h"
#include "GameObjects/Ice.h"
#include "GameObjects/Water.h"
#include "GameObjects/Eagle.h"
#include "GameObjects/Border.h"


std::shared_ptr<IGameObjcect> CreateGameObjectFromDescription(const char description, const glm::vec2& position, const glm::vec2& size, const float rotation)
{
	switch (description)
	{
    case '0':
        return std::make_shared<BrickWall>(BrickWall::EBrickWallType::Right, position, size, rotation, 0.f);
    case '1':
        return std::make_shared<BrickWall>(BrickWall::EBrickWallType::Bottom, position, size, rotation, 0.f);
    case '2':
        return std::make_shared<BrickWall>(BrickWall::EBrickWallType::Left, position, size, rotation, 0.f);
    case '3':
        return std::make_shared<BrickWall>(BrickWall::EBrickWallType::Top, position, size, rotation, 0.f);
    case '4':
        return std::make_shared<BrickWall>(BrickWall::EBrickWallType::All, position, size, rotation, 0.f);
    case 'G':
        return std::make_shared<BrickWall>(BrickWall::EBrickWallType::BottomLeft, position, size, rotation, 0.f);
    case 'H':
        return std::make_shared<BrickWall>(BrickWall::EBrickWallType::BottomRight, position, size, rotation, 0.f);
    case 'I':
        return std::make_shared<BrickWall>(BrickWall::EBrickWallType::TopLeft, position, size, rotation, 0.f);
    case 'J':
        return std::make_shared<BrickWall>(BrickWall::EBrickWallType::TopRight, position, size, rotation, 0.f);
    case '5':
        return std::make_shared<BetonWall>(BetonWall::EBetonWallType::Right, position, size, rotation, 0.f);
    case '6':
        return std::make_shared<BetonWall>(BetonWall::EBetonWallType::Bottom, position, size, rotation, 0.f);
    case '7':
        return std::make_shared<BetonWall>(BetonWall::EBetonWallType::Left, position, size, rotation, 0.f);
    case '8':
        return std::make_shared<BetonWall>(BetonWall::EBetonWallType::Top, position, size, rotation, 0.f);
    case '9':
        return std::make_shared<BetonWall>(BetonWall::EBetonWallType::All, position, size, rotation, 0.f);
    case 'A':
        return std::make_shared<Water>(position, size, rotation, 0.f);
    case 'B':
        return std::make_shared<Trees>(position, size, rotation, 1.f);
    case 'C':
        return std::make_shared<Ice>(position, size, rotation, -1.f);
    case 'E':
        return std::make_shared<Eagle>(position, size, rotation, 0.f);
    case 'D':
        return nullptr;
    default:
        std::cerr << "Unknown GameObject description: " << description << std::endl;
	}

    return nullptr;
}

Level::Level(const std::vector<std::string>& levelDescription)
{
	if (levelDescription.empty())
	{
		std::cerr << "Empty level description!" << std::endl;
	}

	width_ = levelDescription[0].length();
	height_ = levelDescription.size();

    playerRespawn_1_ = { BLOCK_SIZE * (width_ / 2 - 1), BLOCK_SIZE / 2 };
    playerRespawn_1_ = { BLOCK_SIZE * (width_ / 2 + 3), BLOCK_SIZE / 2 };
    enemyRespawn_1_  = { BLOCK_SIZE,                      (height_ * BLOCK_SIZE) - BLOCK_SIZE / 2 };
    enemyRespawn_2_  = { BLOCK_SIZE * (width_ / 2 + 1), (height_ * BLOCK_SIZE) - BLOCK_SIZE / 2 };
    enemyRespawn_3_  = { BLOCK_SIZE * width_,             (height_ * BLOCK_SIZE) - BLOCK_SIZE / 2 };

	mapObjects_.reserve(width_ * height_ + 4); // 4 - границы
	unsigned int currentBottomOffset = static_cast<unsigned int>(BLOCK_SIZE * (height_ - 1) + BLOCK_SIZE / 2.f);

	for (const std::string& currentRow : levelDescription)
	{
		unsigned int currentLeftOffset = BLOCK_SIZE;
		// проходим по каждой букве строки
		for (const char currentElement : currentRow)
		{
            switch (currentElement)
            {
            case 'K':
                playerRespawn_1_ = { currentLeftOffset, currentBottomOffset };
                break;
            case 'L':
                playerRespawn_2_ = { currentLeftOffset, currentBottomOffset };
                break;
            case 'M':
                enemyRespawn_1_ = { currentLeftOffset, currentBottomOffset };
                break;
            case 'N':
                enemyRespawn_2_ = { currentLeftOffset, currentBottomOffset };
                break;
            case 'O':
                enemyRespawn_3_ = { currentLeftOffset, currentBottomOffset };
                break;
            default:
                mapObjects_.emplace_back(CreateGameObjectFromDescription(currentElement, glm::vec2(currentLeftOffset, currentBottomOffset), glm::vec2(BLOCK_SIZE, BLOCK_SIZE), 0.f));
                break;
            }
            
            
			currentLeftOffset += BLOCK_SIZE;
		}
		currentBottomOffset -= BLOCK_SIZE;
	}

    // bottom border
    mapObjects_.emplace_back(std::make_shared<Border>(glm::vec2(BLOCK_SIZE, 0.f), glm::vec2(width_ * BLOCK_SIZE, BLOCK_SIZE / 2.f), 0.f, 0.f));

    // top border
    mapObjects_.emplace_back(std::make_shared<Border>(glm::vec2(BLOCK_SIZE, height_ * BLOCK_SIZE + BLOCK_SIZE / 2.f), glm::vec2(width_ * BLOCK_SIZE, BLOCK_SIZE / 2.f), 0.f, 0.f));

    // left border
    mapObjects_.emplace_back(std::make_shared<Border>(glm::vec2(0.f, 0.f), glm::vec2(BLOCK_SIZE, (height_ + 1) * BLOCK_SIZE), 0.f, 0.f));

    // right border
    mapObjects_.emplace_back(std::make_shared<Border>(glm::vec2((width_ + 1) * BLOCK_SIZE, 0.f), glm::vec2(BLOCK_SIZE * 2.f, (height_ + 1) * BLOCK_SIZE), 0.f, 0.f));
}

void Level::Render() const
{
	for (const auto& currentMapOpbject : mapObjects_)
	{
		if (currentMapOpbject)
		{
			currentMapOpbject->Render();
		}
	}
}

void Level::Update(const uint64_t delta)
{
	for (const auto& currentMapOpbject : mapObjects_)
	{
		if (currentMapOpbject)
		{
			currentMapOpbject->Update(delta);
		}
	}
}

size_t Level::GetLevelWidth() const
{
    return (width_ + 3) * BLOCK_SIZE;
}

size_t Level::GetLevelHeight() const
{
    return (height_ + 1) * BLOCK_SIZE;
}

const glm::ivec2& Level::GetPlayerRespawn_1() const
{
    return playerRespawn_1_;
}

const glm::ivec2& Level::GetPlayerRespawn_2() const
{
    return playerRespawn_2_;
}

const glm::ivec2& Level::GetEnemyRespawn_1() const
{
    return enemyRespawn_1_;
}

const glm::ivec2& Level::GetEnemyRespawn_2() const
{
    return enemyRespawn_2_;
}

const glm::ivec2& Level::GetEnemyRespawn_3() const
{
    return enemyRespawn_3_;
}
