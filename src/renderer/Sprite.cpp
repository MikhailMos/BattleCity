#include "Sprite.h"

#include "ShaderProgram.h"
#include "Texture2D.h"

#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Renderer {
	
	Sprite::Sprite(const std::shared_ptr<Texture2D> pTexture, const std::string& initialSubTexture, const std::shared_ptr<ShaderProgram> pShaderProgram, const glm::vec2& position, const glm::vec2& size, const float rotation)
		: pTexture_(std::move(pTexture))
		, pShaderProgram_(std::move(pShaderProgram))
		, position_(position)
		, size_(size)
		, rotation_(rotation)
	{
		const GLfloat vertexCoords[] = {
			// 2- 3    1
			// | /   / |
			// 1    3 -2

			// X	Y
			 0.f, 0.f,
			 0.f, 1.f,
			 1.f, 1.f,

			 1.f, 1.f,
			 1.f, 0.f,
			 0.f, 0.f
		};

		auto subTexture = pTexture_->GetSubTexture(initialSubTexture);

		const GLfloat textureCoords[] = {
			// U						V
			 subTexture.leftBottomUV.x, subTexture.leftBottomUV.y,
			 subTexture.leftBottomUV.x, subTexture.rightTopUV.y,
			 subTexture.rightTopUV.x,   subTexture.rightTopUV.y,

			 subTexture.rightTopUV.x,   subTexture.rightTopUV.y,
			 subTexture.rightTopUV.x,   subTexture.leftBottomUV.y,
			 subTexture.leftBottomUV.x, subTexture.leftBottomUV.y
		};

		glGenVertexArrays(1, &VAO_);
		glBindVertexArray(VAO_);

		// VERTEX
		glGenBuffers(1, &vertexCoordsVBO_);
		glBindBuffer(GL_ARRAY_BUFFER, vertexCoordsVBO_);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexCoords), &vertexCoords, GL_STATIC_DRAW);
		
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

		// TEXTURE
		glGenBuffers(1, &textureCoordsVBO_);
		glBindBuffer(GL_ARRAY_BUFFER, textureCoordsVBO_);
		glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoords), &textureCoords, GL_STATIC_DRAW);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

		// очищаем
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	Sprite::~Sprite()
	{
		glDeleteBuffers(1, &vertexCoordsVBO_);
		glDeleteBuffers(1, &textureCoordsVBO_);
		glDeleteVertexArrays(1, &VAO_);
	}

	void Sprite::Render() const
	{
		pShaderProgram_->Use();

		glm::mat4 model(1.f);

		// по правилу математики с матрицами, все действия делаем задом на перед
		//5. постановка спрайта в нужное место
		model = glm::translate(model, glm::vec3(position_, 0.f));
		//4. перемещаем обратно, где был
		model = glm::translate(model, glm::vec3(0.5f * size_.x, 0.5f * size_.y, 0.f));
		//3. поворачиваем
		model = glm::rotate(model, glm::radians(rotation_), glm::vec3(0.f, 0.f, 1.f));
		//2. перемещаем к центру, чтобы ориджин был в центре спрайта
		model = glm::translate(model, glm::vec3(-0.5f * size_.x, -0.5f * size_.y, 0.f));
		//1. изменяем размер
		model = glm::scale(model, glm::vec3(size_, 1.f));

		
		glBindVertexArray(VAO_);
		pShaderProgram_->SetMatrix4("modelMat", model);

		// активируем текстуру
		glActiveTexture(GL_TEXTURE0);
		pTexture_->Bind();

		glDrawArrays(GL_TRIANGLES, 0, 6);

		// зануляем
		glBindVertexArray(0);
	}

	void Sprite::SetPosition(const glm::vec2& position)
	{
		position_ = position;
	}

	void Sprite::SetSize(const glm::vec2& size)
	{
		size_ = size;
	}

	void Sprite::SetRotation(const float rotation)
	{
		rotation_ = rotation;
	}
}