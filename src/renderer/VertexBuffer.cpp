#include "VertexBuffer.h"

namespace RenderEngine {

	VertexBuffer::VertexBuffer()
		: id_(0)
	{
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &id_);
	}

	VertexBuffer& VertexBuffer::operator=(VertexBuffer&& vertexBuffer) noexcept
	{
		id_ = vertexBuffer.id_;
		vertexBuffer.id_ = 0;

		return *this;
	}

	VertexBuffer::VertexBuffer(VertexBuffer&& vertexBuffer) noexcept
	{
		id_ = vertexBuffer.id_;
		vertexBuffer.id_ = 0;
	}

	void VertexBuffer::Init(const void* data, const unsigned int size)
	{
		glGenBuffers(1, &id_);
		glBindBuffer(GL_ARRAY_BUFFER, id_);
		glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	}

	void VertexBuffer::Update(const void* data, const unsigned int size) const
	{
		// для передачи данных, привязываем буфер
		glBindBuffer(GL_ARRAY_BUFFER, id_);
		// обновляем данные
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	void VertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, id_);
	}

	void VertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

}