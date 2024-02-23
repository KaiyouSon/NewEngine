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
	void SaveSceneData();
	void SaveAseetesData();

public:
	Scene();

public:
	void LoadToJson(const std::string& sceneName);
	void SaveToJson();

public:
	std::string GetName();
	GameObjectManager* GetGameObjectManager();
	AssetsManager* GetAssetsManager();
};
