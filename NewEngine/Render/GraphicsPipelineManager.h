#pragma once
#include "GraphicsPipeline.h"
#include <d3d12.h>
#include <string>
#include <memory>
#include <map>

class GraphicsPipelineManager
{
private:
	static std::map<std::string, std::unique_ptr<GraphicsPipeline>> graphicsPipelineMap;

public:
	static GraphicsPipeline* Create(
		ShaderObject* shaderObject,
		ID3D12RootSignature* rootSignature,
		const CullMode& cullMode,
		const D3D12_DEPTH_STENCIL_DESC& depthStencilDesc,
		const TopologyType& topologyType,
		const size_t& rtvNum,
		const std::string& graphicsPipelineTag);

	static GraphicsPipeline* GetGraphicsPipeline(const std::string& graphicsPipelineTag);
};

