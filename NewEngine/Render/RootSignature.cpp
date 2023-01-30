#include "RootSignature.h"
#include "ShaderObject.h"
#include "RenderBase.h"
#include <d3dcompiler.h>
#include <cassert>

void RootSignature::Create()
{
	// �f�X�N���v�^�����W�̐ݒ�
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1;			// ��x�̕`��Ɏg���e�N�X�`����1���Ȃ̂łP
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;		// �e�N�X�`�����W�X�^0��
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_ROOT_PARAMETER rootParam;
	rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE; // ���
	rootParam.DescriptorTable.pDescriptorRanges = &descriptorRange;		  // �f�X�N���v�^�����W
	rootParam.DescriptorTable.NumDescriptorRanges = 1;					  // �f�X�N���v�^�����W��
	rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;			  // �S�ẴV�F�[�_���猩����

	// �e�N�X�`�����W�X�^0�ԁiSRV�j
	rootParameters.emplace_back(rootParam);

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
	rootSignatureDesc.pParameters = rootParameters.data();	// ���[�g�p�����[�^�̐擪�A�h���X
	rootSignatureDesc.NumParameters = (UINT)rootParameters.size();		// ���[�g�p�����[�^��
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	// ���[�g�V�O�l�`���̃V���A���C�Y
	auto errorBlob = ShaderObject::GetErrorBlob();
	Microsoft::WRL::ComPtr<ID3DBlob> rootSigBlob;
	result = D3D12SerializeRootSignature(
		&rootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob,
		&errorBlob);
	assert(SUCCEEDED(result));

	result = RenderBase::GetInstance()->GetDevice()->CreateRootSignature(
		0,
		rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));

	assert(SUCCEEDED(result));
}

void RootSignature::AddConstantBufferViewToRootRrameter(const size_t& number)
{
	for (int i = 0; i < number; i++)
	{
		D3D12_ROOT_PARAMETER rootParam;
		rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	// ���
		rootParam.Descriptor.ShaderRegister = (UINT)rootParameters.size();	// �萔�o�b�t�@�ԍ�
		rootParam.Descriptor.RegisterSpace = 0;						// �f�t�H���g�l
		rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	// �S�ẴV�F�[�_���猩����

		rootParameters.emplace_back(rootParam);
	}
}
