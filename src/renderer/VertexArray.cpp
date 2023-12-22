#include "VertexArray.h"

namespace RenderEngine {

	VertexArray::VertexArray()
	{
		glGenVertexArrays(1, &id_);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &id_);
	}

	VertexArray& VertexArray::operator=(VertexArray&& vertexArray) noexcept
	{
		id_ = vertexArray.id_;
		vertexArray.id_ = 0;

		return *this;
	}

	VertexArray::VertexArray(VertexArray&& vertexArray) noexcept
	{
		id_ = vertexArray.id_;
		vertexArray.id_ = 0;
	}

	void VertexArray::AddBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& layout) 
	{
		Bind();
		vertexBuffer.Bind();
		const auto& layoutElements = layout.GetlayoutElements();
		GLbyte* offset = nullptr;

		for (unsigned int i = 0; i < layoutElements.size(); ++i)
		{
			const auto& currentLayoutElement = layoutElements[i];
			GLuint currentAttribIndex = elementsCount_ + i;
			glEnableVertexAttribArray(currentAttribIndex);
			glVertexAttribPointer(currentAttribIndex, currentLayoutElement.count, currentLayoutElement.type, currentLayoutElement.normalized, layout.GetStride(), offset);
			offset += currentLayoutElement.size;
		}

		elementsCount_ += static_cast<unsigned int>(layoutElements.size());

	}

	void VertexArray::Bind() const
	{
		glBindVertexArray(id_);
	}

	void VertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

}