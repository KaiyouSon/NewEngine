#include "GraphicsPipelineManager.h"

std::map<std::string, std::unique_ptr<GraphicsPipeline>> GraphicsPipelineManager::sGraphicsPipelineMap_;

GraphicsPipeline* GraphicsPipelineManager::Create(
	ShaderObject* shaderObject,
	ID3D12RootSignature* rootSignature,
	const CullMode cullMode,
	const D3D12_DEPTH_STENCIL_DESC depthStencilDesc,
	const TopologyType topologyType,
	const uint32_t rtvNum,
	const std::string graphicsPipelineTag)
{
	std::unique_ptr<GraphicsPipeline> gp = std::make_unique<GraphicsPipeline>();
	gp->SetShaderObject(shaderObject);
	gp->SetCullMode(cullMode);
	gp->SetDepthStencilDesc(depthStencilDesc);
	gp->SetTopologyType(topologyType);
	gp->SetRootSignature(rootSignature);
	gp->SetRTVNum(rtvNum);
	gp->Create();

	sGraphicsPipelineMap_.insert(std::make_pair(graphicsPipelineTag, std::move(gp)));

	return sGraphicsPipelineMap_[graphicsPipelineTag].get();
}

GraphicsPipeline* GraphicsPipelineManager::GetGraphicsPipeline(const std::string graphicsPipelineTag)
{
	return sGraphicsPipelineMap_[graphicsPipelineTag].get();
}
