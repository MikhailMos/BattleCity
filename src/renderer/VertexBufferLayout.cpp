#include "VertexBufferLayout.h"

namespace RenderEngine {
	
	VertexBufferLayout::VertexBufferLayout()
		: stride_(0)
	{}

	void VertexBufferLayout::ReserveElements(const size_t count)
	{
		layoutElements_.reserve(count);
	}

	unsigned int VertexBufferLayout::GetStride() const
	{
		return stride_;
	}

	void VertexBufferLayout::AddElementLayoutFloat(const GLint count, const bool normalized)
	{
		layoutElements_.push_back({ count, GL_FLOAT, normalized, count * sizeof(GLfloat) });
		stride_ += layoutElements_.back().size;
	}

	const std::vector<VertexBufferLayoutElement>& VertexBufferLayout::GetlayoutElements() const
	{
		return layoutElements_;
	}

}