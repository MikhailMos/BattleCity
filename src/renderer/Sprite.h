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

		struct FrameDescription {
			
			FrameDescription(const glm::vec2 _leftBottomUV, const glm::vec2 _rightTopUV, const uint64_t _duration)
				: leftBottomUV(_leftBottomUV)
				, rightTopUV(_rightTopUV)
				, duration(_duration)
			{}
			
			glm::vec2 leftBottomUV;
			glm::vec2 rightTopUV;
			uint64_t duration;
		};

		Sprite(std::shared_ptr<Texture2D> pTexture,
			   std::string& initialSubTexture,
			   std::shared_ptr<ShaderProgram> pShaderProgram);

		~Sprite();

		Sprite(const Sprite&) = delete;
		Sprite& operator=(const Sprite&) = delete;

		void Render(const glm::vec2& position, const glm::vec2& size, const float rotation, const size_t frameId = 0) const;

		uint64_t GetFrameDuration(const size_t frameId) const;
		size_t GetFramesCount() const;
		void InsertFrames(std::vector<FrameDescription> framesDescriptions);

	protected:
		std::shared_ptr<Texture2D> pTexture_;
		std::shared_ptr<ShaderProgram> pShaderProgram_;
		
		VertexArray vertexArray_;
		VertexBuffer vertexCoordsBuffer_;
		VertexBuffer textureCoordsBuffer_;
		IndexBuffer indexBuffer_;

		std::vector<FrameDescription> framesDescriptions_;
		mutable size_t last_frameId_;
	};

}