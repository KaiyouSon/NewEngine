#include "ParticleMeshInfo.h"

ParticleMeshInfo::ParticleMeshInfo()
{
	mChangingName = "nullptr";
	mComponentInfo.type = ComponentType::ParticleMeshInfo;
}

ParticleMeshInfo::ParticleMeshInfo(GameObject* gameObj) : Component(gameObj)
{
	mChangingName = mGameObj->name;
	mComponentInfo.type = ComponentType::ParticleMeshInfo;
}

void ParticleMeshInfo::Update()
{
}

nlohmann::json ParticleMeshInfo::SaveToJson()
{
	nlohmann::json ParticleMeshInfoData;
	ParticleMeshInfoData["particle_mesh_info"] = mComponentInfo.SaveToJson();
	ParticleMeshInfoData["particle_mesh_info"]["color"] = { mColor.r,mColor.g,mColor.b,mColor.a };
	return ParticleMeshInfoData;
}

void ParticleMeshInfo::LoadToJson(const nlohmann::json& componentField)
{
	if (!componentField.contains("particle_mesh_info"))
	{
		return;
	}

	mComponentInfo.LoadToJson(componentField["particle_mesh_info"]);
	mColor.r = componentField["particle_mesh_info"]["color"][0];
	mColor.g = componentField["particle_mesh_info"]["color"][1];
	mColor.b = componentField["particle_mesh_info"]["color"][2];
	mColor.a = componentField["particle_mesh_info"]["color"][3];

	SetParamToObj();
}

void ParticleMeshInfo::ShowDataToInspector()
{
	ParticleMesh* castObj = dynamic_cast<ParticleMesh*>(mGameObj);

	if (Gui::DrawCollapsingHeader("Object Info", true))
	{
		Gui::DrawInputText("Object Name", mChangingName);
		if (!ImGui::IsItemActive())
		{
			castObj->name = mChangingName;
		}

		Gui::DrawColorEdit("Color", mColor);

		SetParamToObj();
	}
}

//void ParticleMeshInfo::DrawLayerTagGUI()
//{
//	std::string label = "LayerTag";
//	std::string previewTag = "Test";
//	static uint32_t currentComboIndex = 0;
//
//	Gui::DrawItemsMapCombo(label, previewTag, currentComboIndex, *Renderer::GetLayerMap());
//}

void ParticleMeshInfo::SetParamToObj()
{
	ParticleMesh* castObj = dynamic_cast<ParticleMesh*>(mGameObj);
	castObj->color = mColor;
}