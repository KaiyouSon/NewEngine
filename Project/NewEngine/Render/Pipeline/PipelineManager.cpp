#include "PipelineManager.h"
#include "json.hpp"

std::unordered_map<std::string, std::unique_ptr<GraphicsPipeline>> PipelineManager::sGraphicsPipelineMap;
std::unordered_map<std::string, std::unique_ptr<ComputePipeline>> PipelineManager::sComputePipelineMap;

// GraphicsPipelineの生成
void PipelineManager::CreateGraphicsPipeline(const GraphicsPipelineSetting& setting, const std::string tag)
{
	std::unique_ptr<GraphicsPipeline> gp = std::make_unique<GraphicsPipeline>(setting);

	sGraphicsPipelineMap.insert(std::make_pair(tag, std::move(gp)));
}

void PipelineManager::ReCreateGraphicsPipeline(const std::string tag)
{
	tag;
	//sGraphicsPipelineMap[tag]->Create(sGraphicsPipelineMap[tag]->GetSetting());
}

void PipelineManager::ReCreateGraphicsPipeline(const GraphicsPipelineSetting& setting, const std::string tag)
{
	tag;
	setting;
	//sGraphicsPipelineMap[tag]->Create(setting);
}

// ComputePipelineの生成
void PipelineManager::CreateComputePipeline(const ComputePipelineSetting& setting, const std::string tag)
{
	std::unique_ptr<ComputePipeline> cp = std::make_unique<ComputePipeline>();
	cp->Create(setting);

	sComputePipelineMap.insert(std::make_pair(tag, std::move(cp)));
}

void PipelineManager::SavePipelineData()
{
	nlohmann::json jsonArray = nlohmann::json::array();
	for (const auto& [tag, pipeline] : sGraphicsPipelineMap)
	{
		auto setting = pipeline->GetSetting();

		nlohmann::json jsonLayer = nlohmann::json
		{
			{ "Tag", tag },
			{ "Pipeline Blend", setting.renderTargetBlendMode },
			{ "Pipeline Type", (uint32_t)setting.pipelineType },
			{ "Fill Mode", (uint32_t)setting.fillMode },
			{ "Cull Mode", (uint32_t)setting.cullMode },
			{ "Topology Type",(uint32_t)setting.topologyType },
		};

		if (setting.vs)
		{
			jsonLayer.push_back({ "VS Tag", setting.vs->GetShaderTag() });
		}
		if (setting.gs)
		{
			jsonLayer.push_back({ "GS Tag", setting.gs->GetShaderTag() });
		}
		if (setting.ps)
		{
			jsonLayer.push_back({ "PS Tag", setting.ps->GetShaderTag() });
		}

		jsonArray.push_back(jsonLayer);
	}
	nlohmann::json jsonData = nlohmann::json{ {"Pipeline",jsonArray} };

	std::ofstream file(EngineDataDirectory + "PipelineData.json");
	file << std::setw(4) << jsonData << std::endl;
}

// ゲッター
GraphicsPipeline* PipelineManager::GetGraphicsPipeline(const std::string tag)
{
	auto it = sGraphicsPipelineMap.find(tag);
	if (it == sGraphicsPipelineMap.end())
	{
		return nullptr;
	}

	return sGraphicsPipelineMap[tag].get();
}
ComputePipeline* PipelineManager::GetComputePipeline(const std::string tag)
{
	auto it = sComputePipelineMap.find(tag);
	if (it == sComputePipelineMap.end())
	{
		return nullptr;
	}

	return sComputePipelineMap[tag].get();
}

GraphicsPipelineSetting PipelineManager::GetGraphicsPipelineSetting(const std::string tag)
{
	auto it = sGraphicsPipelineMap.find(tag);
	if (it == sGraphicsPipelineMap.end())
	{
		return GraphicsPipelineSetting();
	}

	return sGraphicsPipelineMap[tag]->GetSetting();
}

std::unordered_map<std::string, std::unique_ptr<GraphicsPipeline>>* PipelineManager::GetGraphicsPipelineMap()
{
	return &sGraphicsPipelineMap;
}
