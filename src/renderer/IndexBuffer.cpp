#include "IndexBuffer.h"

namespace Renderer {

	IndexBuffer::IndexBuffer()
		: id_(0)
	{
	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &id_);
	}

	IndexBuffer& IndexBuffer::operator=(IndexBuffer&& indexBuffer) noexcept
	{
		id_ = indexBuffer.id_;
		indexBuffer.id_ = 0;

		return *this;
	}

	IndexBuffer::IndexBuffer(IndexBuffer&& indexBuffer) noexcept
	{
		id_ = indexBuffer.id_;
		indexBuffer.id_ = 0;
	}

	void IndexBuffer::Init(const void* data, const unsigned int size)
	{
		glGenBuffers(1, &id_);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	void IndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
	}

	void IndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

}