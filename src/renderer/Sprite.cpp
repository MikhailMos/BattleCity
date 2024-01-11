#include "Sprite.h"

#include "ShaderProgram.h"
#include "Texture2D.h"
#include "Renderer.h"

#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace RenderEngine {
	
	Sprite::Sprite(std::shared_ptr<Texture2D> pTexture, std::string& initialSubTexture, std::shared_ptr<ShaderProgram> pShaderProgram)
		: pTexture_(std::move(pTexture))
		, pShaderProgram_(std::move(pShaderProgram))
		, last_frameId_(0)
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
		indexBuffer_.Init(indices, 6);

		// очищаем
		vertexArray_.Unbind();
		indexBuffer_.Unbind();
		
	}

	Sprite::~Sprite()
	{
	}

	void Sprite::Render(const glm::vec2& position, const glm::vec2& size, const float rotation, const float layer, const size_t frameId) const
	{
		if (last_frameId_ != frameId)
		{
			last_frameId_ = frameId;
			const FrameDescription& currentFrameDescription = framesDescriptions_[frameId];

			const GLfloat textureCoords[] = {
				// U						V
				 currentFrameDescription.leftBottomUV.x, currentFrameDescription.leftBottomUV.y,
				 currentFrameDescription.leftBottomUV.x, currentFrameDescription.rightTopUV.y,
				 currentFrameDescription.rightTopUV.x,   currentFrameDescription.rightTopUV.y,
				 currentFrameDescription.rightTopUV.x,   currentFrameDescription.leftBottomUV.y,
			};

			textureCoordsBuffer_.Update(textureCoords, 2 * 4 * sizeof(GLfloat));
		}
		
		pShaderProgram_->Use();

		glm::mat4 model(1.f);

		// по правилу математики с матрицами, все действия делаем задом на перед
		//5. постановка спрайта в нужное место
		model = glm::translate(model, glm::vec3(position, 0.f));
		//4. перемещаем обратно, где был
		model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.f));
		//3. поворачиваем
		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.f, 0.f, 1.f));
		//2. перемещаем к центру, чтобы ориджин был в центре спрайта
		model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.f));
		//1. изменяем размер
		model = glm::scale(model, glm::vec3(size, 1.f));
		
		pShaderProgram_->SetMatrix4("modelMat", model);
		pShaderProgram_->SetFoat("layer", layer);

		// активируем текстуру
		glActiveTexture(GL_TEXTURE0);
		pTexture_->Bind();

		Renderer::Draw(vertexArray_, indexBuffer_, *pShaderProgram_);
	}

	uint64_t Sprite::GetFrameDuration(const size_t frameId) const
	{
		return framesDescriptions_[frameId].duration;
	}

	size_t Sprite::GetFramesCount() const
	{
		return framesDescriptions_.size();
	}

	void Sprite::InsertFrames(std::vector<FrameDescription> framesDescriptions)
	{
		framesDescriptions_ = std::move(framesDescriptions);
	}

}