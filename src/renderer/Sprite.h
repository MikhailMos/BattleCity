#pragma once

#include "glad/glad.h"
#include "glm/vec2.hpp"

#include <memory>

namespace Renderer {

	class Texture2D;
	class ShaderProgram;

	class Sprite
	{
	public:
		Sprite(const std::shared_ptr<Texture2D> pTexture, 
			   const std::shared_ptr<ShaderProgram> pShaderProgram, 
			   const glm::vec2& position = glm::vec2(0.f), 
			   const glm::vec2& size = glm::vec2(1.f),
			   const float rotation = 0.f);

		~Sprite();

		Sprite(const Sprite&) = delete;
		Sprite& operator=(const Sprite&) = delete;

		void Render() const;
		void SetPosition(const glm::vec2& position);
		void SetSize(const glm::vec2 & size);
		void SetRotation(const float rotation);

	private:
		std::shared_ptr<Texture2D> pTexture_;
		std::shared_ptr<ShaderProgram> pShaderProgram_;
		glm::vec2 position_;
		glm::vec2 size_;
		float rotation_;

	};

}