#include "DirectionalLightInfo.h"

DirectionalLightInfo::DirectionalLightInfo()
{
	mChangingName = "nullptr";
	mComponentInfo.type = ComponentType::DirectionalLightInfo;
}

DirectionalLightInfo::DirectionalLightInfo(GameObject* gameObj) : Component(gameObj)
{
	mCastObj = dynamic_cast<DirectionalLight*>(mGameObj);

	mChangingName = mGameObj->name;
	mComponentInfo.type = ComponentType::DirectionalLightInfo;
}

void DirectionalLightInfo::Update()
{
}

nlohmann::json DirectionalLightInfo::SaveToJson()
{
	nlohmann::json objectInfoData;
	objectInfoData["object_info"].push_back(mComponentInfo.SaveToJson());
	objectInfoData["object_info"].push_back(SaveBaseInfoToJson(mGameObj));
	return objectInfoData;
}

void DirectionalLightInfo::LoadToJson(const nlohmann::json& componentField)
{
	mComponentInfo.LoadToJson(componentField["object_info"][0]);
	LoadBaseInfoToJson(mGameObj, componentField["object_info"][1]);
}

void DirectionalLightInfo::ShowDataToInspector()
{
	if (Gui::DrawCollapsingHeader("Object Info", true))
	{
		if (Gui::BeginTreeNode("Base Info", true))
		{
			ShowLightObjectDataToInspector(mGameObj);
			Gui::EndTreeNode();
		}
	}
}