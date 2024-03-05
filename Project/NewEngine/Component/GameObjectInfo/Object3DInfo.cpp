#include "Object3DInfo.h"

Object3DInfo::Object3DInfo()
{
	mComponentInfo.type = ComponentType::Object3DInfo;
}

Object3DInfo::Object3DInfo(GameObject* gameObj) : Component(gameObj)
{
	mCastObj = dynamic_cast<Object3D*>(mGameObj);

	mChangingName = mGameObj->name;
	mComponentInfo.type = ComponentType::Object3DInfo;
}

void Object3DInfo::Update()
{
}

nlohmann::json Object3DInfo::SaveToJson()
{
	nlohmann::json object3DInfoData;

	object3DInfoData["object3D_info"] = mComponentInfo.SaveToJson();
	object3DInfoData["object3D_info"]["is_active"] = mCastObj->isActive;
	object3DInfoData["object3D_info"]["color"] =
	{
		mCastObj->color.r,
		mCastObj->color.g,
		mCastObj->color.b,
		mCastObj->color.a
	};
	return object3DInfoData;
}

void Object3DInfo::LoadToJson(const nlohmann::json& componentField)
{
	mComponentInfo.LoadToJson(componentField["object3D_info"]);
	//mCastObj->isActive = componentField["object3D_info"]["is_active"];
	//mCastObj->isActive = componentField["object3D_info"]["is_active"];
	//mCastObj->color.r = componentField["object3D_info"]["color"][0];
	//mCastObj->color.g = componentField["object3D_info"]["color"][1];
	//mCastObj->color.b = componentField["object3D_info"]["color"][2];
	//mCastObj->color.a = componentField["object3D_info"]["color"][3];
}

void Object3DInfo::ShowDataToInspector()
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
