#pragma once

#include <vector>
#include <string>
#include <memory>

class IGameObjcect;

class Level {
public:
	Level(const std::vector<std::string>& levelDescription);
	void Render() const;
	void Update(const uint64_t delta);
	size_t GetLevelWidth() const;
	size_t GetLevelHeight() const;

private:
	size_t width_;
	size_t height_;
	std::vector<std::shared_ptr<IGameObjcect>> mapObjects_;
};