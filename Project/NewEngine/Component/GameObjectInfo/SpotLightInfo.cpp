#include "SpotLightInfo.h"

SpotLightInfo::SpotLightInfo()
{
	mChangingName = "nullptr";
	mComponentInfo.type = ComponentType::SpotLightInfo;
}

SpotLightInfo::SpotLightInfo(GameObject* gameObj) : Component(gameObj)
{
	mCastObj = dynamic_cast<SpotLight*>(mGameObj);

	mChangingName = mGameObj->name;
	mComponentInfo.type = ComponentType::SpotLightInfo;
}

void SpotLightInfo::Update()
{
}

nlohmann::json SpotLightInfo::SaveToJson()
{
	nlohmann::json directionalLightInfoData;
	directionalLightInfoData["point_light_info"] = mComponentInfo.SaveToJson();
	directionalLightInfoData["point_light_info"]["is_active"] = mCastObj->isActive;
	directionalLightInfoData["point_light_info"]["light_vec"] = { mCastObj->vec.x,mCastObj->vec.y,mCastObj->vec.z };
	directionalLightInfoData["point_light_info"]["radius"] = mCastObj->radius;
	directionalLightInfoData["point_light_info"]["decay"] = mCastObj->decay;
	directionalLightInfoData["point_light_info"]["cos_angle"] = { mCastObj->cosAngle.x,mCastObj->cosAngle.y };
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

void SpotLightInfo::LoadToJson(const nlohmann::json& componentField)
{
	mComponentInfo.LoadToJson(componentField["point_light_info"]);
	mCastObj->isActive = componentField["point_light_info"]["is_active"];
	mCastObj->vec.x = componentField["point_light_info"]["light_vec"][0];
	mCastObj->vec.y = componentField["point_light_info"]["light_vec"][1];
	mCastObj->vec.z = componentField["point_light_info"]["light_vec"][2];
	mCastObj->radius = componentField["point_light_info"]["radius"];
	mCastObj->decay = componentField["point_light_info"]["decay"];
	mCastObj->cosAngle.x = componentField["point_light_info"]["cos_angle"][0];
	mCastObj->cosAngle.y = componentField["point_light_info"]["cos_angle"][1];
	mCastObj->color.r = componentField["point_light_info"]["color"][0];
	mCastObj->color.g = componentField["point_light_info"]["color"][1];
	mCastObj->color.b = componentField["point_light_info"]["color"][2];
	mCastObj->color.a = componentField["point_light_info"]["color"][3];
	mCastObj->colorRate.x = componentField["point_light_info"]["color_rate"][0];
	mCastObj->colorRate.y = componentField["point_light_info"]["color_rate"][1];
	mCastObj->colorRate.z = componentField["point_light_info"]["color_rate"][2];
}

void SpotLightInfo::ShowDataToInspector()
{
	if (Gui::DrawCollapsingHeader("Object Info", true))
	{
		Gui::DrawCheckBox("Active Flag", &mCastObj->isActive);

		Gui::DrawInputText("Object Name", mChangingName);
		if (!ImGui::IsItemActive())
		{
			mCastObj->name = mChangingName;
		}

		Gui::DrawSlider3("Light Vec", mCastObj->vec, 0.01f);
		Gui::DrawSlider1("Radius", mCastObj->radius);
		Gui::DrawSlider1("Decay", mCastObj->decay);
		Gui::DrawSlider2("Cos Angle", mCastObj->cosAngle, 0.1f);

		Gui::DrawColorEdit("Color", mCastObj->color);
		Gui::DrawSlider3("Color Rate", mCastObj->colorRate, 0.01f);
	}
}