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
	static std::unordered_map<std::string, std::unique_ptr<GraphicsPipeline>> sGraphicsPipelineMap_;

public:
	static GraphicsPipeline* Create(
		ShaderObject* shaderObject,
		ID3D12RootSignature* rootSignature,
		const CullMode cullMode,
		const D3D12_DEPTH_STENCIL_DESC depthStencilDesc,
		const TopologyType topologyType,
		const uint32_t rtvNum,
		const std::string graphicsPipelineTag);

	static GraphicsPipeline* GetGraphicsPipeline(const std::string graphicsPipelineTag);
};

