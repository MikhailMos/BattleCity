#pragma once

#include "glad/glad.h"

namespace Renderer {

	class IndexBuffer
	{
	public:
		IndexBuffer();
		~IndexBuffer();

		IndexBuffer(const IndexBuffer&) = delete;
		IndexBuffer& operator=(const IndexBuffer&) = delete;

		IndexBuffer& operator=(IndexBuffer&& indexBuffer) noexcept;
		IndexBuffer(IndexBuffer&& indexBuffer) noexcept;

		void Init(const void* data, const unsigned int size);
		void Bind() const;
		void Unbind() const;

	private:
		GLuint id_;

	};


}