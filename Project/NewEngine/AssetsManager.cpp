#include "AssetsManager.h"
namespace fs = std::filesystem;
using namespace DirectX;

AssetsManager* gAssetsManager = nullptr;

AssetsManager::AssetsManager() :
	mMaterialManager(std::make_unique<MaterialManager>())
	//,	mModelManager(std::make_unique<ModelManager>())
{
}

void AssetsManager::LoadAssets(const std::string& sceneName)
{
	mTextureManager.mTextureMapArrays[(uint32_t)TextureType::Default].clear();

	// テクスチャのロード
	LoadTextures(AppTextureDirectory + sceneName);

	// マテリアルのロード
	LoadMaterials(EngineMaterialDirectory);	// 一回エンジンのロード

	// モデルのロード
	LoadModels(AppModelDirectory + sceneName);
}
void AssetsManager::LoadMaterials(const std::string folderPath)
{
	fs::path fsFolderPath = folderPath;
	if (!fs::is_directory(fsFolderPath))
	{
		return;
	}

	for (const auto& entry : fs::directory_iterator(fsFolderPath))
	{
		if (fs::is_directory(entry))
		{
			LoadMaterials(entry.path().string());
		}
		else
		{
			fs::path path = entry.path();
			LoadMaterial(path.string());
		}
	}
}
void AssetsManager::LoadTextures(const std::string& folderPath)
{
	fs::path fsFolderPath = folderPath;
	if (!fs::is_directory(fsFolderPath))
	{
		return;
	}

	for (const auto& entry : fs::directory_iterator(fsFolderPath))
	{
		if (fs::is_directory(entry))
		{
			LoadTextures(entry.path().string());
		}
		else
		{
			fs::path path = entry.path();
			LoadTexture(path.string());
		}
	}
}
void AssetsManager::LoadModels(const std::string& folderPath)
{
	fs::path fsFolderPath = folderPath;
	if (!fs::is_directory(fsFolderPath))
	{
		return;
	}

	for (const auto& entry : fs::directory_iterator(fsFolderPath))
	{
		// フォルダーの時
		if (fs::is_directory(entry))
		{
			fs::path path = entry.path();
			std::string filePath = path.string() + "/" + path.filename().string();
			std::string objExt = ".obj";
			std::string fbxExt = ".fbx";

			// フォルダ内にモデルデータがあるなら
			if (fs::exists(filePath + objExt) || fs::exists(filePath + fbxExt))
			{
				LoadModel(path.string());
			}
			else
			{
				LoadModels(entry.path().string());
			}

		}
	}
}

Texture* AssetsManager::GetTexture(const std::string& tag)
{
	Texture* tex = dynamic_cast<Texture*>(mTextureManager.mTextureMapArrays[(uint32_t)TextureType::Default][tag].get());
	return tex;
}
Material* AssetsManager::GetMaterial(const std::string& tag)
{
	return mMaterialManager->GetMaterial(tag);
}
Model* AssetsManager::GetModel(const std::string& tag)
{
	return mModelManager.GetModel2(tag);
}

std::unordered_map<std::string, std::unique_ptr<ITexture>>* AssetsManager::GetTextureMap(const TextureType texType)
{
	return &mTextureManager.mTextureMapArrays[(uint32_t)texType];
}
std::unordered_map<std::string, std::unique_ptr<Model>>* AssetsManager::GetModelMap()
{
	return mModelManager.GetModelMap();
}

void AssetsManager::LoadMaterial(const std::string& path)
{
	mMaterialManager->LoadMaterial(path);
}
void AssetsManager::LoadTexture(const std::string& path)
{
	mTextureManager.LoadTexture(path);
}
ITexture* AssetsManager::LoadMaterialTexture(const std::string & path)
{
	return mTextureManager.LoadMaterialTexture(path);
}
void AssetsManager::LoadModel(const std::string& path)
{
	mModelManager.LoadModel(path);
}
void AssetsManager::LoadModel(const std::wstring& wpath)
{
	mModelManager.LoadModel(wpath);
}

void LoadTexture(const std::string& path)
{
	SceneManager::GetInstance()->mCurrentScene->GetAssetsManager()->LoadTexture(path);
}
