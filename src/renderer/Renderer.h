#pragma once

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "ShaderProgram.h"

#include <string>

namespace RenderEngine {

	class Renderer {
	public:
		static void Draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const ShaderProgram& shader);
		static void SetClearColor(float red, float green, float blue, float alpha);
		static void SetDepthTest(const bool enable);	// Включает/выключает буфер глубины
		static void Clear();
		static void SetViewport(unsigned int widht, unsigned int height, unsigned int leftOffset = 0, unsigned int bottomOffset = 0);
		
		static std::string GetRendererStr();
		static std::string GetVersionStr();
		static std::string GetVendorStr();
	};



}