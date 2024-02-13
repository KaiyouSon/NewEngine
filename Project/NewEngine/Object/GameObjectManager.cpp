#include "GameObjectManager.h"
#include "SceneManager.h"

void GameObjectManager::Update()
{
	for (const auto& obj : mGameObjects)
	{
		obj->Update();
	}
}

void GameObjectManager::Draw()
{
	for (const auto& obj : mGameObjects)
	{
		obj->Draw();
	}
}

void GameObjectManager::LoadToJson(nlohmann::json& objectsField)
{
	GameObjectType type = (GameObjectType)objectsField["type"];
	std::string name = objectsField["name"];
	AddGameObject(type, name);

	mGameObjects.back()->LoadToJson(objectsField);
}

void GameObjectManager::AddGameObject(const GameObjectType type, const std::string name)
{
	std::unique_ptr<GameObject> obj;
	switch (type)
	{
	case GameObjectType::Object3D:
		obj = std::make_unique<Object3D>(name);
		break;
	case GameObjectType::Sprite:
		obj = std::make_unique<Sprite>(name);
	}
	mGameObjects.push_back(std::move(obj));
}

void GameObjectManager::DestroyGameObject(const std::string name)
{
	std::erase_if(mGameObjects,
		[&](const std::unique_ptr<GameObject>& obj)
		{
			return obj->name == name;
		});
}

GameObject* GameObjectManager::GetGameObject(const std::string name)
{
	for (const auto& obj : mGameObjects)
	{
		if (obj->name == name)
		{
			return obj.get();
		}
	}

	return nullptr;
}

std::vector<std::unique_ptr<GameObject>>* GameObjectManager::GetGameObjects()
{
	return &mGameObjects;
}




void AddGameObject(const GameObjectType type, const std::string name)
{
	auto gemeObjectManager = SceneManager::GetInstance()->mCurrentScene->GetGameObjectManager();
	gemeObjectManager->AddGameObject(type, name);
}

void DestroyGameObject(const std::string name)
{
	auto gemeObjectManager = SceneManager::GetInstance()->mCurrentScene->GetGameObjectManager();
	gemeObjectManager->DestroyGameObject(name);
}

GameObject* GetGameObject(const std::string name)
{
	auto gemeObjectManager = SceneManager::GetInstance()->mCurrentScene->GetGameObjectManager();
	return gemeObjectManager->GetGameObject(name);
}

std::vector<std::unique_ptr<GameObject>>* GetGameObjects()
{
	auto gemeObjectManager = SceneManager::GetInstance()->mCurrentScene->GetGameObjectManager();
	return gemeObjectManager->GetGameObjects();
}
