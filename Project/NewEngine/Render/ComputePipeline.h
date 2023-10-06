#pragma once
#include "ShaderObject.h"
#include "RootSignature.h"
#include <memory>

struct ComputePipelineSetting
{
	// 繧ｷ繧ｧ繝繝ｼ繧ｪ繝悶ず繧ｧ繧ｯ繝・
	ShaderObject* shaderObject;

	// RootParamter髢｢騾｣
	RootSignatureSetting rootSignatureSetting;
};

class ComputePipeline
{
private:
	HRESULT mResult;
	ComputePipelineSetting mSetting;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> mPSO;
	std::unique_ptr<RootSignature> mRootSignature;	// 繝ｫ繝ｼ繝医す繧ｰ繝阪・繝√Ε繝ｼ

public:
	ComputePipeline();
	void Create(const ComputePipelineSetting& setting);
	void DrawCommand();

public:
	RootSignature* GetRootSignature();
};
