#include "ResourceManager.h"

#include "../renderer/ShaderProgram.h"
#include "../renderer/Texture2D.h"
#include "../renderer/Sprite.h"
#include "../renderer/AnimatedSprite.h"

#include <sstream>
#include <fstream>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb_image.h"

ResourceManager::ShaderProgramsMap ResourceManager::m_shaderPrograms_;
std::string ResourceManager::m_path_;
ResourceManager::TexturesMap ResourceManager::m_textures_;
ResourceManager::SpritesMap ResourceManager::m_sprites_;
ResourceManager::AnimatedSpritesMap ResourceManager::m_animated_sprites_;

void ResourceManager::SetExecutablePath(const std::string& executablePath)
{
	size_t found = executablePath.find_last_of("/\\");
	m_path_ = executablePath.substr(0, found);
}

void ResourceManager::UnloadAllResources()
{
	m_shaderPrograms_.clear();
	m_path_.clear();
	m_textures_.clear();
	m_sprites_.clear();
	m_animated_sprites_.clear();
}

std::shared_ptr<RenderEngine::ShaderProgram> ResourceManager::LoadShaders(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath)
{
	std::string vertexString = GetFileString(vertexPath);
	if (vertexString.empty()) {
		std::cerr << "No vertex shader!" << std::endl;
		return nullptr;
	}

	std::string fragmentString = GetFileString(fragmentPath);
	if (vertexString.empty()) {
		std::cerr << "No fragment shader!" << std::endl;
		return nullptr;
	}

	std::shared_ptr<RenderEngine::ShaderProgram>& newShader = m_shaderPrograms_.emplace(shaderName, std::make_shared<RenderEngine::ShaderProgram>(vertexString, fragmentString)).first->second;
	if (!newShader->IsCompiled()) {
		std::cerr << "Can't load shader program:\n"
			<< "Vertex: " << vertexPath << "\n"
			<< "Fragment: " << fragmentPath << std::endl;
		
		return nullptr;
	}
	
	return newShader;
}

std::shared_ptr<RenderEngine::ShaderProgram> ResourceManager::GetShaderProgram(const std::string& shaderName)
{
	ShaderProgramsMap::const_iterator it = m_shaderPrograms_.find(shaderName);
	if (it != m_shaderPrograms_.end()) {
		return it->second;
	}

	std::cerr << "Can't find the shader program: " << shaderName << std::endl;

	return nullptr;
}

std::shared_ptr<RenderEngine::Texture2D> ResourceManager::LoadTexture(const std::string& textureName, const std::string& texturePath)
{
	int channels = 0;
	int width = 0;
	int height = 0;

	stbi_set_flip_vertically_on_load(true);
	unsigned char* pixels = stbi_load(std::string(m_path_ + "/" + texturePath).c_str(), &width, &height, &channels, 0);

	if (!pixels) {
		std::cerr << "Can't load image: " << texturePath << std::endl;
		return nullptr;
	}

	std::shared_ptr<RenderEngine::Texture2D> new_texture = m_textures_.emplace(textureName, std::make_shared<RenderEngine::Texture2D>(width, height, 
																															  pixels, 
																															  channels, 
																															  GL_NEAREST, 
																															  GL_CLAMP_TO_EDGE)).first->second;

	stbi_image_free(pixels);

	return new_texture;
}

std::shared_ptr<RenderEngine::Texture2D> ResourceManager::GetTexture(const std::string& textureName)
{
	TexturesMap::const_iterator it = m_textures_.find(textureName);
	if (it != m_textures_.end()) {
		return it->second;
	}

	std::cerr << "Can't find the texture: " << textureName << std::endl;

	return nullptr;
}

std::shared_ptr<RenderEngine::Sprite> ResourceManager::LoadSprite(const std::string& spriteName, 
															  const std::string& textureName, 
															  const std::string& shaderName, 
															  const unsigned int spriteWidth, 
															  const unsigned int spriteHeight,
															  const std::string& subTextureName)
{
	auto pTexture = GetTexture(textureName);
	if (!pTexture) 
	{
		std::cerr << "Can't find the texture: " << textureName << " for the sprite: " << spriteName << std::endl;
		return nullptr;
	}

	auto pShader = GetShaderProgram(shaderName);
	if (!pShader)
	{
		std::cerr << "Can't find the shader: " << shaderName << " for the sprite: " << spriteName << std::endl;
		return nullptr;
	}

	std::shared_ptr<RenderEngine::Sprite> new_sprite = m_sprites_.emplace(spriteName, 
																	  std::make_shared<RenderEngine::Sprite>(pTexture, 
																										 const_cast<std::string&>(subTextureName), 
																										 pShader, 
																										 glm::vec2(0.f, 0.f), 
																										 glm::vec2((float)spriteWidth, (float)spriteHeight)
																										)
																	 ).first->second;

	return new_sprite;
}

std::shared_ptr<RenderEngine::Sprite> ResourceManager::GetSprite(const std::string& spriteName)
{
	SpritesMap::const_iterator it = m_sprites_.find(spriteName);
	if (it != m_sprites_.end()) {
		return it->second;
	}

	std::cerr << "Can't find the sprite: " << spriteName << std::endl;

	return nullptr;
}

std::shared_ptr<RenderEngine::AnimatedSprite> ResourceManager::LoadAnimatedSprite(const std::string& spriteName, const std::string& textureName, const std::string& shaderName, const unsigned int spriteWidth, const unsigned int spriteHeight, const std::string& subTextureName)
{
	auto pTexture = GetTexture(textureName);
	if (!pTexture)
	{
		std::cerr << "Can't find the texture: " << textureName << " for the animated sprite: " << spriteName << std::endl;
		return nullptr;
	}

	auto pShader = GetShaderProgram(shaderName);
	if (!pShader)
	{
		std::cerr << "Can't find the shader: " << shaderName << " for the animated sprite: " << spriteName << std::endl;
		return nullptr;
	}

	std::shared_ptr<RenderEngine::AnimatedSprite> new_sprite = m_animated_sprites_.emplace(spriteName,
																					   std::make_shared<RenderEngine::AnimatedSprite>(pTexture, 
																																  const_cast<std::string&>(subTextureName),
																																  pShader, 
																																  glm::vec2(0.f, 0.f), 
																																  glm::vec2((float)spriteWidth, (float)spriteHeight)
																																)
																					  ).first->second;

	return new_sprite;
}

std::shared_ptr<RenderEngine::AnimatedSprite> ResourceManager::GetAnimatedSprite(const std::string& spriteName)
{
	auto it = m_animated_sprites_.find(spriteName);
	if (it != m_animated_sprites_.end()) {
		return it->second;
	}

	std::cerr << "Can't find the animated sprite: " << spriteName << std::endl;

	return nullptr;
}

std::shared_ptr<RenderEngine::Texture2D> ResourceManager::LoadTextureAtlas(std::string textureName, 
																	std::string texturePath, 
																	std::vector<std::string> subTextures, 
																	const unsigned int subTextureWidth, 
																	const unsigned int subTextureHeight)
{
	auto pTexture = LoadTexture(std::move(textureName), std::move(texturePath));
	if (pTexture) 
	{
		const unsigned int textureWidth = pTexture->Width();
		const unsigned int textureHeight = pTexture->Height();
		unsigned int currentTextureOffsetX = 0;
		unsigned int currentTextureOffsetY = textureHeight;

		for (auto& currentSubTextureName : subTextures) 
		{
			glm::vec2 leftBottomUV(static_cast<float>(currentTextureOffsetX) / textureWidth,				 static_cast<float>(currentTextureOffsetY - subTextureHeight) / textureHeight);
			glm::vec2 rightTopUV(static_cast<float>(currentTextureOffsetX + subTextureWidth) / textureWidth, static_cast<float>(currentTextureOffsetY) / textureHeight);

			pTexture->AddSubTexture(std::move(currentSubTextureName), leftBottomUV, rightTopUV);

			currentTextureOffsetX += subTextureWidth;
			if (currentTextureOffsetX >= textureWidth) 
			{
				currentTextureOffsetX = 0;
				currentTextureOffsetY -= subTextureHeight;
			}
		}
	}

	return pTexture;
}

std::string ResourceManager::GetFileString(const std::string& relativeFilePath)
{
	std::ifstream f;
	f.open(m_path_ + "/" + relativeFilePath.c_str(), std::ios::in | std::ios::binary);
	if (!f.is_open()) {
		std::cerr << "Failed to open file: " << relativeFilePath << std::endl;
		return std::string{};
	}

	std::stringstream buffer;
	buffer << f.rdbuf();

	return buffer.str();
}
