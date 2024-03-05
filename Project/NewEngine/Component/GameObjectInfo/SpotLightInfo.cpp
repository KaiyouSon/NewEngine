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
	nlohmann::json objectInfoData;
	objectInfoData["object_info"].push_back(mComponentInfo.SaveToJson());
	objectInfoData["object_info"].push_back(SaveBaseInfoToJson(mGameObj));
	objectInfoData["object_info"].push_back(SaveSpotLightInfoToJson());
	return objectInfoData;
}

void SpotLightInfo::LoadToJson(const nlohmann::json& componentField)
{
	mComponentInfo.LoadToJson(componentField["object_info"][0]);
	LoadBaseInfoToJson(mGameObj, componentField["object_info"][1]);
	LoadSpotLightInfoToJson(componentField["object_info"][2]);
}

void SpotLightInfo::ShowDataToInspector()
{
	if (Gui::DrawCollapsingHeader("Object Info", true))
	{
		if (Gui::BeginTreeNode("Base Info", true))
		{
			ShowLightObjectDataToInspector(mGameObj);
			Gui::EndTreeNode();
		}

		if (Gui::BeginTreeNode("SpotLight Info", true))
		{
			Gui::DrawSlider3("Light Vec", mCastObj->vec, 0.01f);
			Gui::DrawSlider1("Radius", mCastObj->radius);
			Gui::DrawSlider1("Decay", mCastObj->decay);
			Gui::DrawSlider2("Cos Angle", mCastObj->cosAngle, 0.1f);
			Gui::DrawSlider3("Color Rate", mCastObj->colorRate, 0.01f);
			Gui::EndTreeNode();
		}
	}
}

nlohmann::json SpotLightInfo::SaveSpotLightInfoToJson()
{
	nlohmann::json result;
	result["spot_light_info"]["light_vec"] = { mCastObj->vec.x,mCastObj->vec.y,mCastObj->vec.z };
	result["spot_light_info"]["radius"] = mCastObj->radius;
	result["spot_light_info"]["decay"] = mCastObj->decay;
	result["spot_light_info"]["cos_angle"] = { mCastObj->cosAngle.x,mCastObj->cosAngle.y };
	result["spot_light_info"]["color_rate"] =
	{
		mCastObj->colorRate.x,
		mCastObj->colorRate.y,
		mCastObj->colorRate.z
	};
	return result;
}

void SpotLightInfo::LoadSpotLightInfoToJson(const nlohmann::json& componentField)
{
	mCastObj->radius = componentField["spot_light_info"]["radius"];
	mCastObj->decay = componentField["spot_light_info"]["decay"];
	mCastObj->colorRate.x = componentField["spot_light_info"]["color_rate"][0];
	mCastObj->colorRate.y = componentField["spot_light_info"]["color_rate"][1];
	mCastObj->colorRate.z = componentField["spot_light_info"]["color_rate"][2];
}