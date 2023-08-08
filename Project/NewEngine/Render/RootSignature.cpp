#include "RootSignature.h"
#include "ShaderObject.h"
#include "RenderBase.h"
#include <d3dcompiler.h>
#include <cassert>

RootSignatureSetting::RootSignatureSetting() :
	constantBufferViewNum(1), descriptorRangeNum(1)
{
}


void RootSignature::Create(const RootSignatureSetting setting)
{
	// RootParameter��ConstantBufferView��ǉ�
	AddConstantBufferViewToRootRrameter(setting.constantBufferViewNum);

	// RootParameter��DescriptorRange��ǉ�
	AddDescriptorRangeToRootPrameter(setting.descriptorRangeNum);

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
	rootSignatureDesc.pParameters = mRootParameters.data();					// ���[�g�p�����[�^�̐擪�A�h���X
	rootSignatureDesc.NumParameters = (uint32_t)mRootParameters.size();		// ���[�g�p�����[�^��
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	// ���[�g�V�O�l�`���̃V���A���C�Y
	auto errorBlob = ShaderObject::GetErrorBlob();
	Microsoft::WRL::ComPtr<ID3DBlob> rootSigBlob;
	mResult = D3D12SerializeRootSignature(
		&rootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob,
		&errorBlob);
	assert(SUCCEEDED(mResult));

	mResult = RenderBase::GetInstance()->GetDevice()->CreateRootSignature(
		0,
		rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&mRootSignature));

	assert(SUCCEEDED(mResult));
}

void RootSignature::AddConstantBufferViewToRootRrameter(const uint32_t number)
{
	for (uint32_t i = 0; i < number; i++)
	{
		D3D12_ROOT_PARAMETER rootParam;
		rootParam.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;	// ���
		rootParam.Descriptor.ShaderRegister = (uint32_t)mRootParameters.size();	// �萔�o�b�t�@�ԍ�
		rootParam.Descriptor.RegisterSpace = 0;						// �f�t�H���g�l
		rootParam.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;	// �S�ẴV�F�[�_���猩����

		mRootParameters.emplace_back(rootParam);
		mConstantBufferNum++;
	}
}
void RootSignature::AddDescriptorRangeToRootPrameter(const uint32_t number)
{
	// �Ȃ����������Ȃ��ƃG���[�ɂȂ�
	for (uint32_t i = 0; i < number; i++)
	{
		mDescriptorRanges.emplace_back();
		mDescriptorRanges.back().Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, (uint32_t)i);
	}

	for (uint32_t i = 0; i < number; i++)
	{
		mRootParameters.emplace_back();
		mRootParameters.back().InitAsDescriptorTable(1, &mDescriptorRanges[i]);

		mDescriptorRangeNum++;
	}
}

ID3D12RootSignature* RootSignature::GetRootSignature()
{
	return mRootSignature.Get();
}
uint32_t RootSignature::GetConstantBufferStartIndex()
{
	// �K��0�Ԗڂ�����悤�ɂ��邽��0��Ԃ�
	return 0;
}
uint32_t RootSignature::GetConstantBufferEndIndex()
{
	return (uint32_t)(mRootParameters.size() - mDescriptorRangeNum);
}
uint32_t RootSignature::GetDescriptorTableStartIndex()
{
	return (uint32_t)(mRootParameters.size() - mDescriptorRangeNum);
}
uint32_t RootSignature::GetDescriptorTableEndIndex()
{
	return (uint32_t)(mRootParameters.size());
}