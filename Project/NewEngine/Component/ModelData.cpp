#include "ModelData.h"

ModelData::ModelData()
{
	mModel = ModelManager::GetModel("Sphere");

	mComponentInfo.type = ComponentType::ModelData;
}

ModelData::ModelData(GameObject* gameObj) : Component(gameObj)
{
	mModel = ModelManager::GetModel("Sphere");

	mComponentInfo.type = ComponentType::ModelData;
}

void ModelData::Update()
{
}

Model* ModelData::GetModel()
{
	return mModel;
}

void ModelData::SetModel(const std::string& tag)
{
	//mModel = ModelManager::GetModel(tag);
	mModel = gAssetsManager->GetModel(tag);
}

nlohmann::json ModelData::SaveToJson()
{
	nlohmann::json modelData;
	modelData["model_data"] = mComponentInfo.SaveToJson();
	modelData["model_data"]["model_tag"] = mModel->tag;
	modelData["model_data"]["model_format"] = mModel->format;

	return modelData;
}

void ModelData::LoadToJson(const nlohmann::json& componentField)
{
	nlohmann::json modelDataField = componentField["model_data"];

	mComponentInfo.LoadToJson(modelDataField);

	std::string modelTag = modelDataField["model_tag"];

	Object3D* castObj = dynamic_cast<Object3D*>(mGameObj);
	castObj->SetModel(modelTag);
}

void ModelData::ShowDataToInspector()
{
	if (Gui::DrawCollapsingHeader("Model Data", true))
	{
		static uint32_t currentComboIndex = 0;
		std::string previewTag = mModel ? mModel->tag : "empty";

		std::string label = "Current Model";
		if (ImGui::BeginCombo(label.c_str(), previewTag.c_str()))
		{
			uint32_t index = 0;
			for (const auto& [tag, tex] : *gAssetsManager->GetModelMap())
			{
				const bool isSelected = (currentComboIndex == index);
				if (ImGui::Selectable(tag.c_str(), isSelected))
				{
					ImGui::SetItemDefaultFocus();
					currentComboIndex = index;
					previewTag = tag;

					Object3D* castObj = dynamic_cast<Object3D*>(mGameObj);
					castObj->SetModel(tag);
				}
				index++;
			}
			ImGui::EndCombo();
		}

		if (mModel)
		{
			if (Gui::BeginTreeNode("Model Texture", true))
			{
				Vec2 size = 256;
				Gui::DrawImage(mModel->texture, size);

				Gui::EndTreeNode();
			}
		}
	}
}