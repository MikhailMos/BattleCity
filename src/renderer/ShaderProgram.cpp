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
		ID_ = glCreateProgram();
		glAttachShader(ID_, vertexShaderID);
		glAttachShader(ID_, fragmentShaderID);
		glLinkProgram(ID_);

		GLint success;
		glGetProgramiv(ID_, GL_LINK_STATUS, &success);
		if (!success) {
			GLchar infolog[1024];
			glGetShaderInfoLog(ID_, 1024, nullptr, infolog);
			std::cerr << "ERROR::SHADER: Link time error:\n" << infolog << std::endl;
			
		}
		else 
		{
			isCompiled_ = true;
		}
		
		// удаляем шейдеры, т.к. они уже есть в ShaderProgram
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
	}

	ShaderProgram::~ShaderProgram()
	{
		glDeleteProgram(ID_);
	}

	ShaderProgram::ShaderProgram(ShaderProgram&& shaderProgram) noexcept
	{
		ID_ = shaderProgram.ID_;
		isCompiled_ = shaderProgram.isCompiled_;

		shaderProgram.ID_ = 0;
		shaderProgram.isCompiled_ = false;
	}

	ShaderProgram& ShaderProgram::operator=(ShaderProgram&& shaderProgram) noexcept
	{
		glDeleteProgram(ID_);
		ID_ = shaderProgram.ID_;
		isCompiled_ = shaderProgram.isCompiled_;

		shaderProgram.ID_ = 0;
		shaderProgram.isCompiled_ = false;

		return *this;
	}

	bool ShaderProgram::IsCompiled() const
	{
		return isCompiled_;
	}

	void ShaderProgram::Use() const
	{
		glUseProgram(ID_);
	}

	void ShaderProgram::SetInt(const std::string& name, const GLint value)
	{
		glUniform1i(glGetUniformLocation(ID_, name.c_str()), value);
	}

	void ShaderProgram::SetFoat(const std::string& name, const GLfloat value)
	{
		glUniform1f(glGetUniformLocation(ID_, name.c_str()), value);
	}

	void ShaderProgram::SetMatrix4(const std::string& name, const glm::mat4& matrix)
	{
		glUniformMatrix4fv(glGetUniformLocation(ID_, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
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
