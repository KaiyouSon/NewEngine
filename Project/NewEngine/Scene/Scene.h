#pragma once
#include "StandardLib.h"
#include "GameObjectManager.h"
#include "AssetsManager.h"
#include "Renderer.h"

class Scene
{
private:
	std::string mName;
	bool mIsActive;
	std::unique_ptr<GameObjectManager> mGameObjectManager;
	std::unique_ptr<AssetsManager> mAssetsManager;
	std::unique_ptr<Renderer> mRenderer;

private:
	void SaveSceneData();

public:
	Scene();
	void UpdateGameObject();
	void DrawGameObject();

public:
	void LoadToJson(const std::string& sceneName);
	void SaveToJson();

public:
	void SetisActive(const bool isActive);

public:
	std::string GetName();
	GameObjectManager* GetGameObjectManager();
	AssetsManager* GetAssetsManager();
	Renderer* GetRenderer();
	bool GetisActive();
};
