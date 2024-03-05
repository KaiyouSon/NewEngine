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
	nlohmann::json objectInfoData;
	objectInfoData["object_info"].push_back(mComponentInfo.SaveToJson());
	objectInfoData["object_info"].push_back(SaveBaseInfoToJson(mGameObj));
	return objectInfoData;
}

void ParticleMeshInfo::LoadToJson(const nlohmann::json& componentField)
{
	mComponentInfo.LoadToJson(componentField["object_info"][0]);
	LoadBaseInfoToJson(mGameObj, componentField["object_info"][1]);
}

void ParticleMeshInfo::ShowDataToInspector()
{
	if (Gui::DrawCollapsingHeader("Object Info", true))
	{
		if (Gui::BeginTreeNode("Base Info", true))
		{
			ShowGameObjectDataToInspector(mGameObj);
			Gui::EndTreeNode();
		}
	}
}