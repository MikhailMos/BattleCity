#include "ResourceManager.h"

#include "../renderer/ShaderProgram.h"
#include "../renderer/Texture2D.h"
#include "../renderer/Sprite.h"
#include "../renderer/AnimatedSprite.h"

#include <sstream>
#include <fstream>
#include <iostream>

#include <rapidjson/document.h>
#include <rapidjson/error/en.h>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb_image.h"

ResourceManager::ShaderProgramsMap ResourceManager::m_shaderPrograms_;
std::string ResourceManager::m_path_;
ResourceManager::TexturesMap ResourceManager::m_textures_;
ResourceManager::SpritesMap ResourceManager::m_sprites_;
ResourceManager::AnimatedSpritesMap ResourceManager::m_animated_sprites_;
std::vector<std::vector<std::string>> ResourceManager::levels_;

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
																										 pShader)
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

std::shared_ptr<RenderEngine::AnimatedSprite> ResourceManager::LoadAnimatedSprite(const std::string& spriteName, const std::string& textureName, const std::string& shaderName, const std::string& subTextureName)
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
																																  pShader)
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

bool ResourceManager::LoadJSONResources(const std::string& JSONPath)
{
	const std::string JSONString = GetFileString(JSONPath);
	if (JSONString.empty())
	{
		std::cerr << "No JSON resources file!" << std::endl;
		return false;
	}

	rapidjson::Document document;
	rapidjson::ParseResult parseResult = document.Parse(JSONString.c_str());
	if (!parseResult)
	{
		std::cerr << "JSON parse error: " << rapidjson::GetParseError_En(parseResult.Code()) << " (" << parseResult.Offset() << ")" << std::endl;
		std::cerr << "In JSON file: " << JSONPath << std::endl;
		return false;
	}

	// подгуражем шейдеры
	auto shaderIt = document.FindMember("shaders");
	if (shaderIt != document.MemberEnd()) 
	{
		for (const auto& currentShader : shaderIt->value.GetArray()) 
		{
			const std::string name = currentShader["name"].GetString();
			const std::string filePath_v = currentShader["filePath_v"].GetString();
			const std::string filePath_f = currentShader["filePath_f"].GetString();

			LoadShaders(name, filePath_v, filePath_f);
		}
	}

	// подгружаем текстурные атласы
	auto textureAtlasesIt = document.FindMember("textureAtlases");
	if (textureAtlasesIt != document.MemberEnd())
	{
		for (const auto& currentTextureAtlas : textureAtlasesIt->value.GetArray())
		{
			const std::string name = currentTextureAtlas["name"].GetString();
			const std::string filePath = currentTextureAtlas["filePath"].GetString();
			const unsigned int subTextureWidth = currentTextureAtlas["subTextureWidth"].GetInt();
			const unsigned int subTextureHeight = currentTextureAtlas["subTextureHeight"].GetInt();

			const auto subTexturesArray = currentTextureAtlas["subTextures"].GetArray();
			std::vector<std::string> subTextures;
			subTextures.reserve(subTexturesArray.Size());

			for (const auto& currentSubTexture : subTexturesArray)
			{
				subTextures.emplace_back(currentSubTexture.GetString());
			}

			LoadTextureAtlas(name, filePath, std::move(subTextures), subTextureWidth, subTextureHeight);
		}
	}

	// подгружаем статические объекты (спрайты)
	auto spritesIt = document.FindMember("sprites");
	if (spritesIt != document.MemberEnd())
	{
		for (const auto& currentSprite : spritesIt->value.GetArray())
		{
			const std::string name = currentSprite["name"].GetString();
			const std::string textureAtlas = currentSprite["textureAtlas"].GetString();
			const std::string shader = currentSprite["shader"].GetString();
			const std::string initialSubTexture = currentSprite["initialSubTexture"].GetString();

			auto pSprite = LoadSprite(name, textureAtlas, shader, initialSubTexture);
			if (!pSprite)
			{
				continue;
			}
		}
	}

	// подгружаем анимацию (спрайты)
	auto animatedSpritesIt = document.FindMember("animatedSprites");
	if (animatedSpritesIt != document.MemberEnd())
	{
		for (const auto& currentAnimatedSprite : animatedSpritesIt->value.GetArray())
		{
			const std::string name = currentAnimatedSprite["name"].GetString();
			const std::string textureAtlas = currentAnimatedSprite["textureAtlas"].GetString();
			const std::string shader = currentAnimatedSprite["shader"].GetString();
			const std::string initialSubTexture = currentAnimatedSprite["initialSubTexture"].GetString();
			
			auto pAnimatedSprite = LoadAnimatedSprite(name, textureAtlas, shader, initialSubTexture);
			if (!pAnimatedSprite)
			{
				continue;
			}

			const auto statesArray = currentAnimatedSprite["states"].GetArray();
			for (const auto& currentState : statesArray)
			{
				const std::string stateName = currentState["stateName"].GetString();
				
				const auto framesArray = currentState["frames"].GetArray();
				std::vector<std::pair<std::string, uint64_t>> frames;
				frames.reserve(framesArray.Size());
				for (const auto& currentFrame : framesArray)
				{
					const std::string subTexture = currentFrame["subTexture"].GetString();
					const uint64_t duration = currentFrame["duration"].GetUint64();

					frames.emplace_back(std::make_pair(subTexture, duration));
				}

				pAnimatedSprite->InsertState(stateName, std::move(frames));
			}

		}
	}

	// подгружаем текстурные уровня	
	auto levelsIt = document.FindMember("levels");
	if (levelsIt != document.MemberEnd())
	{
		for (const auto& currentLevel : levelsIt->value.GetArray())
		{
			const auto descriptionArray = currentLevel["description"].GetArray();
			std::vector<std::string> levelRows;
			levelRows.reserve(descriptionArray.Size());
			size_t maxLength = 0;
			for (const auto& currentRow : descriptionArray)
			{
				levelRows.emplace_back(currentRow.GetString());
				if (maxLength < levelRows.back().length())
				{
					maxLength = levelRows.back().length();
				}
			}
			
			for (auto& curretnRow : levelRows)
			{
				while (curretnRow.length() < maxLength)
				{
					curretnRow.append("F"); // F - пустое место на карте (start_screen_maping.png)
				}
			}

			levels_.emplace_back(std::move(levelRows));
		}
	}

	return true;
}

const std::vector<std::vector<std::string>>& ResourceManager::GetLevels()
{
	return levels_;
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
