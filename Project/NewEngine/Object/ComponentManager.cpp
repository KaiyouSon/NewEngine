#include "stdafx.h"
#include "ComponentManager.h"

ComponentManager::ComponentManager(GameObject* gameObj) :mGameObj(gameObj)
{
}

void ComponentManager::Update()
{
	for (const auto& component : mComponents)
	{
		component->Update();
	}
}

void ComponentManager::LoadToJson(const nlohmann::json& componentsField)
{
	uint32_t count = 0;
	for (const auto& component : mComponents)
	{
		if (componentsField.size() <= count)
		{
			break;
		}

		component->LoadToJson(componentsField[count]);
		count++;
	}
}

nlohmann::json ComponentManager::SaveToJson()
{
	nlohmann::json componentsData;

	for (const auto& component : mComponents)
	{
		nlohmann::json data = component->SaveToJson();
		componentsData["components"].push_back(data);
	}

	return componentsData;
}

std::vector<std::unique_ptr<Component>>* ComponentManager::GetComponents()
{
	return &mComponents;
}