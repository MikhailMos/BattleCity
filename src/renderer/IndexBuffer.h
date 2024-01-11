#pragma once

#include "glad/glad.h"

namespace RenderEngine {

	class IndexBuffer
	{
	public:
		IndexBuffer();
		~IndexBuffer();

		IndexBuffer(const IndexBuffer&) = delete;
		IndexBuffer& operator=(const IndexBuffer&) = delete;

		IndexBuffer& operator=(IndexBuffer&& indexBuffer) noexcept;
		IndexBuffer(IndexBuffer&& indexBuffer) noexcept;

		void Init(const void* data, const unsigned int count);
		void Bind() const;
		void Unbind() const;
		unsigned int GetCount() const;

	private:
		GLuint id_;
		unsigned int count_;

	};


}