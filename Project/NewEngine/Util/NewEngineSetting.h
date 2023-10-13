#pragma once
#include "NewEngineEnum.h"
#include <d3d12.h>
#include <string>
#include <vector>

// 前方宣言
class ShaderCompiler;

// インプットレイアウトの設定構造体
struct InputLayoutSetting
{
	std::string semanticName;
	DXGI_FORMAT format;
	uint32_t index;

	InputLayoutSetting();
	InputLayoutSetting(
		const std::string semanticName,
		const DXGI_FORMAT format,
		const uint32_t index = 0);
};

// シェーダーコンパイラーの設定構造体
struct ShaderCompilerSetting
{
	std::string csFilePath;
	std::string vsFilePath;
	std::string gsFilePath;
	std::string psFilePath;
	std::vector<InputLayoutSetting> mInputLayoutSettings;
};

// ルートシグネーチャの設定構造体
struct RootSignatureSetting
{
	uint32_t maxCbvRootParameter;
	uint32_t maxSrvDescritorRange;
	uint32_t maxUavDescritorRange;
	RootSignatureSetting();
	RootSignatureSetting(
		const uint32_t maxCbvRootParameter,
		const uint32_t maxSrvDescritorRange,
		const uint32_t maxUavDescritorRange);
};

// グラフィックスパイプラインの設定構造体
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
	ShaderCompiler* shaderObject;

	// 豺ｱ蠎ｦ險ｭ螳・
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc;

	// RTV縺ｮ謨ｰ
	uint32_t rtvNum;

	// RootParamter髢｢騾｣
	RootSignatureSetting rootSignatureSetting;

	// 繧ｳ繝ｳ繧ｹ繝医Λ繧ｯ繧ｿ
	GraphicsPipelineSetting();
};