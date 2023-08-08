#include "GraphicsPipelineManager.h"

std::unordered_map<std::string, std::unique_ptr<GraphicsPipeline>> GraphicsPipelineManager::sGraphicsPipelineMap;

GraphicsPipeline* GraphicsPipelineManager::Create(const GraphicsPipelineSetting& setting, const std::string tag)
{
	std::unique_ptr<GraphicsPipeline> gp = std::make_unique<GraphicsPipeline>();
	gp->SetGraphicsPipelineSetter(setting);
	gp->Create();

	sGraphicsPipelineMap.insert(std::make_pair(tag, std::move(gp)));
	return sGraphicsPipelineMap[tag].get();
}

GraphicsPipeline* GraphicsPipelineManager::GetGraphicsPipeline(const std::string tag)
{
	return sGraphicsPipelineMap[tag].get();
}
