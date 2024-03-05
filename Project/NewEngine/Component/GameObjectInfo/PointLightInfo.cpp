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
	nlohmann::json objectInfoData;
	objectInfoData["object_info"].push_back(mComponentInfo.SaveToJson());
	objectInfoData["object_info"].push_back(SaveBaseInfoToJson(mGameObj));
	objectInfoData["object_info"].push_back(SavePointLightInfoToJson());
	return objectInfoData;
}

void PointLightInfo::LoadToJson(const nlohmann::json& componentField)
{
	mComponentInfo.LoadToJson(componentField["object_info"][0]);
	LoadBaseInfoToJson(mGameObj, componentField["object_info"][1]);
	LoadPointLightInfoToJson(componentField["object_info"][2]);
}

void PointLightInfo::ShowDataToInspector()
{
	if (Gui::DrawCollapsingHeader("Object Info", true))
	{
		if (Gui::BeginTreeNode("Base Info", true))
		{
			ShowLightObjectDataToInspector(mGameObj);
			Gui::EndTreeNode();
		}

		if (Gui::BeginTreeNode("PointLight Info", true))
		{
			Gui::DrawSlider1("Radius", mCastObj->radius);
			Gui::DrawSlider1("Decay", mCastObj->decay);
			Gui::DrawSlider3("Color Rate", mCastObj->colorRate, 0.01f);
			Gui::EndTreeNode();
		}
	}
}

nlohmann::json PointLightInfo::SavePointLightInfoToJson()
{
	nlohmann::json result;
	result["point_light_info"]["radius"] = mCastObj->radius;
	result["point_light_info"]["decay"] = mCastObj->decay;
	result["point_light_info"]["color_rate"] =
	{
		mCastObj->colorRate.x,
		mCastObj->colorRate.y,
		mCastObj->colorRate.z
	};
	return result;
}

void PointLightInfo::LoadPointLightInfoToJson(const nlohmann::json& componentField)
{
	mCastObj->radius = componentField["point_light_info"]["radius"];
	mCastObj->decay = componentField["point_light_info"]["decay"];
	mCastObj->colorRate.x = componentField["point_light_info"]["color_rate"][0];
	mCastObj->colorRate.y = componentField["point_light_info"]["color_rate"][1];
	mCastObj->colorRate.z = componentField["point_light_info"]["color_rate"][2];
}