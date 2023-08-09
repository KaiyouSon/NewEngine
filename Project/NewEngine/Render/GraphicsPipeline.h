#pragma once
#include "ShaderObject.h"
#include "RootSignature.h"
#include "Enum.h"
#include <wrl.h>
#include <cstdint>

struct GraphicsPipelineSetting
{
	// パイプライン生成用
	enum PipelineBlend
	{
		Alpha = 0b0001,
		Add = 0b0010,
		Sub = 0b0100,
		Inv = 0b1000,
	};

	// 生成するパイプラインの種類
	uint8_t pipelineBlend;

	// カーリングモード
	CullMode cullMode;

	// トポロジータイプ
	TopologyType topologyType;

	// シェダーオブジェクト
	ShaderObject* shaderObject;

	// 深度設定
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc;

	// RTVの数
	uint32_t rtvNum;

	// RootParamter関連
	RootSignatureSetting rootSignatureSetting;

	// コンストラクタ
	GraphicsPipelineSetting();
};

class GraphicsPipeline
{
private:
	HRESULT mResult;
	GraphicsPipelineSetting mSetting;
	std::vector<Microsoft::WRL::ComPtr<ID3D12PipelineState>> mPSOs;
	std::unique_ptr<RootSignature> mRootSignature;	// ルートシグネーチャー

private:
	void CreatePipelineState(const GraphicsPipelineSetting::PipelineBlend pipelineBlend);

public:
	GraphicsPipeline();
	void Create();
	void DrawCommand(const BlendMode blendMode);

public:
	// セッター
	void SetGraphicsPipelineSetter(const GraphicsPipelineSetting& setting);

public:
	// ゲッター
	RootSignature* GetRootSignature();
	GraphicsPipelineSetting GetSetting();
};

