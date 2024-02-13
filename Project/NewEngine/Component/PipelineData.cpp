#include "PipelineData.h"

PipelineData::PipelineData()
{
	mComponentInfo.type = ComponentType::PipelineData;
}

PipelineData::PipelineData(GameObject* gameObj) : Component(gameObj)
{
	mComponentInfo.type = ComponentType::PipelineData;
}

void PipelineData::Update()
{
}

nlohmann::json PipelineData::SaveToJson()
{
	return nlohmann::json();
}

void PipelineData::LoadToJson(const nlohmann::json& componentField)
{
	componentField;
}

void PipelineData::ShowDataToInspector()
{
	if (Gui::DrawCollapsingHeader("Pipeline Data", true))
	{
		static uint32_t currentComboIndex = 0;
		std::string previewTag = "Test";

		ImGui::SetNextItemWidth(196.f);
		if (ImGui::BeginCombo("Graphics Pipeline", previewTag.c_str()))
		{
			uint32_t index = 0;
			for (const auto& [tag, pipeline] : *PipelineManager::GetGraphicsPipelineMap())
			{
				const bool isSelected = (currentComboIndex == index);
				if (ImGui::Selectable(tag.c_str(), isSelected))
				{
					ImGui::SetItemDefaultFocus();
					currentComboIndex = index;
					previewTag = tag;
					//SetTexture(tex.get(), i);
					//if (mCurrentTexIndex == i)
					//{
					//	mGameObj->SetTexture(previewTag);
					//}
				}
				index++;
			}
			ImGui::EndCombo();
		}
	}
}