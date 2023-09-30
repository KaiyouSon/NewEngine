#include "ComputePipeline.h"
#include "RenderBase.h"

ComputePipeline::ComputePipeline() :
	mResult(HRESULT())
{
}

void ComputePipeline::Create()
{
	// RootSignature�̐���
	mRootSignature = std::make_unique<RootSignature>();
	mRootSignature->Create(mSetting.rootSignatureSetting);

	ID3D12Device* device = RenderBase::GetInstance()->GetDevice();

	// �O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_COMPUTE_PIPELINE_STATE_DESC pipelineDesc{};

	// �V�F�[�_�[�̐ݒ�
	if (mSetting.shaderObject->GetCSBlob() != nullptr)
	{
		pipelineDesc.CS = CD3DX12_SHADER_BYTECODE(mSetting.shaderObject->GetCSBlob());
	}

	// ���̑��̐ݒ�i�I�v�V�����j
	pipelineDesc.NodeMask = 0;							// �m�[�h�}�X�N�̐ݒ�
	pipelineDesc.CachedPSO.pCachedBlob = nullptr;		// �L���b�V�����ꂽPSO�ւ̃|�C���^
	pipelineDesc.CachedPSO.CachedBlobSizeInBytes = 0;	// �L���b�V�����ꂽPSO�̃T�C�Y

	// PSO�쐬
	mResult = device->CreateComputePipelineState(&pipelineDesc, IID_PPV_ARGS(&mPSO));
}

void ComputePipeline::DrawCommand()
{
}
