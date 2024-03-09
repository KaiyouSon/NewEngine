#include "CameraInfo.h"

CameraInfo::CameraInfo()
{
	mChangingName = "nullptr";
	mComponentInfo.type = ComponentType::CameraInfo;
}

CameraInfo::CameraInfo(GameObject* gameObj) : Component(gameObj)
{
	mChangingName = mGameObj->name;
	mComponentInfo.type = ComponentType::CameraInfo;
}

void CameraInfo::Update()
{
}

nlohmann::json CameraInfo::SaveToJson()
{
	Camera* castObj = dynamic_cast<Camera*>(mGameObj);

	nlohmann::json cameraInfoData;
	cameraInfoData["camera_info"] = mComponentInfo.SaveToJson();
	cameraInfoData["camera_info"]["fov_angle"] = castObj->fovAngle;
	cameraInfoData["camera_info"]["p_near_far_clip"] = { castObj->pNearFarClip.x,castObj->pNearFarClip.y };
	cameraInfoData["camera_info"]["o_near_far_clip"] = { castObj->oNearFarClip.x,castObj->oNearFarClip.y };
	return cameraInfoData;
}

void CameraInfo::LoadToJson(const nlohmann::json& componentField)
{
	Camera* castObj = dynamic_cast<Camera*>(mGameObj);

	mComponentInfo.LoadToJson(componentField["camera_info"]);
	castObj->fovAngle = componentField["camera_info"]["fov_angle"];
	castObj->pNearFarClip.x = componentField["camera_info"]["p_near_far_clip"][0];
	castObj->pNearFarClip.y = componentField["camera_info"]["p_near_far_clip"][1];
	castObj->oNearFarClip.x = componentField["camera_info"]["o_near_far_clip"][0];
	castObj->oNearFarClip.y = componentField["camera_info"]["o_near_far_clip"][1];
}

void CameraInfo::ShowDataToInspector()
{
	Camera* castObj = dynamic_cast<Camera*>(mGameObj);

	if (Gui::DrawCollapsingHeader("Object Info", true))
	{
		Gui::DrawInputText("Object Name", mChangingName);
		if (!ImGui::IsItemActive())
		{
			castObj->name = mChangingName;
		}

		Gui::DrawSlider1("Fov Angle", castObj->fovAngle, 0.01f);

		if (Gui::BeginTreeNode("Near Far Clip", true))
		{
			// 正射影行列を計算
			Gui::DrawSlider2("Orthographic", castObj->pNearFarClip, 0.01f);

			// 透視投影行列を計算
			Gui::DrawSlider2("Perspective", castObj->oNearFarClip, 0.01f);

			Gui::EndTreeNode();
		}
	}
}