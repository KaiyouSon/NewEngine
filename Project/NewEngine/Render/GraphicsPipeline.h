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
