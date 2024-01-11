#include "Texture2D.h"

namespace RenderEngine {
	
	Texture2D::Texture2D(const GLuint width, GLuint height, const unsigned char* data, 
						 const unsigned int channels, 
						 const GLenum filter, 
						 const GLenum wrapMode)
		: m_width_(width)
		, m_height_(height)
	{
		switch (channels) {
		case 4:
			m_mode_ = GL_RGBA;
			break;
		case 3:
			m_mode_ = GL_RGB;
			break;
		default:
			m_mode_ = GL_RGBA;
			break;
		}
		
		glGenTextures(1, &m_ID_);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_ID_);
		glTexImage2D(GL_TEXTURE_2D, 0, m_mode_, m_width_, m_height_, 0, m_mode_, GL_UNSIGNED_BYTE, data);

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
		glDeleteTextures(1, &m_ID_);

		m_ID_ = texture2d.m_ID_;
		texture2d.m_ID_ = 0;

		m_mode_ = texture2d.m_mode_;
		m_width_ = texture2d.m_width_;
		m_height_ = texture2d.m_height_;

		return *this;
	}

	Texture2D::Texture2D(Texture2D&& texture2d)
	{
		m_ID_ = texture2d.m_ID_;
		texture2d.m_ID_ = 0;

		m_mode_ = texture2d.m_mode_;
		m_width_ = texture2d.m_width_;
		m_height_ = texture2d.m_height_;
	}

	Texture2D::~Texture2D()
	{
		glDeleteTextures(1, &m_ID_);
	}

	void Texture2D::AddSubTexture(std::string name, const glm::vec2& leftBottomUV, const glm::vec2& rightTopUV)
	{
		m_sub_textures_.emplace(std::move(name), SubTexture2D(leftBottomUV, rightTopUV));
	}

	const Texture2D::SubTexture2D& Texture2D::GetSubTexture(const std::string& name) const
	{
		auto it = m_sub_textures_.find(name);
		if (it != m_sub_textures_.end()) 
		{
			return it->second;
		}

		const static SubTexture2D defaultSubTexture;

		return defaultSubTexture;
	}

	unsigned int Texture2D::Width() const
	{
		return m_width_;
	}

	unsigned int Texture2D::Height() const
	{
		return m_height_;
	}

	void Texture2D::Bind() const
	{
		glBindTexture(GL_TEXTURE_2D, m_ID_);
	}

}
