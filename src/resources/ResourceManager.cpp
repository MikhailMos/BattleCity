#include "ResourceManager.h"
#include "../renderer/ShaderProgram.h"
#include "../renderer/Texture2D.h"

#include <sstream>
#include <fstream>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb_image.h"

ResourceManager::ResourceManager(const std::string& executablePath)
{
	size_t found = executablePath.find_last_of("/\\");
	m_path_ = executablePath.substr(0, found);
}

std::shared_ptr<Renderer::ShaderProgram> ResourceManager::LoadShaders(const std::string& shaderName, const std::string& vertexPath, const std::string& fragmentPath)
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

	std::shared_ptr<Renderer::ShaderProgram>& newShader = m_shaderPrograms_.emplace(shaderName, std::make_shared<Renderer::ShaderProgram>(vertexString, fragmentString)).first->second;
	if (!newShader->IsCompiled()) {
		std::cerr << "Can't load shader program:\n"
			<< "Vertex: " << vertexPath << "\n"
			<< "Fragment: " << fragmentPath << std::endl;
		
		return nullptr;
	}
	
	return newShader;
}

std::shared_ptr<Renderer::ShaderProgram> ResourceManager::GetShaderProgram(const std::string& shaderName)
{
	ShaderProgramsMap::const_iterator it = m_shaderPrograms_.find(shaderName);
	if (it != m_shaderPrograms_.end()) {
		return it->second;
	}

	std::cerr << "Can't find the shader program: " << shaderName << std::endl;

	return nullptr;
}

std::shared_ptr<Renderer::Texture2D> ResourceManager::LoadTexture(const std::string& textureName, const std::string& texturePath)
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

	std::shared_ptr<Renderer::Texture2D> new_texture = m_textures_.emplace(textureName, std::make_shared<Renderer::Texture2D>(width, height, 
																															  pixels, 
																															  channels, 
																															  GL_NEAREST, 
																															  GL_CLAMP_TO_EDGE)).first->second;

	stbi_image_free(pixels);

	return new_texture;
}

std::shared_ptr<Renderer::Texture2D> ResourceManager::GetTexture(const std::string& textureName)
{
	TextureMap::const_iterator it = m_textures_.find(textureName);
	if (it != m_textures_.end()) {
		return it->second;
	}

	std::cerr << "Can't find the texture: " << textureName << std::endl;

	return nullptr;
}

std::string ResourceManager::GetFileString(const std::string& relativeFilePath) const
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
