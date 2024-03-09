#include "ComponentFactor.h"

std::unique_ptr<Component> ComponentFactor::CreateComponent(const ComponentType type)
{
	switch (type)
	{
	case ComponentType::ModelData:
		return std::make_unique<ModelData>();

	}

	return nullptr;
}
