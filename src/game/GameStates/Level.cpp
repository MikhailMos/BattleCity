#include "Level.h"

#include <iostream>
#include <algorithm>
#include <cmath>
#include <algorithm>

#include "GLFW/glfw3.h"

#include "../GameObjects/BrickWall.h"
#include "../GameObjects/BetonWall.h"
#include "../GameObjects/Trees.h"
#include "../GameObjects/Ice.h"
#include "../GameObjects/Water.h"
#include "../GameObjects/Eagle.h"
#include "../GameObjects/Border.h"
#include "../GameObjects/Tank.h"


std::shared_ptr<IGameObject> CreateGameObjectFromDescription(const char description, const glm::vec2& position, const glm::vec2& size, const float rotation)
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

Level::Level(const std::vector<std::string>& levelDescription, const Game::EGameMode eGameMode)
    : eGameMode_(eGameMode)
{
	if (levelDescription.empty())
	{
		std::cerr << "Empty level description!" << std::endl;
	}

	widthBlocks_  = levelDescription[0].length();
	heightBlocks_ = levelDescription.size();
    widthPixels_  = static_cast<unsigned int>(widthBlocks_ * BLOCK_SIZE);
    heightPixels_ = static_cast<unsigned int>(heightBlocks_ * BLOCK_SIZE);

    playerRespawn_1_ = { BLOCK_SIZE * (widthBlocks_ / 2 - 1), BLOCK_SIZE / 2 };
    playerRespawn_2_ = { BLOCK_SIZE * (widthBlocks_ / 2 + 3), BLOCK_SIZE / 2 };
    enemyRespawn_1_  = { BLOCK_SIZE,                          (heightBlocks_ * BLOCK_SIZE) - BLOCK_SIZE / 2 };
    enemyRespawn_2_  = { BLOCK_SIZE * (widthBlocks_ / 2 + 1), (heightBlocks_ * BLOCK_SIZE) - BLOCK_SIZE / 2 };
    enemyRespawn_3_  = { BLOCK_SIZE * widthBlocks_,           (heightBlocks_ * BLOCK_SIZE) - BLOCK_SIZE / 2 };

	mapObjects_.reserve(widthBlocks_ * heightBlocks_ + 4); // 4 - границы
	unsigned int currentBottomOffset = static_cast<unsigned int>(BLOCK_SIZE * (heightBlocks_ - 1) + BLOCK_SIZE / 2.f);

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
                mapObjects_.emplace_back(nullptr);
                break;
            case 'L':
                playerRespawn_2_ = { currentLeftOffset, currentBottomOffset };
                mapObjects_.emplace_back(nullptr);
                break;
            case 'M':
                enemyRespawn_1_ = { currentLeftOffset, currentBottomOffset };
                mapObjects_.emplace_back(nullptr);
                break;
            case 'N':
                enemyRespawn_2_ = { currentLeftOffset, currentBottomOffset };
                mapObjects_.emplace_back(nullptr);
                break;
            case 'O':
                enemyRespawn_3_ = { currentLeftOffset, currentBottomOffset };
                mapObjects_.emplace_back(nullptr);
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
    mapObjects_.emplace_back(std::make_shared<Border>(glm::vec2(BLOCK_SIZE, 0.f), glm::vec2(widthBlocks_ * BLOCK_SIZE, BLOCK_SIZE / 2.f), 0.f, 0.f));

    // top border
    mapObjects_.emplace_back(std::make_shared<Border>(glm::vec2(BLOCK_SIZE, heightBlocks_ * BLOCK_SIZE + BLOCK_SIZE / 2.f), glm::vec2(widthBlocks_ * BLOCK_SIZE, BLOCK_SIZE / 2.f), 0.f, 0.f));

    // left border
    mapObjects_.emplace_back(std::make_shared<Border>(glm::vec2(0.f, 0.f), glm::vec2(BLOCK_SIZE, (heightBlocks_ + 1) * BLOCK_SIZE), 0.f, 0.f));

    // right border
    mapObjects_.emplace_back(std::make_shared<Border>(glm::vec2((widthBlocks_ + 1) * BLOCK_SIZE, 0.f), glm::vec2(BLOCK_SIZE * 2.f, (heightBlocks_ + 1) * BLOCK_SIZE), 0.f, 0.f));
}

void Level::InitLevel()
{
    // INITIALIZING TANK
    switch (eGameMode_)
    {
    case Game::EGameMode::TwoPlayers:
        pTank2_ = std::make_shared<Tank>(Tank::ETankType::Player2Green_type1, 0.05, GetPlayerRespawn_2(), glm::vec2(Level::BLOCK_SIZE, Level::BLOCK_SIZE), 0.f);
        Physics::PhysicsEngine::AddDynamicGameObject(pTank2_);
        [[fallthrough]];
    case Game::EGameMode::OnePlayer:
        pTank1_ = std::make_shared<Tank>(Tank::ETankType::Player1Yellow_type1, 0.05, GetPlayerRespawn_1(), glm::vec2(Level::BLOCK_SIZE, Level::BLOCK_SIZE), 0.f);
        Physics::PhysicsEngine::AddDynamicGameObject(pTank1_);
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
    
    switch (eGameMode_)
    {
    case Game::EGameMode::TwoPlayers:
        pTank2_->Render();
        [[fallthrough]];
    case Game::EGameMode::OnePlayer:
        pTank1_->Render();
    }

}

void Level::Update(const double delta)
{
	for (const auto& currentMapOpbject : mapObjects_)
	{
		if (currentMapOpbject)
		{
			currentMapOpbject->Update(delta);
		}
	}

    switch (eGameMode_)
    {
    case Game::EGameMode::TwoPlayers:
        pTank2_->Update(delta);
        [[fallthrough]];
    case Game::EGameMode::OnePlayer:
        pTank1_->Update(delta);
    }

}

unsigned int Level::GetStateWidth() const
{
    return static_cast<unsigned int>((widthBlocks_ + 3) * BLOCK_SIZE);
}

unsigned int Level::GetStateHeight() const
{
    return static_cast<unsigned int>((heightBlocks_ + 1) * BLOCK_SIZE);
}

void Level::ProcessInput(const std::array<bool, 349>& keys)
{
    switch (eGameMode_)
    {
    case Game::EGameMode::TwoPlayers:
        if (keys[GLFW_KEY_UP])
        {
            pTank2_->SetOrientation(Tank::EOrientation::Top);
            pTank2_->SetVelocity(pTank2_->GetMaxVelocity());
        }
        else if (keys[GLFW_KEY_LEFT])
        {
            pTank2_->SetOrientation(Tank::EOrientation::Left);
            pTank2_->SetVelocity(pTank2_->GetMaxVelocity());
        }
        else if (keys[GLFW_KEY_RIGHT])
        {
            pTank2_->SetOrientation(Tank::EOrientation::Right);
            pTank2_->SetVelocity(pTank2_->GetMaxVelocity());
        }
        else if (keys[GLFW_KEY_DOWN])
        {
            pTank2_->SetOrientation(Tank::EOrientation::Bottom);
            pTank2_->SetVelocity(pTank2_->GetMaxVelocity());
        }
        else
        {
            pTank2_->SetVelocity(0);
        }

        if (pTank2_ && keys[GLFW_KEY_RIGHT_SHIFT])
        {
            pTank2_->Fire();
        }
        [[fallthrough]];
    case Game::EGameMode::OnePlayer:
        
        if (keys[GLFW_KEY_W])
        {
            pTank1_->SetOrientation(Tank::EOrientation::Top);
            pTank1_->SetVelocity(pTank1_->GetMaxVelocity());
        }
        else if (keys[GLFW_KEY_A])
        {
            pTank1_->SetOrientation(Tank::EOrientation::Left);
            pTank1_->SetVelocity(pTank1_->GetMaxVelocity());
        }
        else if (keys[GLFW_KEY_S])
        {
            pTank1_->SetOrientation(Tank::EOrientation::Bottom);
            pTank1_->SetVelocity(pTank1_->GetMaxVelocity());
        }
        else if (keys[GLFW_KEY_D])
        {
            pTank1_->SetOrientation(Tank::EOrientation::Right);
            pTank1_->SetVelocity(pTank1_->GetMaxVelocity());
        }
        else
        {
            pTank1_->SetVelocity(0);
        }

        if (pTank1_ && keys[GLFW_KEY_SPACE])
        {
            pTank1_->Fire();
        }
    }
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

std::vector<std::shared_ptr<IGameObject>> Level::GetObjectsInArea(const glm::vec2& bottomLeft, glm::vec2& tomRight)
{
    std::vector<std::shared_ptr<IGameObject>> output;
    output.reserve(9);
    
    glm::vec2 bottomLeft_converted(std::clamp(bottomLeft.x - BLOCK_SIZE, 0.f, static_cast<float>(widthPixels_)), 
                                   std::clamp(heightPixels_ - bottomLeft.y + BLOCK_SIZE / 2, 0.f, static_cast<float>(heightPixels_)));
    glm::vec2 topRight_converted(std::clamp(tomRight.x - BLOCK_SIZE, 0.f, static_cast<float>(widthPixels_)),
                                 std::clamp(heightPixels_ - tomRight.y + BLOCK_SIZE / 2, 0.f, static_cast<float>(heightPixels_)));

    size_t startX = static_cast<size_t>(floor(bottomLeft_converted.x / BLOCK_SIZE));
    size_t endX   = static_cast<size_t>(ceil(topRight_converted.x    / BLOCK_SIZE));

    size_t startY = static_cast<size_t>(floor(topRight_converted.y  / BLOCK_SIZE));
    size_t endY   = static_cast<size_t>(ceil(bottomLeft_converted.y / BLOCK_SIZE));

    for (size_t currentColumn = startX; currentColumn < endX; ++currentColumn)
    {
        for (size_t currentRow = startY; currentRow < endY; ++currentRow)
        {
            auto& currentObject = mapObjects_[currentRow * widthBlocks_ + currentColumn];
            if (currentObject) 
            {
                output.push_back(currentObject);
            }
        }
    }
    // вставляем границы карты
    if (endX >= widthBlocks_)
    {
        output.push_back(mapObjects_[mapObjects_.size() - 1]);
    }

    if (startX <= 1)
    {
        output.push_back(mapObjects_[mapObjects_.size() - 2]);
    }

    if (startY <= 1)
    {
        output.push_back(mapObjects_[mapObjects_.size() - 3]);
    }

    if (endY >= heightBlocks_)
    {
        output.push_back(mapObjects_[mapObjects_.size() - 4]);
    }

    return output;
}


