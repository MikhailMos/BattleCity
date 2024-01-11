#include "ShaderProgram.h"

#include "glm/gtc/type_ptr.hpp"

#include <iostream>

namespace RenderEngine {
	ShaderProgram::ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader)
	{
		// создание вертексного шейдера
		GLuint vertexShaderID;
		if (!CreateShader(vertexShader, GL_VERTEX_SHADER, vertexShaderID)) {
			std::cerr << "VERTEX SHADER compile time error" << std::endl;
			return;
		}

		// создание фрагментного шейдера
		GLuint fragmentShaderID;
		if (!CreateShader(fragmentShader, GL_FRAGMENT_SHADER, fragmentShaderID)) {
			std::cerr << "FRAGMENT SHADER compile time error" << std::endl;
			glDeleteShader(vertexShaderID);
			return;
		}

		// создание ShaderProgram и линковка
		m_ID = glCreateProgram();
		glAttachShader(m_ID, vertexShaderID);
		glAttachShader(m_ID, fragmentShaderID);
		glLinkProgram(m_ID);

		GLint success;
		glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
		if (!success) {
			GLchar infolog[1024];
			glGetShaderInfoLog(m_ID, 1024, nullptr, infolog);
			std::cerr << "ERROR::SHADER: Link time error:\n" << infolog << std::endl;
			
		}
		else 
		{
			m_isCompiled = true;
		}
		
		// удаляем шейдеры, т.к. они уже есть в ShaderProgram
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
	}

	ShaderProgram::~ShaderProgram()
	{
		glDeleteProgram(m_ID);
	}

	ShaderProgram::ShaderProgram(ShaderProgram&& shaderProgram) noexcept
	{
		m_ID = shaderProgram.m_ID;
		m_isCompiled = shaderProgram.m_isCompiled;

		shaderProgram.m_ID = 0;
		shaderProgram.m_isCompiled = false;
	}

	ShaderProgram& ShaderProgram::operator=(ShaderProgram&& shaderProgram) noexcept
	{
		glDeleteProgram(m_ID);
		m_ID = shaderProgram.m_ID;
		m_isCompiled = shaderProgram.m_isCompiled;

		shaderProgram.m_ID = 0;
		shaderProgram.m_isCompiled = false;

		return *this;
	}

	bool ShaderProgram::IsCompiled() const
	{
		return m_isCompiled;
	}

	void ShaderProgram::Use() const
	{
		glUseProgram(m_ID);
	}

	void ShaderProgram::SetInt(const std::string& name, const GLint value)
	{
		glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
	}

	void ShaderProgram::SetFoat(const std::string& name, const GLfloat value)
	{
		glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
	}

	void ShaderProgram::SetMatrix4(const std::string& name, const glm::mat4& matrix)
	{
		glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
	}

	bool ShaderProgram::CreateShader(const std::string& source, const GLenum shader_type, GLuint& shaderID)
	{
		shaderID = glCreateShader(shader_type);
		const char* code = source.c_str();
		glShaderSource(shaderID, 1, &code, nullptr);
		glCompileShader(shaderID);

		GLint success;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

		if (!success) {
			GLchar infolog[1024];
			glGetShaderInfoLog(shaderID, 1024, nullptr, infolog);
			std::cerr << "ERROR::SHADER: Compile time error:\n" << infolog << std::endl;

			return false;
		}

		return true;

	}

}
