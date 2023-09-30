#pragma once
#include "GraphicsPipeline.h"
#include "ComputePipeline.h"
#include <string>
#include <memory>
#include <unordered_map>

class PipelineManager
{
private:
	static std::unordered_map<std::string, std::unique_ptr<GraphicsPipeline>> sGraphicsPipelineMap;
	static std::unordered_map<std::string, std::unique_ptr<ComputePipeline>> sComputePipelineMap;

public:
	// GraphicsPipeline�̍쐬
	static void CreateGraphicsPipeline(const GraphicsPipelineSetting& setting, const std::string tag);

	// ComputePipeline�̍쐬
	static void CreateComputePipeline(const ComputePipelineSetting& setting, const std::string tag);

	// �擾
	static GraphicsPipeline* GetGraphicsPipeline(const std::string tag);
	static ComputePipeline* GetComputePipeline(const std::string tag);
};

