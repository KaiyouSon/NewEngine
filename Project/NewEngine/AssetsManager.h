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
	ModelManager mModelManager;

public:
	AssetsManager();

public:
	void LoadMaterial(const std::string& path);
	void LoadTexture(const std::string& path);
	ITexture* LoadMaterialTexture(const std::string& path);
	void LoadModel(const std::string& path);
	void LoadModel(const std::wstring& wpath);

public:
	void LoadAssets(const std::string& sceneName);

	void LoadMaterials(const std::string folderPath);
	void LoadTextures(const std::string& folderPath);
	void LoadModels(const std::string& folderPath);

	std::unique_ptr<RenderTexture> CreateRenderTexture(const Vec2 size = Vec2(1920, 1080));

public:
	// ゲッター
	Texture* GetTexture(const std::string& tag);
	Material* GetMaterial(const std::string& tag);
	Model* GetModel(const std::string& tag);

public:
	// ゲッター
	std::unordered_map<std::string, std::unique_ptr<ITexture>>* GetTextureMap(const TextureType texType);
	std::unordered_map<std::string, std::unique_ptr<Model>>* GetModelMap();
};

void LoadTexture(const std::string& path);
extern AssetsManager* gAssetsManager;