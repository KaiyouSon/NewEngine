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
	nlohmann::json directionalLightInfoData;
	directionalLightInfoData["directional_light_info"] = mComponentInfo.SaveToJson();
	directionalLightInfoData["directional_light_info"]["is_active"] = mCastObj->isActive;
	directionalLightInfoData["directional_light_info"]["color"] = { mCastObj->color.r,mCastObj->color.g,mCastObj->color.b,mCastObj->color.a };
	return directionalLightInfoData;
}

void DirectionalLightInfo::LoadToJson(const nlohmann::json& componentField)
{
	mComponentInfo.LoadToJson(componentField["directional_light_info"]);
	mCastObj->isActive = componentField["directional_light_info"]["is_active"];
	mCastObj->color.r = componentField["directional_light_info"]["color"][0];
	mCastObj->color.g = componentField["directional_light_info"]["color"][1];
	mCastObj->color.b = componentField["directional_light_info"]["color"][2];
	mCastObj->color.a = componentField["directional_light_info"]["color"][3];
}

void DirectionalLightInfo::ShowDataToInspector()
{
	if (Gui::DrawCollapsingHeader("Object Info", true))
	{
		Gui::DrawCheckBox("Active Flag", &mCastObj->isActive);

		Gui::DrawInputText("Object Name", mChangingName);
		if (!ImGui::IsItemActive())
		{
			mCastObj->name = mChangingName;
		}

		Gui::DrawColorEdit("Color", mCastObj->color);
	}
}