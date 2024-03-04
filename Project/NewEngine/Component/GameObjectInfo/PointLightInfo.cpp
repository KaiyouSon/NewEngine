#include "PointLightInfo.h"

PointLightInfo::PointLightInfo()
{
	mChangingName = "nullptr";
	mComponentInfo.type = ComponentType::PointLightInfo;
}

PointLightInfo::PointLightInfo(GameObject* gameObj) : Component(gameObj)
{
	mCastObj = dynamic_cast<PointLight*>(mGameObj);

	mChangingName = mGameObj->name;
	mComponentInfo.type = ComponentType::PointLightInfo;
}

void PointLightInfo::Update()
{
}

nlohmann::json PointLightInfo::SaveToJson()
{
	nlohmann::json directionalLightInfoData;
	directionalLightInfoData["point_light_info"] = mComponentInfo.SaveToJson();
	directionalLightInfoData["point_light_info"]["is_active"] = mCastObj->isActive;
	directionalLightInfoData["point_light_info"]["radius"] = mCastObj->radius;
	directionalLightInfoData["point_light_info"]["decay"] = mCastObj->decay;
	directionalLightInfoData["point_light_info"]["color"] =
	{
		mCastObj->color.r,
		mCastObj->color.g,
		mCastObj->color.b,
		mCastObj->color.a
	};
	directionalLightInfoData["point_light_info"]["color_rate"] =
	{
		mCastObj->colorRate.x,
		mCastObj->colorRate.y,
		mCastObj->colorRate.z
	};
	return directionalLightInfoData;
}

void PointLightInfo::LoadToJson(const nlohmann::json& componentField)
{
	mComponentInfo.LoadToJson(componentField["point_light_info"]);
	mCastObj->isActive = componentField["point_light_info"]["is_active"];
	mCastObj->radius = componentField["point_light_info"]["radius"];
	mCastObj->decay = componentField["point_light_info"]["decay"];
	mCastObj->color.r = componentField["point_light_info"]["color"][0];
	mCastObj->color.g = componentField["point_light_info"]["color"][1];
	mCastObj->color.b = componentField["point_light_info"]["color"][2];
	mCastObj->color.a = componentField["point_light_info"]["color"][3];
	mCastObj->colorRate.x = componentField["point_light_info"]["color_rate"][0];
	mCastObj->colorRate.y = componentField["point_light_info"]["color_rate"][1];
	mCastObj->colorRate.z = componentField["point_light_info"]["color_rate"][2];
}

void PointLightInfo::ShowDataToInspector()
{
	if (Gui::DrawCollapsingHeader("Object Info", true))
	{
		Gui::DrawCheckBox("Active Flag", &mCastObj->isActive);

		Gui::DrawInputText("Object Name", mChangingName);
		if (!ImGui::IsItemActive())
		{
			mCastObj->name = mChangingName;
		}

		Gui::DrawSlider1("Radius", mCastObj->radius);
		Gui::DrawSlider1("Decay", mCastObj->decay);

		Gui::DrawColorEdit("Color", mCastObj->color);
		Gui::DrawSlider3("Color Rate", mCastObj->colorRate, 0.01f);
	}
}