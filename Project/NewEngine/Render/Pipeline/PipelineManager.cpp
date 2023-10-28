#include "PipelineManager.h"

std::unordered_map<std::string, std::unique_ptr<GraphicsPipeline>> PipelineManager::sGraphicsPipelineMap;
std::unordered_map<std::string, std::unique_ptr<ComputePipeline>> PipelineManager::sComputePipelineMap;

// GraphicsPipelineの生成
void PipelineManager::CreateGraphicsPipeline(const GraphicsPipelineSetting& setting, const std::string tag)
{
	std::unique_ptr<GraphicsPipeline> gp = std::make_unique<GraphicsPipeline>();
	gp->Create(setting);

	sGraphicsPipelineMap.insert(std::make_pair(tag, std::move(gp)));
}

// ComputePipelineの生成
void PipelineManager::CreateComputePipeline(const ComputePipelineSetting& setting, const std::string tag)
{
	std::unique_ptr<ComputePipeline> cp = std::make_unique<ComputePipeline>();
	cp->Create(setting);

	sComputePipelineMap.insert(std::make_pair(tag, std::move(cp)));
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