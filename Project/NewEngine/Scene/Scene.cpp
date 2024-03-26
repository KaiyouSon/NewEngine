#include "Scene.h"

Scene::Scene() :
	mName(std::string()), mIsActive(false),
	mGameObjectManager(std::make_unique<GameObjectManager>()),
	mAssetsManager(std::make_unique<AssetsManager>()),
	mRenderer(std::make_unique<Renderer>())
{
}

void Scene::UpdateGameObject()
{
	mGameObjectManager->Update();
}

void Scene::DrawGameObject()
{
	mGameObjectManager->AppedToRenderer();
	mRenderer->DrawObject();
}

void Scene::LoadToJson(const std::string& sceneName)
{
	std::string path = EngineDataDirectory + "Scene/" + sceneName + "Scene.json";

	// ファイルを開く
	std::ifstream file(path);
	// ファイルが開けない場合はアサーションエラー
	if (file.fail())
	{
		assert(0);
	}

	// JSONをデシリアライズ
	nlohmann::json deserialized;
	file >> deserialized;

	mName = deserialized["name"];

	// アセットのロード
	gAssetsManager = mAssetsManager.get();
	mAssetsManager->LoadAssets(mName);

	mGameObjectManager->GetGameObjects()->clear();

	// "objects"フィールドの各オブジェクトを処理
	for (nlohmann::json& object : deserialized["objects"])
	{
		mGameObjectManager->LoadToJson(object["object"]);
	}

	file.close();

	// 全てのオブジェクトロードし終わったら親子関係を設定する
	mGameObjectManager->SettingParentAfterLoad();
	mRenderer->LoadToJson(sceneName);
}

void Scene::SaveToJson()
{
	// シーンのデータを保存
	SaveSceneData();
}

void Scene::SaveSceneData()
{
	nlohmann::json data;
	data["name"] = mName;
	data["objects"] = mGameObjectManager->SaveToJson();

	std::ofstream file(EngineDataDirectory + "Scene/" + mName + "Scene.json");
	file << std::setw(4) << data << std::endl;
}

void Scene::SetisActive(const bool isActive)
{
	mIsActive = isActive;
}

std::string Scene::GetName()
{
	return mName;
}

GameObjectManager* Scene::GetGameObjectManager()
{
	return mGameObjectManager.get();
}

AssetsManager* Scene::GetAssetsManager()
{
	return mAssetsManager.get();
}

Renderer* Scene::GetRenderer()
{
	return mRenderer.get();
}

bool Scene::GetisActive()
{
	return mIsActive;
}
