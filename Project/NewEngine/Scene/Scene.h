#pragma once
#include "StandardLib.h"
#include "GameObjectManager.h"
#include "AssetsManager.h"
#include "Renderer.h"

class Scene
{
private:
	std::string mName;
	std::unique_ptr<GameObjectManager> mGameObjectManager;
	std::unique_ptr<AssetsManager> mAssetsManager;
	std::unique_ptr<Renderer> mRenderer;

private:
	void SaveSceneData();

public:
	Scene();
	void DrawGameObject();

public:
	void LoadToJson(const std::string& sceneName);
	void SaveToJson();

public:
	std::string GetName();
	GameObjectManager* GetGameObjectManager();
	AssetsManager* GetAssetsManager();
	Renderer* GetRenderer();
};
