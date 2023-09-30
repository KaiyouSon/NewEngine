#include "ComputePipeline.h"
#include "RenderBase.h"

ComputePipeline::ComputePipeline() :
	mResult(HRESULT())
{
}

void ComputePipeline::Create()
{
	// RootSignatureの生成
	mRootSignature = std::make_unique<RootSignature>();
	mRootSignature->Create(mSetting.rootSignatureSetting);

	ID3D12Device* device = RenderBase::GetInstance()->GetDevice();

	// グラフィックスパイプライン設定
	D3D12_COMPUTE_PIPELINE_STATE_DESC pipelineDesc{};

	// シェーダーの設定
	if (mSetting.shaderObject->GetCSBlob() != nullptr)
	{
		pipelineDesc.CS = CD3DX12_SHADER_BYTECODE(mSetting.shaderObject->GetCSBlob());
	}

	// その他の設定（オプション）
	pipelineDesc.NodeMask = 0;							// ノードマスクの設定
	pipelineDesc.CachedPSO.pCachedBlob = nullptr;		// キャッシュされたPSOへのポインタ
	pipelineDesc.CachedPSO.CachedBlobSizeInBytes = 0;	// キャッシュされたPSOのサイズ

	// PSO作成
	mResult = device->CreateComputePipelineState(&pipelineDesc, IID_PPV_ARGS(&mPSO));
}

void ComputePipeline::DrawCommand()
{
}
