#include "Sprite.h"

#include "ShaderProgram.h"
#include "Texture2D.h"

#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Renderer {
	
	Sprite::Sprite(std::shared_ptr<Texture2D> pTexture, std::string& initialSubTexture, std::shared_ptr<ShaderProgram> pShaderProgram, const glm::vec2& position, const glm::vec2& size, const float rotation)
		: pTexture_(std::move(pTexture))
		, pShaderProgram_(std::move(pShaderProgram))
		, position_(position)
		, size_(size)
		, rotation_(rotation)
	{
		const GLfloat vertexCoords[] = {
			// 1-  2
			// | / |
			// 0  -3

			// X	Y
			 0.f, 0.f,
			 0.f, 1.f,
			 1.f, 1.f,
			 1.f, 0.f,
		};

		auto subTexture = pTexture_->GetSubTexture(std::move(initialSubTexture));

		const GLfloat textureCoords[] = {
			// U						V
			 subTexture.leftBottomUV.x, subTexture.leftBottomUV.y,
			 subTexture.leftBottomUV.x, subTexture.rightTopUV.y,
			 subTexture.rightTopUV.x,   subTexture.rightTopUV.y,
			 subTexture.rightTopUV.x,   subTexture.leftBottomUV.y,
		};

		// создаем индексы
		const GLuint indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		// VERTEX
		vertexCoordsBuffer_.Init(vertexCoords, 2 * 4 * sizeof(GLfloat));
		VertexBufferLayout vertexCoordsLayout;
		vertexCoordsLayout.AddElementLayoutFloat(2, false);
		vertexArray_.AddBuffer(vertexCoordsBuffer_, vertexCoordsLayout);

		// TEXTURE
		textureCoordsBuffer_.Init(textureCoords, 2 * 4 * sizeof(GLfloat));
		VertexBufferLayout textureCoordsLayout;
		textureCoordsLayout.AddElementLayoutFloat(2, false);
		vertexArray_.AddBuffer(textureCoordsBuffer_, textureCoordsLayout);

		// ELEMENT BUFFER OBJECT
		indexBuffer_.Init(indices, 6 * sizeof(GLuint));

		// очищаем
		vertexArray_.Unbind();
		indexBuffer_.Unbind();
		
	}

	Sprite::~Sprite()
	{
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

		
		vertexArray_.Bind();
		pShaderProgram_->SetMatrix4("modelMat", model);

		// активируем текстуру
		glActiveTexture(GL_TEXTURE0);
		pTexture_->Bind();

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		// зануляем
		vertexArray_.Unbind();
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