#pragma once
#include "StandardLib.h"
#include "GameObjectManager.h"
#include "AssetsManager.h"

class Scene
{
private:
	std::string mName;
	std::unique_ptr<GameObjectManager> mGameObjectManager;
	std::unique_ptr<AssetsManager> mAssetsManager;

private:
	void SaveGameObjectData(nlohmann::json& data);
	void SaveAseetesData();

public:
	Scene();
	Scene(const std::string& name);

public:
	void SaveToJson();

public:
	std::string GetName();
	GameObjectManager* GetGameObjectManager();
	AssetsManager* GetAssetsManager();
};
