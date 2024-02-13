#pragma once
#include "Component.h"

class PipelineData /*: public Component*/
{
private:
	GraphicsPipeline* mGrphicsPipeline;
	ComputePipeline* mComputePipeline;
//
//public:
//	nlohmann::json SaveToJson() override;
//	void LoadToJson(const nlohmann::json& componentField) override;
//	void ShowDataToInspector() override;
};

