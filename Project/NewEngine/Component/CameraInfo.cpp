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
	cameraInfoData["camera_info"]["camera_type"] = (uint32_t)castObj->cameraType;
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
	castObj->cameraType = (CameraType)componentField["camera_info"]["camera_type"];
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

		Vec2 nearFarClip;
		if (castObj->cameraType == CameraType::Perspective)
		{
			nearFarClip = castObj->pNearFarClip;
		}
		else
		{
			nearFarClip = castObj->oNearFarClip;
		}
		Gui::DrawSlider2("Near Far Clip", nearFarClip, 0.01f);

		if (Gui::BeginTreeNode("Camera Type", true))
		{
			uint32_t cameraType = (uint32_t)castObj->cameraType;
			Gui::DrawRadioButton("Perspective ", &cameraType, (uint32_t)CameraType::Perspective);
			Gui::DrawRadioButton("Orthographic", &cameraType, (uint32_t)CameraType::Orthographic, false);
			castObj->cameraType = (CameraType)cameraType;

			Gui::EndTreeNode();
		}
	}
}