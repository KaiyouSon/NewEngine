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
	renderTargetBlendMask(WriteRGB),
	topologyType(TopologyType::TriangleList),
	shaderCompilerTag("empty"),
	depthStencilDesc(D3D12_DEPTH_STENCIL_DESC()),
	rtvNum(1),
	rootSignatureSetting(RootSignatureSetting())
{
}

DescriptorHeapSetting::DescriptorHeapSetting() :
	maxSize(64), startIndex(0), heapType(DescriptorHeapSetting::None)
{
}

//////////////////////////////////////////////////////////////////////////
/// --- RenderTextureの設定構造体 ------------------------------------ ///
//////////////////////////////////////////////////////////////////////////
RenderTextureSetting::RenderTextureSetting(Vec2 texSize) :
	texSize(texSize), depthSize(texSize), rtvNum(1), isUseDepthTexture(false)
{
}

RenderTextureSetting::RenderTextureSetting(Vec2 texSize, uint32_t rtvNum) :
	texSize(texSize), depthSize(texSize), rtvNum(rtvNum), isUseDepthTexture(false)
{
}

RenderTextureSetting::RenderTextureSetting(Vec2 texSize, Vec2 depthSize, bool isUseDepthTexture) :
	texSize(texSize), depthSize(depthSize), rtvNum(1), isUseDepthTexture(isUseDepthTexture)
{
}

RenderTextureSetting::RenderTextureSetting(Vec2 texSize, Vec2 depthSize, uint32_t rtvNum, bool isUseDepthTexture) :
	texSize(texSize), depthSize(depthSize), rtvNum(rtvNum), isUseDepthTexture(isUseDepthTexture)
{
}
