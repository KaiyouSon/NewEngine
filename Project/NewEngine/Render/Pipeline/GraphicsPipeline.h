#pragma once
#include "RootSignature.h"
#include <wrl.h>
#include <cstdint>

class Material;

class GraphicsPipeline
{
private:
	HRESULT mResult;
	GraphicsPipelineSetting mSetting;
	std::vector<Microsoft::WRL::ComPtr<ID3D12PipelineState>> mPSOs;
	std::unique_ptr<RootSignature> mRootSignature;	// ルートシグネチャー
	std::string tag;

private:
	void CreatePipelineState(const GraphicsPipelineSetting::RenderTargetBlendMode renderTargetBlendMode);

private:
	void ShaderSetting(D3D12_GRAPHICS_PIPELINE_STATE_DESC& pipelineDesc);
	void CullSetting(D3D12_GRAPHICS_PIPELINE_STATE_DESC& pipelineDesc);
	void FillSetting(D3D12_GRAPHICS_PIPELINE_STATE_DESC& pipelineDesc);
	void InputLayoutSetting(D3D12_GRAPHICS_PIPELINE_STATE_DESC& pipelineDesc);
	void RenderTargetBlendSetting(D3D12_GRAPHICS_PIPELINE_STATE_DESC& pipelineDesc);
	void PrimitiveTopologySetting(D3D12_GRAPHICS_PIPELINE_STATE_DESC& pipelineDesc);

public:
	GraphicsPipeline();
	GraphicsPipeline(const GraphicsPipelineSetting& setting);
	void Create(const Material& material);
	void DrawCommand(const BlendMode blendMode);

public: // ゲッター
	RootSignature* GetRootSignature();
	GraphicsPipelineSetting GetSetting();
	ID3D12PipelineState* GetPSO(const BlendMode blendMode);

};
