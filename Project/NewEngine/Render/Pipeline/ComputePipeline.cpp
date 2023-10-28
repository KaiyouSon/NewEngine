#include "ComputePipeline.h"
#include "RenderBase.h"

ComputePipeline::ComputePipeline() :
	mResult(HRESULT())
{
}

void ComputePipeline::Create(const ComputePipelineSetting& setting)
{
	mSetting = setting;

	// RootSignatureの生成
	mRootSignature = std::make_unique<RootSignature>();
	mRootSignature->Create(mSetting.rootSignatureSetting);

	ID3D12Device* device = RenderBase::GetInstance()->GetDevice();

	// コンピュートパイプラインの設定
	D3D12_COMPUTE_PIPELINE_STATE_DESC pipelineDesc{};

	// シェーダーオブジェクトのCS (Compute Shader) ブロブの設定
	if (mSetting.shaderObject->GetShaderBlob(ShaderType::Compute) != nullptr)
	{
		pipelineDesc.CS = CD3DX12_SHADER_BYTECODE(mSetting.shaderObject->GetShaderBlob(ShaderType::Compute));
	}

	// ノードマスクの設定
	pipelineDesc.NodeMask = 0; // ノードマスクは0に設定 (通常の使用において適切な値に設定する必要がある)

	// キャッシュされた PSO データの設定 (通常は使用しないので nullptr)
	pipelineDesc.CachedPSO.pCachedBlob = nullptr;
	pipelineDesc.CachedPSO.CachedBlobSizeInBytes = 0;

	// ルートシグネチャの設定
	pipelineDesc.pRootSignature = mRootSignature->GetRootSignature();

	// PSO (パイプラインステートオブジェクト) の生成
	mResult = device->CreateComputePipelineState(&pipelineDesc, IID_PPV_ARGS(&mPSO));
}

void ComputePipeline::DrawCommand()
{
	auto* cmdList = RenderBase::GetInstance()->GetCommandList();

	// コンピュートルートシグネチャの設定
	cmdList->SetComputeRootSignature(mRootSignature->GetRootSignature());

	// PSO (パイプラインステートオブジェクト) の設定
	cmdList->SetPipelineState(mPSO.Get());
}

RootSignature* ComputePipeline::GetRootSignature()
{
	return mRootSignature.get();
}

ComputePipelineSetting ComputePipeline::GetSetting()
{
	return mSetting;
}