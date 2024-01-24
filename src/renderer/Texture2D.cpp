#include "Texture2D.h"

namespace RenderEngine {
	
	Texture2D::Texture2D(const GLuint width, GLuint height, const unsigned char* data, 
						 const unsigned int channels, 
						 const GLenum filter, 
						 const GLenum wrapMode)
		: width_(width)
		, height_(height)
	{
		switch (channels) {
		case 4:
			mode_ = GL_RGBA;
			break;
		case 3:
			mode_ = GL_RGB;
			break;
		default:
			mode_ = GL_RGBA;
			break;
		}
		
		glGenTextures(1, &ID_);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, ID_);
		glTexImage2D(GL_TEXTURE_2D, 0, mode_, width_, height_, 0, mode_, GL_UNSIGNED_BYTE, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
		glGenerateMipmap(GL_TEXTURE_2D);

		// делаем не активной ни какую стурктуру
		glBindTexture(GL_TEXTURE_2D, 0);
		
	}
	Texture2D& Texture2D::operator=(Texture2D&& texture2d)
	{
		glDeleteTextures(1, &ID_);

		ID_ = texture2d.ID_;
		texture2d.ID_ = 0;

		mode_ = texture2d.mode_;
		width_ = texture2d.width_;
		height_ = texture2d.height_;

		return *this;
	}

	Texture2D::Texture2D(Texture2D&& texture2d)
	{
		ID_ = texture2d.ID_;
		texture2d.ID_ = 0;

		mode_ = texture2d.mode_;
		width_ = texture2d.width_;
		height_ = texture2d.height_;
	}

	Texture2D::~Texture2D()
	{
		glDeleteTextures(1, &ID_);
	}

	void Texture2D::AddSubTexture(std::string name, const glm::vec2& leftBottomUV, const glm::vec2& rightTopUV)
	{
		sub_textures_.emplace(std::move(name), SubTexture2D(leftBottomUV, rightTopUV));
	}

	const Texture2D::SubTexture2D& Texture2D::GetSubTexture(const std::string& name) const
	{
		auto it = sub_textures_.find(name);
		if (it != sub_textures_.end()) 
		{
			return it->second;
		}

		const static SubTexture2D defaultSubTexture;

		return defaultSubTexture;
	}

	unsigned int Texture2D::Width() const
	{
		return width_;
	}

	unsigned int Texture2D::Height() const
	{
		return height_;
	}

	void Texture2D::Bind() const
	{
		glBindTexture(GL_TEXTURE_2D, ID_);
	}

}
