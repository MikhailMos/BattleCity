#include "Renderer.h"

namespace RenderEngine {
	
	void Renderer::Draw(const VertexArray& vertexArray, const IndexBuffer& indexBuffer, const ShaderProgram& shader)
	{
		shader.Use();
		vertexArray.Bind();
		indexBuffer.Bind();

		glDrawElements(GL_TRIANGLES, indexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr);
	}

	void Renderer::SetClearColor(float red, float green, float blue, float alpha)
	{
		glClearColor(red, green, blue, alpha);
	}

	void Renderer::SetDepthTest(const bool enable)
	{
		if (enable) 
		{
			glEnable(GL_DEPTH_TEST);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}
	}

	void Renderer::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::SetViewport(unsigned int widht, unsigned int height, unsigned int leftOffset, unsigned int bottomOffset)
	{
		glViewport(leftOffset, bottomOffset, widht, height);
	}

	std::string Renderer::GetRendererStr()
	{
		return (char*)glGetString(GL_RENDERER);
	}

	std::string Renderer::GetVersionStr()
	{
		return (char*)glGetString(GL_VERSION);
	}

	std::string Renderer::GetVendorStr()
	{
		return (char*)glGetString(GL_VENDOR);
	}

}