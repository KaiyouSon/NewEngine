#pragma once
#include "RootSignature.h"
#include <wrl.h>
#include <cstdint>

class GraphicsPipeline
{
private:
	HRESULT mResult;
	GraphicsPipelineSetting mSetting;
	std::vector<Microsoft::WRL::ComPtr<ID3D12PipelineState>> mPSOs;
	std::unique_ptr<RootSignature> mRootSignature;	// ルートシグネチャー

private:
	void CreatePipelineState(const GraphicsPipelineSetting::PipelineBlend pipelineBlend);

public:
	GraphicsPipeline();
	void Create(const GraphicsPipelineSetting& setting);
	void DrawCommand(const BlendMode blendMode);

public: // ゲッター
	RootSignature* GetRootSignature();
	GraphicsPipelineSetting GetSetting();
	ID3D12PipelineState* GetPSO(const BlendMode blendMode);
};
