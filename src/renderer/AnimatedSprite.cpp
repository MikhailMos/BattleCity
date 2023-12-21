#include "AnimatedSprite.h"

#include "Texture2D.h"

#include <iostream>


namespace Renderer {
	
	AnimatedSprite::AnimatedSprite(std::shared_ptr<Texture2D> pTexture, std::string& initialSubTexture, std::shared_ptr<ShaderProgram> pShaderProgram, const glm::vec2& position, const glm::vec2& size, const float rotation)
		: Sprite(std::move(pTexture), std::move(initialSubTexture), std::move(pShaderProgram), position, size, rotation)
	{
		p_current_animation_durations_ = states_map_.end();
	}
	
	void AnimatedSprite::InsertState(std::string state, std::vector<std::pair<std::string, uint64_t>> subTexturesDuration)
	{
		states_map_.emplace(std::move(state), std::move(subTexturesDuration));
	}

	void AnimatedSprite::Render() const
	{
		if (dirty_) 
		{
			auto subTexture = pTexture_->GetSubTexture(p_current_animation_durations_->second[current_frame_].first);

			const GLfloat textureCoords[] = {
				// U						V
				 subTexture.leftBottomUV.x, subTexture.leftBottomUV.y,
				 subTexture.leftBottomUV.x, subTexture.rightTopUV.y,
				 subTexture.rightTopUV.x,   subTexture.rightTopUV.y,
				 subTexture.rightTopUV.x,   subTexture.leftBottomUV.y,
			};

			textureCoordsBuffer_.Update(textureCoords, 2 * 4 * sizeof(GLfloat));

			dirty_ = false;
		}

		Sprite::Render();
	}

	void AnimatedSprite::Update(const uint64_t delta)
	{
		if (p_current_animation_durations_ != states_map_.end()) 
		{
			current_animation_time_ += delta;

			while (current_animation_time_ >= p_current_animation_durations_->second[current_frame_].second)
			{
				current_animation_time_ -= p_current_animation_durations_->second[current_frame_].second;
				++current_frame_;
				dirty_ = true;

				if (current_frame_ == p_current_animation_durations_->second.size()) {
					current_frame_ = 0;
				}
			}
		}
	}

	void AnimatedSprite::SetState(const std::string& newState)
	{
		auto it = states_map_.find(newState);
		if (it == states_map_.end()) 
		{
			std::cerr << "Can't find animation state: " << newState << std::endl;
			return;
		}

		if (it != p_current_animation_durations_) {
			current_animation_time_ = 0;
			current_frame_ = 0;
			p_current_animation_durations_ = it;
			dirty_ = true;
		}
		
	}

}