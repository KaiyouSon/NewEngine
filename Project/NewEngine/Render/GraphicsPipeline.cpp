#include "GraphicsPipeline.h"
#include "RenderBase.h"
#include <d3dx12.h>
#include <cassert>

GraphicsPipeline::GraphicsPipeline() :
	/*	mCullMode(CullMode::None),
		mTopologyType(TopologyType::Triangle),
		mShaderObject(nullptr), mRootSignature(nullptr), */mResult(HRESULT())
{
	// �f�t�H���g
	mSetting.depthStencilDesc.DepthEnable = true;
	mSetting.depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;	// �������݋���
	mSetting.depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;	// �������ق����̗p
}

void GraphicsPipeline::Create()
{
	CreatePipelineState(BlendMode::Alpha);		// ���u�����h
	CreatePipelineState(BlendMode::Add);		// ���Z�u�����h
	CreatePipelineState(BlendMode::Sub);		// ���Z�u�����h
	CreatePipelineState(BlendMode::Inv);
}
void GraphicsPipeline::DrawCommand(const BlendMode blendMode)
{
	ID3D12GraphicsCommandList* cmdList = RenderBase::GetInstance()->GetCommandList();

	// ���[�g�V�O�l�[�`��
	cmdList->SetGraphicsRootSignature(mSetting.rootSignature.GetRootSignature());

	// PSO�ݒ�
	if (psos[(uint32_t)blendMode])
	{
		cmdList->SetPipelineState(psos[(uint32_t)blendMode].Get());
	}
}

void GraphicsPipeline::CreatePipelineState(const BlendMode blendMode)
{
	ID3D12Device* device = RenderBase::GetInstance()->GetDevice();

	// �O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	// �V�F�[�_�[�̐ݒ�
	if (mSetting.shaderObject->GetVSBlob() != nullptr)
	{
		pipelineDesc.VS = CD3DX12_SHADER_BYTECODE(mSetting.shaderObject->GetVSBlob());
	}
	if (mSetting.shaderObject->GetGSBlob() != nullptr)
	{
		pipelineDesc.GS = CD3DX12_SHADER_BYTECODE(mSetting.shaderObject->GetGSBlob());
	}
	if (mSetting.shaderObject->GetPSBlob() != nullptr)
	{
		pipelineDesc.PS = CD3DX12_SHADER_BYTECODE(mSetting.shaderObject->GetPSBlob());
	}

	// �T���v���}�X�N�̐ݒ�
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�

	// ���X�^���C�U�̐ݒ�
	switch (mSetting.cullMode)
	{
	case CullMode::None:	// �J�����O���Ȃ�
		pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
		break;

	case CullMode::Front:	// �O�ʃJ�����O
		pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_FRONT;
		break;

	case CullMode::Back:	// �w�ʃJ�����O
		pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
		break;

	default:
		pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
		break;
	}
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;	// �|���S�����h��Ԃ�
	pipelineDesc.RasterizerState.DepthClipEnable = true; // �[�x�N���b�s���O��L����

	// �f�v�X�X�e���V���X�e�[�g�̐ݒ�
	pipelineDesc.DepthStencilState = mSetting.depthStencilDesc;
	if (mSetting.depthStencilDesc.DepthEnable == (BOOL)true)
	{
		pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;	// �[�x�l�t�H�[�}�b�g
	}

	// �����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	for (uint32_t i = 0; i < mSetting.rtvNum; i++)
	{
		D3D12_RENDER_TARGET_BLEND_DESC blendDesc{};// = pipelineDesc.BlendState.RenderTarget[i];

		blendDesc.RenderTargetWriteMask =
			D3D12_COLOR_WRITE_ENABLE_RED |
			D3D12_COLOR_WRITE_ENABLE_GREEN |
			D3D12_COLOR_WRITE_ENABLE_BLUE;
		blendDesc.BlendEnable = true;					// �u�����h��L���ɂ���
		blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;	// ���Z
		blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;		// �\�[�X�̒l��100���g��
		blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;	// �f�X�g�̒l��  0���g��

		// ����������
		switch (blendMode)
		{
		case BlendMode::Alpha: // ���u�����h
			blendDesc.BlendOp = D3D12_BLEND_OP_ADD;				// ���Z
			blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;			// �\�[�X�̃A���t�@�l
			blendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;	// 1.0f-�\�[�X�̃A���t�@�l
			break;

		case BlendMode::Add:	// ���Z�u�����h
			blendDesc.BlendOp = D3D12_BLEND_OP_ADD;		// ���Z
			blendDesc.SrcBlend = D3D12_BLEND_ONE;		// �\�[�X�̒l��100���g��
			blendDesc.DestBlend = D3D12_BLEND_ONE;		// �f�X�g�̒l��100���g��
			break;

		case BlendMode::Sub:	// ���Z�u�����h
			blendDesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;	// �f�X�g����\�[�X�����Z
			blendDesc.SrcBlend = D3D12_BLEND_ONE;				// �\�[�X�̒l��100���g��
			blendDesc.DestBlend = D3D12_BLEND_ONE;				// �f�X�g�̒l��100���g��
			break;

		case BlendMode::Inv:	// ���]
			blendDesc.BlendOp = D3D12_BLEND_OP_ADD;				// ���Z
			blendDesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;	// 1.0f-�f�X�g�J���[�̒l
			blendDesc.DestBlend = D3D12_BLEND_ZERO;				// �g��Ȃ�
			break;

		default:
			break;
		}

		pipelineDesc.BlendState.RenderTarget[i] = blendDesc;
	}

	// ���_���C�A�E�g�̐ݒ�
	pipelineDesc.InputLayout.pInputElementDescs = mSetting.shaderObject->GetInputLayout().data();
	pipelineDesc.InputLayout.NumElements = (uint32_t)mSetting.shaderObject->GetInputLayout().size();

	// �}�`�̌`��ݒ�
	switch (mSetting.topologyType)
	{
	case TopologyType::Point:
		pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
		break;

	case TopologyType::Line:
		pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
		break;

	case TopologyType::Triangle:
		pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		break;

	default:
		break;
	}

	// ���̑��̐ݒ�
	pipelineDesc.NumRenderTargets = (uint32_t)mSetting.rtvNum; // �`��Ώۂ̐�
	for (size_t i = 0; i < mSetting.rtvNum; i++)
	{
		pipelineDesc.RTVFormats[i] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0�`255�w���RGBA
	}
	pipelineDesc.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	pipelineDesc.pRootSignature = mSetting.rootSignature.GetRootSignature();

	// �p�C�v�����X�e�[�g�̐���
	switch (blendMode)
	{
	case BlendMode::Alpha: // ���u�����h
		mResult = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&mAlphaPSO));
		break;

	case BlendMode::Add:	// ���Z�u�����h
		mResult = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&mAddPSO));
		break;

	case BlendMode::Sub:	// ���Z�u�����h
		mResult = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&mSubPSO));
		break;

	case BlendMode::Inv:	// ���]
		mResult = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&mInvPSO));
		break;

	default:
		break;
	}
	assert(SUCCEEDED(mResult));
}

// �Z�b�^�[
void GraphicsPipeline::SetCullMode(const CullMode cullMode)
{
	mCullMode = cullMode;
}
void GraphicsPipeline::SetTopologyType(const TopologyType topologyType)
{
	mTopologyType = topologyType;
}
void GraphicsPipeline::SetShaderObject(ShaderObject* shaderObject)
{
	mShaderObject = shaderObject;
}
void GraphicsPipeline::SetRootSignature(ID3D12RootSignature* rootSignature)
{
	mRootSignature = rootSignature;
}
void GraphicsPipeline::SetDepthStencilDesc(const D3D12_DEPTH_STENCIL_DESC depthStencilDesc)
{
	mDepthStencilDesc = depthStencilDesc;
}
void GraphicsPipeline::SetRTVNum(const uint32_t rtvNum)
{
	mRtvNum = rtvNum;
}

void GraphicsPipeline::SetGraphicsPipelineSetter(const GraphicsPipelineSetting& setting)
{
	mSetting = setting;
}

// �Q�b�^�[
ID3D12PipelineState* GraphicsPipeline::GetAlphaPipeline() const { return mAlphaPSO.Get(); }
ID3D12PipelineState* GraphicsPipeline::GetAddPipeline() const { return mAddPSO.Get(); }
ID3D12PipelineState* GraphicsPipeline::GetSubPipeline() const { return mSubPSO.Get(); }
ID3D12PipelineState* GraphicsPipeline::GetInvPipeline() const { return mInvPSO.Get(); }