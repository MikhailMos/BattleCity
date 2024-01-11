#pragma once

#include <vector>
#include "glad/glad.h"

namespace RenderEngine {

	struct VertexBufferLayoutElement {
		GLint count;
		GLenum type;
		GLboolean normalized;
		unsigned int size;
	};

	class VertexBufferLayout {
	public:
		VertexBufferLayout();

		void ReserveElements(const size_t count);
		unsigned int GetStride() const;
		void AddElementLayoutFloat(const GLint count, const bool normalized);
		const std::vector<VertexBufferLayoutElement>& GetlayoutElements() const;
	
	private:
		std::vector<VertexBufferLayoutElement> layoutElements_;
		unsigned int stride_;
	};

}