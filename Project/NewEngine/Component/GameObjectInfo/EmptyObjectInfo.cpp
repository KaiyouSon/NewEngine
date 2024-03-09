#include"EmptyObjectInfo.h"

EmptyObjectInfo::EmptyObjectInfo()
{
	mComponentInfo.type = ComponentType::EmptyObjectInfo;
}

EmptyObjectInfo::EmptyObjectInfo(GameObject* gameObj) : Component(gameObj)
{
	mChangingName = mGameObj->name;
	mComponentInfo.type = ComponentType::EmptyObjectInfo;
}

void EmptyObjectInfo::Update()
{
}

nlohmann::json EmptyObjectInfo::SaveToJson()
{
	nlohmann::json objectInfoData;
	objectInfoData["object_info"].push_back(mComponentInfo.SaveToJson());
	objectInfoData["object_info"].push_back(SaveBaseInfoToJson(mGameObj));
	return objectInfoData;
}

void EmptyObjectInfo::LoadToJson(const nlohmann::json& componentField)
{
	if (!componentField.contains("object_info"))
	{
		return;
	}

	mComponentInfo.LoadToJson(componentField["object_info"][0]);
	LoadBaseInfoToJson(mGameObj, componentField["object_info"][1]);
}

void EmptyObjectInfo::ShowDataToInspector()
{
	if (Gui::DrawCollapsingHeader("Object Info", true))
	{
		if (Gui::BeginTreeNode("Base Info", true))
		{
			ShowEmptyObjectDataToInspector(mGameObj);
			Gui::EndTreeNode();
		}
	}
}
