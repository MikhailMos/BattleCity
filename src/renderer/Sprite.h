#pragma once

#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"

#include "glad/glad.h"
#include "glm/vec2.hpp"

#include <memory>
#include <string>

namespace RenderEngine {

	class Texture2D;
	class ShaderProgram;

	class Sprite
	{
	public:
		Sprite(std::shared_ptr<Texture2D> pTexture,
			   std::string& initialSubTexture,
			   std::shared_ptr<ShaderProgram> pShaderProgram);

		~Sprite();

		Sprite(const Sprite&) = delete;
		Sprite& operator=(const Sprite&) = delete;

		virtual void Render(const glm::vec2& position, const glm::vec2& size, const float rotation) const;

	protected:
		std::shared_ptr<Texture2D> pTexture_;
		std::shared_ptr<ShaderProgram> pShaderProgram_;
		
		VertexArray vertexArray_;
		VertexBuffer vertexCoordsBuffer_;
		VertexBuffer textureCoordsBuffer_;
		IndexBuffer indexBuffer_;
	};

}