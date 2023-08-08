#pragma once
#include "GraphicsPipeline.h"
#include <d3d12.h>
#include <string>
#include <memory>
#include <unordered_map>
#include <cstdint>

class GraphicsPipelineManager
{
private:
	static std::unordered_map<std::string, std::unique_ptr<GraphicsPipeline>> sGraphicsPipelineMap;

public:
	// ¶¬
	static GraphicsPipeline* Create(const GraphicsPipelineSetting& setting, const std::string tag);

	// æ“¾
	static GraphicsPipeline* GetGraphicsPipeline(const std::string tag);
};

