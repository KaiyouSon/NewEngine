#pragma once
#include "ShaderObject.h"
#include "RootSignature.h"
#include <memory>

struct ComputePipelineSetting
{
	// �V�F�_�[�I�u�W�F�N�g
	ShaderObject* shaderObject;

	// RootParamter�֘A
	RootSignatureSetting rootSignatureSetting;
};

class ComputePipeline
{
private:
	HRESULT mResult;
	ComputePipelineSetting mSetting;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> mPSO;
	std::unique_ptr<RootSignature> mRootSignature;	// ���[�g�V�O�l�[�`���[

public:
	ComputePipeline();
	void Create(const ComputePipelineSetting& setting);
	void DrawCommand();
};

