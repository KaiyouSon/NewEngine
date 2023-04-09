#include "GraphicsPipeline.h"
#include "RenderBase.h"
#include <d3dx12.h>
#include <cassert>

GraphicsPipeline::GraphicsPipeline() :
	cullMode(CullMode::None),
	topologyType(TopologyType::TriangleTopology),
	shaderObject(nullptr), rootSignature(nullptr), result(HRESULT())
{
	depthStencilDesc.DepthEnable = true; // �[�x�e�X�g���s��
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;	// �������݋���
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;	// �������ق����̗p
}

void GraphicsPipeline::Create()
{
	CreatePipelineState(BlendMode::Alpha);		// ���u�����h
	CreatePipelineState(BlendMode::Add);		// ���Z�u�����h
	CreatePipelineState(BlendMode::Sub);		// ���Z�u�����h
	CreatePipelineState(BlendMode::Inv);
}

void GraphicsPipeline::CreatePipelineState(const BlendMode& blendMode)
{
	ID3D12Device* device = RenderBase::GetInstance()->GetDevice();

	// �O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	// �V�F�[�_�[�̐ݒ�
	if (shaderObject->GetVSBlob() != nullptr)
	{
		pipelineDesc.VS = CD3DX12_SHADER_BYTECODE(shaderObject->GetVSBlob());
	}
	if (shaderObject->GetGSBlob() != nullptr)
	{
		pipelineDesc.GS = CD3DX12_SHADER_BYTECODE(shaderObject->GetGSBlob());
	}
	if (shaderObject->GetPSBlob() != nullptr)
	{
		pipelineDesc.PS = CD3DX12_SHADER_BYTECODE(shaderObject->GetPSBlob());
	}

	// �T���v���}�X�N�̐ݒ�
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�

	// ���X�^���C�U�̐ݒ�
	switch (cullMode)
	{
	case CullMode::None:	// �J�����O���Ȃ�
		pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
		break;

	case CullMode::CullFront:	// �O�ʃJ�����O
		pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_FRONT;
		break;

	case CullMode::CullBack:	// �w�ʃJ�����O
		pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
		break;

	default:
		pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
		break;
	}
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;	// �|���S�����h��Ԃ�
	pipelineDesc.RasterizerState.DepthClipEnable = true; // �[�x�N���b�s���O��L����

	// �f�v�X�X�e���V���X�e�[�g�̐ݒ�
	pipelineDesc.DepthStencilState = depthStencilDesc;
	if (depthStencilDesc.DepthEnable == (BOOL)true)
	{
		pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;	// �[�x�l�t�H�[�}�b�g
	}

	// �����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	for (int i = 0; i < rtvNum; i++)
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
	pipelineDesc.InputLayout.pInputElementDescs = shaderObject->GetInputLayout().data();
	pipelineDesc.InputLayout.NumElements = (UINT)shaderObject->GetInputLayout().size();

	// �}�`�̌`��ݒ�
	switch (topologyType)
	{
	case TopologyType::PointTopology:
		pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
		break;

	case TopologyType::LineTopology:
		pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
		break;

	case TopologyType::TriangleTopology:
		pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		break;

	default:
		break;
	}

	// ���̑��̐ݒ�
	pipelineDesc.NumRenderTargets = (UINT)rtvNum; // �`��Ώۂ�1��
	for (size_t i = 0; i < rtvNum; i++)
	{
		pipelineDesc.RTVFormats[i] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0�`255�w���RGBA
	}
	pipelineDesc.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	pipelineDesc.pRootSignature = rootSignature;

	// �p�C�v�����X�e�[�g�̐���
	switch (blendMode)
	{
	case BlendMode::Alpha: // ���u�����h
		result = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&alphaPipeline));
		break;

	case BlendMode::Add:	// ���Z�u�����h
		result = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&addPipeline));
		break;

	case BlendMode::Sub:	// ���Z�u�����h
		result = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&subPipeline));
		break;

	case BlendMode::Inv:	// ���]
		result = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&invPipeline));
		break;

	default:
		break;
	}
	assert(SUCCEEDED(result));
}