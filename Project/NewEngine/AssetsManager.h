#pragma once
#include "StandardLib.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "MaterialManager.h"

class AssetsManager
{
private:
	TextureManager mTextureManager;
	std::unique_ptr<MaterialManager> mMaterialManager;
	//ModelManager mModelManager;

public:
	AssetsManager();

public:
	void LoadTexture(const std::string& path);
	void LoadModel(const std::string& path);

public:
	void LoadAssets(const std::string& sceneName);

	void LoadTextures(const std::string& folderPath);
	void LoadModels(const std::string& folderPath);

	Texture* GetTexture(const std::string& tag);
	Material* GetMaterial(const std::string& tag);

public:
	// ゲッター
	std::unordered_map<std::string, std::unique_ptr<ITexture>>* GetTextureMap(const TextureType texType);
};

void LoadTexture(const std::string& path);
extern AssetsManager* gAssetsManager;