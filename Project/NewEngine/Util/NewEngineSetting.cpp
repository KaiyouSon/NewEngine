#include "NewEngineSetting.h"

InputLayoutSetting::InputLayoutSetting() :
	semanticName(std::string()),
	format(DXGI_FORMAT()),
	index(0)
{
}

InputLayoutSetting::InputLayoutSetting(
	const std::string semanticName,
	const DXGI_FORMAT format,
	const uint32_t index) :
	semanticName(semanticName),
	format(format),
	index(index)
{
}

RootSignatureSetting::RootSignatureSetting() :
	maxCbvRootParameter(1),
	maxSrvDescritorRange(1),
	maxUavDescritorRange(0)
{
}

RootSignatureSetting::RootSignatureSetting(
	const uint32_t maxCbvRootParameter,
	const uint32_t maxSrvDescritorRange,
	const uint32_t maxUavDescritorRange) :
	maxCbvRootParameter(maxCbvRootParameter),
	maxSrvDescritorRange(maxSrvDescritorRange),
	maxUavDescritorRange(maxUavDescritorRange)
{
}

GraphicsPipelineSetting::GraphicsPipelineSetting() :
	pipelineBlend(PipelineBlend::Alpha),
	fillMode(FillMode::Solid),
	cullMode(CullMode::Back),
	renderTargetBlendMask(WriteAll),
	topologyType(TopologyType::TriangleList),
	shaderObject(nullptr),
	depthStencilDesc(D3D12_DEPTH_STENCIL_DESC()),
	rtvNum(1),
	rootSignatureSetting(RootSignatureSetting())
{
}
