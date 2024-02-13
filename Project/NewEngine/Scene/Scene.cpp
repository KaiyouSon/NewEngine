#include "Scene.h"

Scene::Scene()
{

}

Scene::Scene(const std::string& tag) :
	mTag(tag), mGameObjectManager(std::make_unique<GameObjectManager>())
{
}

std::string Scene::GetTag()
{
	return mTag;
}

GameObjectManager* Scene::GetGameObjectManager()
{
	return mGameObjectManager.get();
}
