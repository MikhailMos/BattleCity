#pragma once

#include <glad/glad.h>
#include <string>

#include "glm/mat4x4.hpp"

namespace RenderEngine {

	class ShaderProgram {
	public:
		ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);
		~ShaderProgram();

		ShaderProgram(ShaderProgram&& shaderProgram) noexcept;
		ShaderProgram& operator=(ShaderProgram&& shaderProgram) noexcept;
		

		ShaderProgram() = delete;
		ShaderProgram(ShaderProgram&) = delete;
		ShaderProgram& operator=(const ShaderProgram&) = delete;

		bool IsCompiled() const;
		void Use() const;
		void SetInt(const std::string& name, const GLint value);
		void SetFoat(const std::string& name, const GLfloat value);
		void SetMatrix4(const std::string& name, const glm::mat4& matrix);

	private:
		bool isCompiled_ = false;
		GLuint ID_ = 0;

		bool CreateShader(const std::string& source, const GLenum shader_type, GLuint& shaderID);
	};

}

