﻿#pragma once

#include <string>
#include <memory>
#include <map>
#include <vector>

namespace RenderEngine 
{
	class ShaderProgram;
	class Texture2D;
	class Sprite;
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

	static std::shared_ptr<RenderEngine::ShaderProgram> LoadShaders(const std::string& shaderName, const std::string & vertexPath, const std::string& fragmentPath);
	static std::shared_ptr<RenderEngine::ShaderProgram> GetShaderProgram(const std::string& shaderName);
	
	static std::shared_ptr<RenderEngine::Texture2D> LoadTexture(const std::string& textureName, const std::string& texturePath);
	static std::shared_ptr<RenderEngine::Texture2D> GetTexture(const std::string& textureName);

	static std::shared_ptr<RenderEngine::Sprite> LoadSprite(const std::string& spriteName,
												 const std::string& textureName, 
												 const std::string& shaderName, 
												 const std::string& subTextureName = "default");
	static std::shared_ptr<RenderEngine::Sprite> GetSprite(const std::string& spriteName);

	static std::shared_ptr<RenderEngine::Texture2D> LoadTextureAtlas(std::string textureName,
														  std::string texturePath, 
														  std::vector<std::string> subTextures, 
														  const unsigned int subTextureWidth, 
														  const unsigned int subTextureHeight);

	static bool LoadJSONResources(const std::string& JSONPath);

	static const std::vector<std::vector<std::string>>& GetLevels();
	static const std::vector<std::string>& GetStartScreen();

private:
	typedef std::map<const std::string, std::shared_ptr<RenderEngine::ShaderProgram>> ShaderProgramsMap;
	static ShaderProgramsMap shaderPrograms_;

	static std::string path_;

	typedef std::map<const std::string, std::shared_ptr<RenderEngine::Texture2D>> TexturesMap;
	static TexturesMap textures_;

	typedef std::map<const std::string, std::shared_ptr<RenderEngine::Sprite>> SpritesMap;
	static SpritesMap sprites_;

	static std::vector<std::vector<std::string>> levels_;
	static std::vector<std::string> startScreen_;

	static std::string GetFileString(const std::string& relativeFilePath);

};