#pragma once
#include "ShaderObject.h"
#include "RootSignature.h"
#include "Enum.h"
#include <wrl.h>
#include <cstdint>

struct GraphicsPipelineSetting
{
	// 繝代う繝励Λ繧､繝ｳ逕滓・逕ｨ
	enum PipelineBlend
	{
		Alpha = 0b0001,
		Add = 0b0010,
		Sub = 0b0100,
		Inv = 0b1000,
	};

	// 蝪励ｊ縺､縺ｶ縺・
	enum FillMode
	{
		Solid,
		Wireframe,
	};

	// 譖ｸ縺崎ｾｼ縺ｿ
	enum RenderTargetBlendMask
	{
		WriteNone = 0,
		WriteRed = 1,
		WriteGreen = 2,
		WriteBlue = 4,
		WriteAlpha = 8,
		WriteAll = ((WriteRed | WriteGreen) | WriteBlue) | WriteAlpha,
	};

	// 逕滓・縺吶ｋ繝代う繝励Λ繧､繝ｳ縺ｮ遞ｮ鬘・
	uint8_t pipelineBlend;

	// 蝪励ｊ縺､縺ｶ縺・
	FillMode fillMode;

	// 譖ｸ縺崎ｾｼ縺ｿ
	RenderTargetBlendMask renderTargetBlendMask;

	// 繧ｫ繝ｼ繝ｪ繝ｳ繧ｰ繝｢繝ｼ繝・
	CullMode cullMode;

	// 繝医・繝ｭ繧ｸ繝ｼ繧ｿ繧､繝・
	TopologyType topologyType;

	// 繧ｷ繧ｧ繝繝ｼ繧ｪ繝悶ず繧ｧ繧ｯ繝・
	ShaderObject* shaderObject;

	// 豺ｱ蠎ｦ險ｭ螳・
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc;

	// RTV縺ｮ謨ｰ
	uint32_t rtvNum;

	// RootParamter髢｢騾｣
	RootSignatureSetting rootSignatureSetting;

	// 繧ｳ繝ｳ繧ｹ繝医Λ繧ｯ繧ｿ
	GraphicsPipelineSetting();
};

class GraphicsPipeline
{
private:
	HRESULT mResult;
	GraphicsPipelineSetting mSetting;
	std::vector<Microsoft::WRL::ComPtr<ID3D12PipelineState>> mPSOs;
	std::unique_ptr<RootSignature> mRootSignature;	// 繝ｫ繝ｼ繝医す繧ｰ繝阪・繝√Ε繝ｼ

private:
	void CreatePipelineState(const GraphicsPipelineSetting::PipelineBlend pipelineBlend);

public:
	GraphicsPipeline();
	void Create(const GraphicsPipelineSetting& setting);
	void DrawCommand(const BlendMode blendMode);

public:
	// 繧ｲ繝・ち繝ｼ
	RootSignature* GetRootSignature();
	GraphicsPipelineSetting GetSetting();
	ID3D12PipelineState* GetPSO(const BlendMode blendMode);
};
