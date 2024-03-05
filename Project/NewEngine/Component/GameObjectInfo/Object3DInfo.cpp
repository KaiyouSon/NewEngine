#include "Object3DInfo.h"

Object3DInfo::Object3DInfo()
{
	mComponentInfo.type = ComponentType::Object3DInfo;
}

Object3DInfo::Object3DInfo(GameObject* gameObj) : Component(gameObj)
{
	mChangingName = mGameObj->name;
	mComponentInfo.type = ComponentType::Object3DInfo;
}

void Object3DInfo::Update()
{
}

nlohmann::json Object3DInfo::SaveToJson()
{
	nlohmann::json objectInfoData;
	objectInfoData["object_info"].push_back(mComponentInfo.SaveToJson());
	objectInfoData["object_info"].push_back(SaveBaseInfoToJson(mGameObj));
	return objectInfoData;
}

void Object3DInfo::LoadToJson(const nlohmann::json& componentField)
{
	mComponentInfo.LoadToJson(componentField["object_info"][0]);
	LoadBaseInfoToJson(mGameObj, componentField["object_info"][1]);
}

void Object3DInfo::ShowDataToInspector()
{
	if (Gui::DrawCollapsingHeader("Object Info", true))
	{
		if (Gui::BeginTreeNode("Base Info", true))
		{
			ShowGameObjectDataToInspector(mGameObj);
			Gui::EndTreeNode();
		}
	}
}
