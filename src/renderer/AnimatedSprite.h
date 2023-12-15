#pragma once

#include "Sprite.h"

#include <map>
#include <vector>

namespace Renderer {

	class AnimatedSprite : public Sprite
	{
	public:
		AnimatedSprite(std::shared_ptr<Texture2D> pTexture,
					   std::string& initialSubTexture,
					   std::shared_ptr<ShaderProgram> pShaderProgram, 
					   const glm::vec2& position = glm::vec2(0.f), 
					   const glm::vec2& size = glm::vec2(1.f),
					   const float rotation = 0.f);
		
		void InsertState(std::string state, std::vector<std::pair<std::string, uint64_t>> subTexturesDuration);
		void Render() const override;
		void Update(const uint64_t delta);
		void SetState(const std::string& newState);

	private:
		std::map<std::string, std::vector<std::pair<std::string, uint64_t>>> states_map_;
		size_t current_frame_ = 0;
		uint64_t current_animation_time_ = 0;
		std::map<std::string, std::vector<std::pair<std::string, uint64_t>>>::const_iterator p_current_animation_durations_;
		mutable bool dirty_ = false;
	};

}