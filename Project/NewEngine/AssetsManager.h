#pragma once
#include "StandardLib.h"

class AssetsManager
{
private:
	std::array<std::unordered_map<std::string, std::unique_ptr<ITexture>>, 2> mTextureMapArrays;

public:
	void LoadTexture(const std::string& path);

public:
	void SaveToJson(const std::string& sceneName);
	void LoadToJson(const std::string& sceneName);

public:
	void Load();
	void UnLoad();
};

void LoadTexture(const std::string& path);