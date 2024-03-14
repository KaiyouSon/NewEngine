#include "ComponentManager.h"
#include "ComponentFactor.h"

ComponentManager::ComponentManager(GameObject* gameObj) :mGameObj(gameObj)
{
}

void ComponentManager::Update()
{
	for (auto& component : mComponents)
	{
		if (!component)
		{
			continue;
		}

		component->Update();
	}

	std::erase_if(mComponents,
		[](const std::unique_ptr<Component>& component)
		{
			return !component->GetIsUsing();
		});
}

void ComponentManager::Copy(ComponentManager* componentManager)
{
	for (uint32_t i = 0; i < componentManager->mComponents.size(); i++)
	{
		if (i < mComponents.size())
		{
			const auto& thisComponent = mComponents[i];
			const auto& otherComponent = componentManager->mComponents[i];

			thisComponent->CopyComponent(otherComponent.get());
		}
	}

	//mComponents.push_back(std::move(ComponentFactor::CreateComponent(component->GetComponentType())));
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