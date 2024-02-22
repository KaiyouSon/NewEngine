#include "Scene.h"

Scene::Scene()
{

}

Scene::Scene(const std::string& name) :
	mName(name),
	mGameObjectManager(std::make_unique<GameObjectManager>()),
	mAssetsManager(std::make_unique<AssetsManager>())
{
	mAssetsManager->LoadToJson(mName);
}

void Scene::SaveToJson()
{
	SaveAseetesData();

	nlohmann::json data;
	data["name"] = mName;

	SaveGameObjectData(data);

	std::ofstream file(EngineDataDirectory + "Scene/" + mName + "Scene.json");
	file << std::setw(4) << data << std::endl;
}

void Scene::SaveGameObjectData(nlohmann::json& data)
{
	nlohmann::json objectsData;
	for (const auto& obj : *mGameObjectManager->GetGameObjects())
	{
		nlohmann::json objectData;
		objectData["object"] = obj->SaveToJson();

		objectsData.push_back(objectData);
	}

	data["objects"] = objectsData;
}

void Scene::SaveAseetesData()
{
	mAssetsManager->SaveToJson(mName);
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
