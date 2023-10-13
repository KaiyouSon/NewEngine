#include "ComputePipeline.h"
#include "RenderBase.h"

ComputePipeline::ComputePipeline() :
	mResult(HRESULT())
{
}

void ComputePipeline::Create(const ComputePipelineSetting& setting)
{
	mSetting = setting;

	// RootSignature縺ｮ逕滓・
	mRootSignature = std::make_unique<RootSignature>();
	mRootSignature->Create(mSetting.rootSignatureSetting);

	ID3D12Device* device = RenderBase::GetInstance()->GetDevice();

	// 繧ｰ繝ｩ繝輔ぅ繝・け繧ｹ繝代う繝励Λ繧､繝ｳ險ｭ螳・
	D3D12_COMPUTE_PIPELINE_STATE_DESC pipelineDesc{};

	// 繧ｷ繧ｧ繝ｼ繝繝ｼ縺ｮ險ｭ螳・
	if (mSetting.shaderObject->GetCSBlob() != nullptr)
	{
		pipelineDesc.CS = CD3DX12_SHADER_BYTECODE(mSetting.shaderObject->GetCSBlob());
	}

	// 縺昴・莉悶・險ｭ螳夲ｼ医が繝励す繝ｧ繝ｳ・・
	pipelineDesc.NodeMask = 0;							// 繝弱・繝峨・繧ｹ繧ｯ縺ｮ險ｭ螳・
	pipelineDesc.CachedPSO.pCachedBlob = nullptr;		// 繧ｭ繝｣繝・す繝･縺輔ｌ縺蘖SO縺ｸ縺ｮ繝昴う繝ｳ繧ｿ
	pipelineDesc.CachedPSO.CachedBlobSizeInBytes = 0;	// 繧ｭ繝｣繝・す繝･縺輔ｌ縺蘖SO縺ｮ繧ｵ繧､繧ｺ

	pipelineDesc.pRootSignature = mRootSignature->GetRootSignature();

	// PSO菴懈・
	mResult = device->CreateComputePipelineState(&pipelineDesc, IID_PPV_ARGS(&mPSO));
}

void ComputePipeline::DrawCommand()
{
	auto* cmdList = RenderBase::GetInstance()->GetCommandList();

	// 繝ｫ繝ｼ繝医す繧ｰ繝阪・繝√Ε
	cmdList->SetComputeRootSignature(mRootSignature->GetRootSignature());

	// PSO
	cmdList->SetPipelineState(mPSO.Get());
}

RootSignature* ComputePipeline::GetRootSignature()
{
	return mRootSignature.get();
}

