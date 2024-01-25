#pragma once

#include <glm/vec2.hpp>
#include <memory>
#include <string>

#include "iGameObject.h"
#include "../../renderer/SpriteAnimator.h"
#include "../../game_system/Timer.h"

namespace RenderEngine {
	class Sprite;
}

class Bullet;

class Tank : public IGameObject {
public:
    enum class ETankType : uint8_t
    {
        Player1Yellow_type1,
        Player1Yellow_type2,
        Player1Yellow_type3,
        Player1Yellow_type4,

        Player2Green_type1,
        Player2Green_type2,
        Player2Green_type3,
        Player2Green_type4,

        EnemyWhite_type1,
        EnemyWhite_type2,
        EnemyWhite_type3,
        EnemyWhite_type4,

        EnemyGreen_type1,
        EnemyGreen_type2,
        EnemyGreen_type3,
        EnemyGreen_type4,

        EnemyRed_type1,
        EnemyRed_type2,
        EnemyRed_type3,
        EnemyRed_type4
    };

    inline static std::string TankTypeToSpriteString[] = {
        "player1_yellow_tank_type1_sprite",
        "player1_yellow_tank_type2_sprite",
        "player1_yellow_tank_type3_sprite",
        "player1_yellow_tank_type4_sprite",

        "player2_green_tank_type1_sprite",
        "player2_green_tank_type2_sprite",
        "player2_green_tank_type3_sprite",
        "player2_green_tank_type4_sprite",

        "enemy_white_tank_type1_sprite",
        "enemy_white_tank_type2_sprite",
        "enemy_white_tank_type3_sprite",
        "enemy_white_tank_type4_sprite",

        "enemy_green_tank_type1_sprite",
        "enemy_green_tank_type2_sprite",
        "enemy_green_tank_type3_sprite",
        "enemy_green_tank_type4_sprite",

        "enemy_red_tank_type1_sprite",
        "enemy_red_tank_type2_sprite",
        "enemy_red_tank_type3_sprite",
        "enemy_red_tank_type4_sprite"
    };
	
	enum class EOrientation {
		Top,
		Bottom,
		Left,
		Right
	};

	Tank(const Tank::ETankType eType,
        const double maxVelocity,
		const glm::vec2& position, 
		const glm::vec2& size,
		const float layer);

	void Render() const override;
	void SetOrientation(const EOrientation eOrientation);
	void Update(const double delta) override;
	double GetMaxVelocity() const;
	void SetVelocity(const double velocity) override;
	void Fire();

private:
	EOrientation e_orientation_;
	std::shared_ptr<Bullet> pCurrentBullet_;
	std::shared_ptr<RenderEngine::Sprite> pSprite_top_;
	std::shared_ptr<RenderEngine::Sprite> pSprite_bottom_;
	std::shared_ptr<RenderEngine::Sprite> pSprite_left_;
	std::shared_ptr<RenderEngine::Sprite> pSprite_right_;
	RenderEngine::SpriteAnimator spriteAnimator_top_;
	RenderEngine::SpriteAnimator spriteAnimator_bottom_;
	RenderEngine::SpriteAnimator spriteAnimator_left_;
	RenderEngine::SpriteAnimator spriteAnimator_right_;

	std::shared_ptr<RenderEngine::Sprite> pSprite_respawn_;
	RenderEngine::SpriteAnimator spriteAnimator_respawn_;

	std::shared_ptr<RenderEngine::Sprite> pSprite_shield_;
	RenderEngine::SpriteAnimator spriteAnimator_shield_;
	
	Timer respawnTimer_;
	Timer shieldTimer_;

	double maxVelocity_; // скорость танка
	bool isSpawning_;
	bool hasShield_;

    //std::unique_ptr<AIComponent> pAIComponent_;

    static const std::string& GetTankSpriteFromType(const ETankType eType);
};