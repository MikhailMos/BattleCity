#pragma once

#include "glad/glad.h"

namespace Renderer {

	class VertexBuffer
	{
	public:
		VertexBuffer();
		~VertexBuffer();

		VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer& operator=(const VertexBuffer&) = delete;

		VertexBuffer& operator=(VertexBuffer&& vertexBuffer) noexcept;
		VertexBuffer(VertexBuffer&& vertexBuffer) noexcept;

		void Init(const void* data, const unsigned int size);
		void Update(const void* data, const unsigned int size) const;
		void Bind() const;
		void Unbind() const;

	private:
		GLuint id_;

	};


}