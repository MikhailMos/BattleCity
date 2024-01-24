#include "ResourceManager.h"

#include "../renderer/ShaderProgram.h"
#include "../renderer/Texture2D.h"
#include "../renderer/Sprite.h"

#include <sstream>
#include <fstream>
#include <iostream>

#include <rapidjson/document.h>
#include <rapidjson/error/en.h>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include "stb_image.h"

ResourceManager::ShaderProgramsMap ResourceManager::shaderPrograms_;
std::string ResourceManager::path_;
ResourceManager::TexturesMap ResourceManager::textures_;
ResourceManager::SpritesMap ResourceManager::sprites_;
std::vector<std::vector<std::string>> ResourceManager::levels_;
std::vector<std::string> ResourceManager::startScreen_;

void ResourceManager::SetExecutablePath(const std::string& executablePath)
{
	size_t found = executablePath.find_last_of("/\\");
	path_ = executablePath.substr(0, found);
}

void ResourceManager::UnloadAllResources()
{
	shaderPrograms_.clear();
	path_.clear();
	textures_.clear();
	sprites_.clear();
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

	std::shared_ptr<RenderEngine::ShaderProgram>& newShader = shaderPrograms_.emplace(shaderName, std::make_shared<RenderEngine::ShaderProgram>(vertexString, fragmentString)).first->second;
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
	ShaderProgramsMap::const_iterator it = shaderPrograms_.find(shaderName);
	if (it != shaderPrograms_.end()) {
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
	unsigned char* pixels = stbi_load(std::string(path_ + "/" + texturePath).c_str(), &width, &height, &channels, 0);

	if (!pixels) {
		std::cerr << "Can't load image: " << texturePath << std::endl;
		return nullptr;
	}

	std::shared_ptr<RenderEngine::Texture2D> new_texture = textures_.emplace(textureName, std::make_shared<RenderEngine::Texture2D>(width, height, 
																															  pixels, 
																															  channels, 
																															  GL_NEAREST, 
																															  GL_CLAMP_TO_EDGE)).first->second;

	stbi_image_free(pixels);

	return new_texture;
}

std::shared_ptr<RenderEngine::Texture2D> ResourceManager::GetTexture(const std::string& textureName)
{
	TexturesMap::const_iterator it = textures_.find(textureName);
	if (it != textures_.end()) {
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

	std::shared_ptr<RenderEngine::Sprite> new_sprite = sprites_.emplace(spriteName, 
																	  std::make_shared<RenderEngine::Sprite>(pTexture, 
																										 const_cast<std::string&>(subTextureName), 
																										 pShader)
																	 ).first->second;

	return new_sprite;
}

std::shared_ptr<RenderEngine::Sprite> ResourceManager::GetSprite(const std::string& spriteName)
{
	SpritesMap::const_iterator it = sprites_.find(spriteName);
	if (it != sprites_.end()) {
		return it->second;
	}

	std::cerr << "Can't find the sprite: " << spriteName << std::endl;

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
			glm::vec2 leftBottomUV(static_cast<float>(currentTextureOffsetX + 0.01f) / textureWidth,				 static_cast<float>(currentTextureOffsetY - subTextureHeight + 0.01f) / textureHeight);
			glm::vec2 rightTopUV(static_cast<float>(currentTextureOffsetX + subTextureWidth - 0.01f) / textureWidth, static_cast<float>(currentTextureOffsetY - 0.01f) / textureHeight);

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

			auto framesIt = currentSprite.FindMember("frames");
			if (framesIt != currentSprite.MemberEnd()) 
			{
				const auto framesArray = framesIt->value.GetArray();
				std::vector<RenderEngine::Sprite::FrameDescription> framesDescriptions;
				framesDescriptions.reserve(framesArray.Size());
				for (const auto& currentFrame : framesArray)
				{
					const std::string subTextureStr = currentFrame["subTexture"].GetString();
					const double duration = currentFrame["duration"].GetDouble();

					const auto pTextureAtlas = GetTexture(textureAtlas);
					const auto pSubTexture = pTextureAtlas->GetSubTexture(subTextureStr);

					framesDescriptions.emplace_back(pSubTexture.leftBottomUV, pSubTexture.rightTopUV, duration);
				}
				pSprite->InsertFrames(std::move(framesDescriptions));
			}
		}
	}

	// подгружаем стартовый экран
	auto startScreenIt = document.FindMember("start_screen");
	if (startScreenIt != document.MemberEnd())
	{
		const auto descriptionArray = startScreenIt->value.GetArray();
		startScreen_.reserve(descriptionArray.Size());
		size_t maxLength = 0;
		for (const auto& currentRow : descriptionArray)
		{
			startScreen_.emplace_back(currentRow.GetString());
			if (maxLength < startScreen_.back().length())
			{
				maxLength = startScreen_.back().length();
			}
		}
		// выравниваем строки
		for (auto& currentRow : startScreen_)
		{
			while (currentRow.length() < maxLength)
			{
				currentRow.append("F");
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
					curretnRow.append("D"); // пустое место на карте
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

const std::vector<std::string>& ResourceManager::GetStartScreen()
{
	return startScreen_;
}

std::string ResourceManager::GetFileString(const std::string& relativeFilePath)
{
	std::ifstream f;
	f.open(path_ + "/" + relativeFilePath.c_str(), std::ios::in | std::ios::binary);
	if (!f.is_open()) {
		std::cerr << "Failed to open file: " << relativeFilePath << std::endl;
		return std::string{};
	}

	std::stringstream buffer;
	buffer << f.rdbuf();

	return buffer.str();
}
