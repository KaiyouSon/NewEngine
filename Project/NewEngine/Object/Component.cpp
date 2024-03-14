#include "Component.h"

void ComponentInfo::LoadToJson(const nlohmann::json& componentInfoField)
{
	index = componentInfoField["component_info"]["index"];
	type = (ComponentType)componentInfoField["component_info"]["type"];
}

nlohmann::json ComponentInfo::SaveToJson()
{
	nlohmann::json componentInfoData;
	componentInfoData["component_info"]["index"] = index;
	componentInfoData["component_info"]["type"] = (uint32_t)type;
	return componentInfoData;
}

Component::Component()
{
}

Component::Component(GameObject* gameObj) :
	mGameObj(gameObj), mIsUsing(true)
{
}

void Component::SetComponentInfo(const uint32_t index)
{
	mComponentInfo.index = index;
}

ComponentType Component::GetComponentType()
{
	return mComponentInfo.type;
}

bool Component::GetIsUsing()
{
	return mIsUsing;
}
