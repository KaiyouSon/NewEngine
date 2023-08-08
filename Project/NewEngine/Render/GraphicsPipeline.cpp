#include "GraphicsPipeline.h"
#include "RenderBase.h"
#include <d3dx12.h>
#include <cassert>

GraphicsPipelineSetting::GraphicsPipelineSetting() :
	pipelineBlend(PipelineBlend::Alpha), cullMode(CullMode::Back),
	topologyType(TopologyType::TriangleList), shaderObject(nullptr),
	depthStencilDesc(D3D12_DEPTH_STENCIL_DESC()), rtvNum(1),
	rootSignatureSetting(RootSignatureSetting())
{
}

GraphicsPipeline::GraphicsPipeline() : mResult(HRESULT())
{
	// �f�t�H���g
	mSetting.depthStencilDesc.DepthEnable = true;
	mSetting.depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;	// �������݋���
	mSetting.depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;	// �������ق����̗p
}

void GraphicsPipeline::Create()
{
	CreatePipelineState(BlendMode::Alpha);		// ���u�����h
}
void GraphicsPipeline::DrawCommand(const BlendMode blendMode)
{
	ID3D12GraphicsCommandList* cmdList = RenderBase::GetInstance()->GetCommandList();

	// RootSignature�ݒ�
	cmdList->SetGraphicsRootSignature(mRootSignature->GetRootSignature());

	// PSO�ݒ�
	if (mPSOs[(uint32_t)blendMode])
	{
		cmdList->SetPipelineState(mPSOs[(uint32_t)blendMode].Get());
	}
	else
	{
		assert(0 && "�g�p���Ă���p�C�v���C����nullptr�ł�");
	}

	// �`��̐ݒ�
	switch (mSetting.topologyType)
	{
	case TopologyType::Point:
		cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
		break;

	case TopologyType::Line:
		cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);
		break;

	case TopologyType::TriangleList:
		cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		break;

	case TopologyType::TriangleStrip:
		cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		break;

	default:
		break;
	}
}

void GraphicsPipeline::CreatePipelineState(const BlendMode blendMode)
{
	// RootSignature�̐���
	mRootSignature = std::make_unique<RootSignature>();
	mRootSignature->Create(mSetting.rootSignatureSetting);

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

	case TopologyType::TriangleList:
	case TopologyType::TriangleStrip:
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
	pipelineDesc.pRootSignature = mRootSignature->GetRootSignature();

	// �p�C�v�����X�e�[�g�̐���
	uint8_t bit = (uint8_t)mSetting.pipelineBlend;
	mPSOs.resize(4);
	for (int i = 0; i < 4; i++)
	{
		uint8_t mask = (1 << i);
		// ��������ꍇ
		if (bit & mask)
		{
			mResult = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&mPSOs[i]));
			assert(SUCCEEDED(mResult));

		}
		// ���Ȃ��ꍇ
		else
		{
			mPSOs[i] = nullptr;
		}
	}
}

// �Z�b�^�[
void GraphicsPipeline::SetGraphicsPipelineSetter(const GraphicsPipelineSetting& setting)
{
	mSetting = setting;
}

RootSignature* GraphicsPipeline::GetRootSignature()
{
	return mRootSignature.get();
}