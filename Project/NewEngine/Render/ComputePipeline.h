#pragma once
#include "ShaderObject.h"
#include "RootSignature.h"
#include <memory>

struct ComputePipelineSetting
{
	// シェダーオブジェクト
	ShaderObject* shaderObject;

	// RootParamter関連
	RootSignatureSetting rootSignatureSetting;
};

class ComputePipeline
{
private:
	HRESULT mResult;
	ComputePipelineSetting mSetting;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> mPSO;
	std::unique_ptr<RootSignature> mRootSignature;	// ルートシグネーチャー

public:
	ComputePipeline();
	void Create(const ComputePipelineSetting& setting);
	void DrawCommand();
};

