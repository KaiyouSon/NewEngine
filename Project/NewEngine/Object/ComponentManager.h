#pragma once
#include "Component.h"
#include "Object3DInfo.h"
#include "ParticleMeshInfo.h"
#include "CameraInfo.h"
#include "SpriteInfo.h"
#include "DirectionalLightInfo.h"
#include "PointLightInfo.h"
#include "SpotLightInfo.h"
#include "EmptyObjectInfo.h"
#include "Transform.h"
#include "ModelData.h"
#include "TextureData.h"
#include "ScriptsComponent.h"
#include "MaterialComponent.h"

class ComponentManager
{
private:
	std::vector<std::unique_ptr<Component>> mComponents;
	GameObject* mGameObj;

public:
	ComponentManager() {}
	ComponentManager(GameObject* gameObj);
	void Update();
	void Copy(ComponentManager* componentManager);

public:
	void LoadToJson(const nlohmann::json& componentsField);
	nlohmann::json SaveToJson();

public:
	std::vector<std::unique_ptr<Component>>* GetComponents();

public:
	template<class T>
	void AddComponent()
	{
		mComponents.push_back(std::move(std::make_unique<T>(mGameObj)));
		mComponents.back()->SetComponentInfo((uint32_t)mComponents.size() - 1);
	}

public:
	template<class T>
	T* GetComponent()
	{
		for (const auto& component : mComponents)
		{
			T* resutl = dynamic_cast<T*>(component.get());
			if (resutl != nullptr)
			{
				return resutl;
			}
		}
		return nullptr;
	}
};

