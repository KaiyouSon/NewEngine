#pragma once
#include "RootSignature.h"
#include <wrl.h>
#include <cstdint>

class GraphicsPipeline
{
private:
	HRESULT mResult;
	GraphicsPipelineSetting mSetting;
	ShaderCompiler* shaderCompiler;
	std::vector<Microsoft::WRL::ComPtr<ID3D12PipelineState>> mPSOs;
	std::unique_ptr<RootSignature> mRootSignature;	// ルートシグネチャー

private:
	void CreatePipelineState(const GraphicsPipelineSetting::PipelineBlend pipelineBlend);

private:
	void ShaderSetting(D3D12_GRAPHICS_PIPELINE_STATE_DESC& pipelineDesc);
	void CullSetting(D3D12_GRAPHICS_PIPELINE_STATE_DESC& pipelineDesc);
	void FillSetting(D3D12_GRAPHICS_PIPELINE_STATE_DESC& pipelineDesc);
	void InputLayoutSetting(D3D12_GRAPHICS_PIPELINE_STATE_DESC& pipelineDesc);
	void RenderTargetBlendSetting(
		const GraphicsPipelineSetting::PipelineBlend pipelineBlend,
		D3D12_GRAPHICS_PIPELINE_STATE_DESC& pipelineDesc);
	void PrimitiveTopologySetting(D3D12_GRAPHICS_PIPELINE_STATE_DESC& pipelineDesc);

public:
	GraphicsPipeline();
	GraphicsPipeline(const GraphicsPipelineSetting& setting);
	void Create(const GraphicsPipelineSetting& setting);
	void DrawCommand(const BlendMode blendMode);

public: // ゲッター
	RootSignature* GetRootSignature();
	GraphicsPipelineSetting GetSetting();
	ID3D12PipelineState* GetPSO(const BlendMode blendMode);
};
