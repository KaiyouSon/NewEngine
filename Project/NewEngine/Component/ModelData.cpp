#include "ModelData.h"
#include "MainWindow.h"

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

void ModelData::CopyComponent(Component* component)
{
	auto castComponent = dynamic_cast<ModelData*>(component);

	Object3D* castObj = dynamic_cast<Object3D*>(mGameObj);
	castObj->SetModel(castComponent->mModel);
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

	if (mModel)
	{
		modelData["model_data"]["model_tag"] = mModel->tag;
		modelData["model_data"]["model_format"] = mModel->format;
	}

	return modelData;
}

void ModelData::LoadToJson(const nlohmann::json& componentField)
{
	nlohmann::json modelDataField = componentField["model_data"];
	mComponentInfo.LoadToJson(modelDataField);

	if (modelDataField.contains("model_tag"))
	{
		std::string modelTag = modelDataField["model_tag"];
		Object3D* castObj = dynamic_cast<Object3D*>(mGameObj);
		castObj->SetModel(modelTag);
	}
}

void ModelData::ShowDataToInspector()
{
	if (Gui::DrawCollapsingHeader("Model Data", true))
	{
		std::string label = "empty";
		if (mModel)
		{
			label = mModel->tag;
		}

		Gui::DrawString("Current Model : ");

		const float windowWidth = ImGui::GetContentRegionAvail().x;
		const Vec2 buttonSize = Vec2(windowWidth, 40);
		Gui::DrawButton(label.c_str(), buttonSize);

		if (Gui::DragDropTarget("DragDrop Model"))
		{
			std::string tag = MainWindow::GetInstance()->GetDragDropTag(MainWindow::Model);
			Object3D* castObj = dynamic_cast<Object3D*>(mGameObj);
			castObj->SetModel(tag);
		}

		//static uint32_t currentComboIndex = 0;
		//std::string previewTag = mModel ? mModel->tag : "empty";

		//std::string label = "Current Model";
		//if (ImGui::BeginCombo(label.c_str(), previewTag.c_str()))
		//{
		//	uint32_t index = 0;
		//	for (const auto& [tag, tex] : *gAssetsManager->GetModelMap())
		//	{
		//		const bool isSelected = (currentComboIndex == index);
		//		if (ImGui::Selectable(tag.c_str(), isSelected))
		//		{
		//			ImGui::SetItemDefaultFocus();
		//			currentComboIndex = index;
		//			previewTag = tag;

		//			Object3D* castObj = dynamic_cast<Object3D*>(mGameObj);
		//			castObj->SetModel(tag);
		//		}
		//		index++;
		//	}
		//	ImGui::EndCombo();
		//}

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