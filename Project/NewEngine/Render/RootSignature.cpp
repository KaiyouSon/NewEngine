#include "RootSignature.h"
#include "ShaderObject.h"
#include "RenderBase.h"
#include <d3dcompiler.h>
#include <cassert>

void RootSignature::Create(const uint32_t number)
{
	//// �f�X�N���v�^�����W�̐ݒ�
	//CD3DX12_DESCRIPTOR_RANGE descriptorRange{};
	//descriptorRange.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
	////descriptorRange.NumDescriptors = 1;			// ��x�̕`��Ɏg���e�N�X�`����1���Ȃ̂łP
	////descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	////descriptorRange.BaseShaderRegister = 0;		// �e�N�X�`�����W�X�^0��
	////descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	//
	//D3D12_ROOT_PARAMETER rootParam;
	//rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // ���
	//rootParam.DescriptorTable.pDescriptorRanges = &descriptorRange;		  // �f�X�N���v�^�����W
	//rootParam.DescriptorTable.NumDescriptorRanges = 1;					  // �f�X�N���v�^�����W��
	//rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;			  // �S�ẴV�F�[�_���猩����
	//
	//// �e�N�X�`�����W�X�^0�ԁiSRV�j
	//rootParameters.emplace_back(rootParam);

	if (number == 1)
	{
		// �f�X�N���v�^�����W�̐ݒ�
		CD3DX12_DESCRIPTOR_RANGE descriptorRange1{};
		descriptorRange1.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

		CD3DX12_ROOT_PARAMETER rootParam;
		rootParam.InitAsDescriptorTable(1, &descriptorRange1, D3D12_SHADER_VISIBILITY_ALL);
		rootParameters_.emplace_back(rootParam);

		descriptorRangeNum_ = 1;
	}
	else if (number == 2)
	{
		// �f�X�N���v�^�����W�̐ݒ�
		CD3DX12_DESCRIPTOR_RANGE descriptorRange1{};
		descriptorRange1.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

		CD3DX12_ROOT_PARAMETER rootParam1;
		rootParam1.InitAsDescriptorTable(1, &descriptorRange1, D3D12_SHADER_VISIBILITY_ALL);
		rootParameters_.emplace_back(rootParam1);

		// �f�X�N���v�^�����W�̐ݒ�
		CD3DX12_DESCRIPTOR_RANGE descriptorRange2{};
		descriptorRange2.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1);

		CD3DX12_ROOT_PARAMETER rootParam2;
		rootParam2.InitAsDescriptorTable(1, &descriptorRange2, D3D12_SHADER_VISIBILITY_ALL);
		rootParameters_.emplace_back(rootParam2);

		descriptorRangeNum_ = 2;
	}

	// �e�N�X�`���T���v���[�̐ݒ�
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					// ���J��Ԃ�(�^�C�����O)
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					// �c�J��Ԃ�(�^�C�����O)
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;					// ���s�J��Ԃ�(�^�C�����O)
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	// �{�[�_�[�̎��͍�
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;					// �S�Ẵ��j�A���
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;									// �~�b�v�}�b�v�ő�l
	samplerDesc.MinLOD = 0.0f;												// �~�b�v�}�b�v�ŏ��l
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;			// �s�N�Z���V�F�[�_����̂ݎg�p�\

	// ���[�g�V�O�l�`���̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParameters_.data();	// ���[�g�p�����[�^�̐擪�A�h���X
	rootSignatureDesc.NumParameters = (UINT)rootParameters_.size();		// ���[�g�p�����[�^��
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	// ���[�g�V�O�l�`���̃V���A���C�Y
	auto errorBlob = ShaderObject::GetErrorBlob();
	Microsoft::WRL::ComPtr<ID3DBlob> rootSigBlob;
	result_ = D3D12SerializeRootSignature(
		&rootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob,
		&errorBlob);
	assert(SUCCEEDED(result_));

	result_ = RenderBase::GetInstance()->GetDevice()->CreateRootSignature(
		0,
		rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature_));

	assert(SUCCEEDED(result_));
}

void RootSignature::AddConstantBufferViewToRootRrameter(const uint32_t number)
{
	for (uint32_t i = 0; i < number; i++)
	{
		D3D12_ROOT_PARAMETER rootParam;
		rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	// ���
		rootParam.Descriptor.ShaderRegister = (UINT)rootParameters_.size();	// �萔�o�b�t�@�ԍ�
		rootParam.Descriptor.RegisterSpace = 0;						// �f�t�H���g�l
		rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	// �S�ẴV�F�[�_���猩����

		rootParameters_.emplace_back(rootParam);
		constantBufferNum_++;
	}
}

void RootSignature::AddDescriptorRangeToRootPrameter(const uint32_t number)
{
	//for (int i = 0; i < number; i++)
	//{
	//	// �f�X�N���v�^�����W�̐ݒ�
	//	CD3DX12_DESCRIPTOR_RANGE descriptorRange{};
	//	descriptorRange.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, (UINT)descriptorRangeNum);

	//	D3D12_ROOT_PARAMETER rootParam;
	//	rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // ���
	//	rootParam.DescriptorTable.pDescriptorRanges = &descriptorRange;		  // �f�X�N���v�^�����W
	//	rootParam.DescriptorTable.NumDescriptorRanges = 1;					  // �f�X�N���v�^�����W��
	//	rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;			  // �S�ẴV�F�[�_���猩����

	//	// �e�N�X�`�����W�X�^0�ԁiSRV�j
	//	rootParameters.emplace_back(rootParam);

	//	descriptorRangeNum++;
	//}
}