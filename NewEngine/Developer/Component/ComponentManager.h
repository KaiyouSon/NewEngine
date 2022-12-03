#pragma once
#include "IComponent.h"
#include "Blend.h"
#include "ModelData.h"
#include "Tag.h"
#include "Texture.h"
#include "Transform.h"
#include <memory>
#include <list>

class ComponentManager
{
private:
	std::list<std::unique_ptr<IComponent>> list;

public:
	ComponentManager();
	~ComponentManager();

	void AddComponent(std::unique_ptr<IComponent>&& component);
	void DestroyComponent(std::unique_ptr<IComponent>&& component);

	template<class T>
	T* GetComponent()
	{
		for (const auto& component : list)
		{
			T* currentComponent = dynamic_cast<T*>(component.get());
			if (currentComponent != nullptr) return currentComponent;
		}
		return nullptr;
	}
};

