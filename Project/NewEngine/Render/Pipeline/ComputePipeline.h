#pragma once
#include "NewEngineSetting.h"
#include "RootSignature.h"
#include <memory>

// ComputePipelineのクラス
class ComputePipeline
{
private:
	HRESULT mResult;
	ComputePipelineSetting mSetting;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> mPSO;
	std::unique_ptr<RootSignature> mRootSignature;	// ルートシグネチャー

public:
	ComputePipeline();
	void Create(const ComputePipelineSetting& setting);
	void DrawCommand();

public:
	RootSignature* GetRootSignature();
	ComputePipelineSetting GetSetting();
};
