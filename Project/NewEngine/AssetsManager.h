#pragma once
#include "StandardLib.h"

class AssetsManager
{
private:
	std::array<std::unordered_map<std::string, std::unique_ptr<ITexture>>, 2> mTextureMapArrays;

public:
	void LoadTexture(const std::string& path);

public:
	void LoadAssets(const std::string& sceneName);

	void LoadTextures(const std::string& folderPath);

	Texture* GetTexture(const std::string& tag);


public:
	void Load();
	void UnLoad();

public:
	// ゲッター
	std::unordered_map<std::string, std::unique_ptr<ITexture>>* GetTextureMap(const TextureType texType);
};

void LoadTexture(const std::string& path);
extern AssetsManager* gAssetsManager;