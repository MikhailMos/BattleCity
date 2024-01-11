#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "glad/glad.h"

namespace RenderEngine {

	class VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		VertexArray(const VertexArray&) = delete;
		VertexArray& operator=(const VertexArray&) = delete;

		VertexArray& operator=(VertexArray&& vertexArray) noexcept;
		VertexArray(VertexArray&& vertexArray) noexcept;

		void AddBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& layout);
		void Bind() const;
		void Unbind() const;

	private:
		GLuint id_ = 0;
		unsigned int elementsCount_ = 0;
	};


}