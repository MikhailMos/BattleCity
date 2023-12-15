#pragma once

#include <string>
#include <memory>
#include <map>
#include <vector>

namespace Renderer 
{
	class ShaderProgram;
	class Texture2D;
	class Sprite;
	class AnimatedSprite;
}

class ResourceManager {
public:
	static void SetExecutablePath(const std::string& executablePath);
	static void UnloadAllResources();
	
	~ResourceManager() = delete;
	ResourceManager() = delete;
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
	ResourceManager& operator=(ResourceManager&&) = delete;
	ResourceManager(ResourceManager&&) = delete;

	static std::shared_ptr<Renderer::ShaderProgram> LoadShaders(const std::string& shaderName, const std::string & vertexPath, const std::string& fragmentPath);
	static std::shared_ptr<Renderer::ShaderProgram> GetShaderProgram(const std::string& shaderName);
	
	static std::shared_ptr<Renderer::Texture2D> LoadTexture(const std::string& textureName, const std::string& texturePath);
	static std::shared_ptr<Renderer::Texture2D> GetTexture(const std::string& textureName);

	static std::shared_ptr<Renderer::Sprite> LoadSprite(const std::string& spriteName,
												 const std::string& textureName, 
												 const std::string& shaderName, 
												 const unsigned int spriteWidth, 
												 const unsigned int spriteHeight, 
												 const std::string& subTextureName = "default");
	static std::shared_ptr<Renderer::Sprite> GetSprite(const std::string& spriteName);

	static std::shared_ptr<Renderer::AnimatedSprite> LoadAnimatedSprite(const std::string& spriteName,
														 const std::string& textureName,
														 const std::string& shaderName,
														 const unsigned int spriteWidth,
														 const unsigned int spriteHeight,
														 const std::string& subTextureName = "default");
	static std::shared_ptr<Renderer::AnimatedSprite> GetAnimatedSprite(const std::string& spriteName);

	static std::shared_ptr<Renderer::Texture2D> LoadTextureAtlas(std::string textureName,
														  std::string texturePath, 
														  std::vector<std::string> subTextures, 
														  const unsigned int subTextureWidth, 
														  const unsigned int subTextureHeight);


private:
	typedef std::map<const std::string, std::shared_ptr<Renderer::ShaderProgram>> ShaderProgramsMap;
	static ShaderProgramsMap m_shaderPrograms_;

	static std::string m_path_;

	typedef std::map<const std::string, std::shared_ptr<Renderer::Texture2D>> TexturesMap;
	static TexturesMap m_textures_;

	typedef std::map<const std::string, std::shared_ptr<Renderer::Sprite>> SpritesMap;
	static SpritesMap m_sprites_;

	typedef std::map<const std::string, std::shared_ptr<Renderer::AnimatedSprite>> AnimatedSpritesMap;
	static AnimatedSpritesMap m_animated_sprites_;

	static std::string GetFileString(const std::string& relativeFilePath);

};