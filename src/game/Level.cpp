#include "Level.h"

#include <iostream>
#include <algorithm>
#include <cmath>

#include "GameObjects/BrickWall.h"

const unsigned int BLOCK_SIZE = 16;

std::shared_ptr<IGameObjcect> CreateGameObjectFromDescription(const char description, const glm::vec2& position, const glm::vec2& size, const float rotation)
{
	switch (description)
	{
    case '0':
        //return std::make_shared<BrickWall>(BrickWall::EBrickWallType::Right, position, size, rotation, 0.f);
        return std::make_shared<BrickWall>(ResourceManager::GetSprite("brickWall_Right"), position, size, rotation);
    case '1':
        //return std::make_shared<BrickWall>(BrickWall::EBrickWallType::Bottom, position, size, rotation, 0.f);
        return std::make_shared<BrickWall>(ResourceManager::GetSprite("brickWall_Bottom"), position, size, rotation);
    case '2':
        //return std::make_shared<BrickWall>(BrickWall::EBrickWallType::Left, position, size, rotation, 0.f);
        return std::make_shared<BrickWall>(ResourceManager::GetSprite("brickWall_Left"), position, size, rotation);
    case '3':
        //return std::make_shared<BrickWall>(BrickWall::EBrickWallType::Top, position, size, rotation, 0.f);
        return std::make_shared<BrickWall>(ResourceManager::GetSprite("brickWall_Top"), position, size, rotation);
    case '4':
        //return std::make_shared<BrickWall>(BrickWall::EBrickWallType::All, position, size, rotation, 0.f);
        return std::make_shared<BrickWall>(ResourceManager::GetSprite("brickWall_All"), position, size, rotation);
    case 'G':
        //return std::make_shared<BrickWall>(BrickWall::EBrickWallType::BottomLeft, position, size, rotation, 0.f);
        return std::make_shared<BrickWall>(ResourceManager::GetSprite("brickWall_BottomLeft"), position, size, rotation);
    case 'H':
        //return std::make_shared<BrickWall>(BrickWall::EBrickWallType::BottomRight, position, size, rotation, 0.f);
        return std::make_shared<BrickWall>(ResourceManager::GetSprite("brickWall_Right"), position, size, rotation);
    case 'I':
        //return std::make_shared<BrickWall>(BrickWall::EBrickWallType::TopLeft, position, size, rotation, 0.f);
        return std::make_shared<BrickWall>(ResourceManager::GetSprite("brickWall_TopLeft"), position, size, rotation);
    case 'J':
        //return std::make_shared<BrickWall>(BrickWall::EBrickWallType::TopRight, position, size, rotation, 0.f);
        return std::make_shared<BrickWall>(ResourceManager::GetSprite("brickWall_TopRight"), position, size, rotation);
   /* case '5':
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
        return std::make_shared<Eagle>(position, size, rotation, 0.f);*/
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

	mapObjects_.reserve(width_ * height_);
	unsigned int currentBottomOffset = static_cast<unsigned int>(BLOCK_SIZE * (height_ - 1));

	for (const std::string& currentRow : levelDescription)
	{
		unsigned int currentLeftOffset = 0;
		// проходим по каждой букве строки
		for (const char currentElement : currentRow)
		{
            mapObjects_.emplace_back(CreateGameObjectFromDescription(currentElement, glm::vec2(currentLeftOffset, currentBottomOffset), glm::vec2(BLOCK_SIZE, BLOCK_SIZE), 0.f));
			currentLeftOffset += BLOCK_SIZE;
		}
		currentBottomOffset -= BLOCK_SIZE;
	}
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
