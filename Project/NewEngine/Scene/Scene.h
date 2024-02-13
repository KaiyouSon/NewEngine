#pragma once
#include "StandardLib.h"
#include "GameObjectManager.h"

class Scene
{
private:
	std::string mTag;
	std::unique_ptr<GameObjectManager> mGameObjectManager;

public:
	Scene();
	Scene(const std::string& tag);

public:
	std::string GetTag();
	GameObjectManager* GetGameObjectManager();
};

