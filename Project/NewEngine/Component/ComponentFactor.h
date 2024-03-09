#pragma once
#include "ComponentManager.h"

class ComponentFactor
{
public:
	static std::unique_ptr<Component> CreateComponent(const ComponentType type);
};

