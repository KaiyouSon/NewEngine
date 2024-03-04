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
	nlohmann::json object3DInfoData;
	object3DInfoData["object3D_info"] = mComponentInfo.SaveToJson();
	return object3DInfoData;
}

void Object3DInfo::LoadToJson(const nlohmann::json& componentField)
{
	mComponentInfo.LoadToJson(componentField["object3D_info"]);
}

void Object3DInfo::ShowDataToInspector()
{
	Object3D* castObj = dynamic_cast<Object3D*>(mGameObj);

	if (Gui::DrawCollapsingHeader("Object Info", true))
	{
		Gui::DrawInputText("Object Name", mChangingName);
		if (!ImGui::IsItemActive())
		{
			castObj->name = mChangingName;
		}
	}
}
