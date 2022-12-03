#include "ConstantBuffer.h"
#include "RenderBase.h"
#include <cassert>
#include <d3dx12.h>
using namespace Microsoft::WRL;

void ConstantBuffer::MaterialBufferInit()
{
	// �q�[�v�̐ݒ�
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	// GPU�ւ̓]���p
	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDateMaterial) + 0xff) & ~0xff; // 256�o�C�g�A���C�������g
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	HRESULT result;

	// �萔�o�b�t�@�̐���
	result = RenderBase::GetInstance()->GetDevice()->
		CreateCommittedResource(
			&cbHeapProp,	// �q�[�v�̐ݒ�
			D3D12_HEAP_FLAG_NONE,
			&cbResourceDesc, // ���\�[�X�̐ݒ�
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuffMaterial));
	assert(SUCCEEDED(result));

	// �萔�o�b�t�@�̃}�b�s���O
	result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial);	// �}�b�s���O
	assert(SUCCEEDED(result));

	// �l���������ނƎ����I�ɓ]�������
	constMapMaterial->color = { 1.0f, 1.0f, 1.0f, 1.0f };

	constBuffMaterial->Unmap(0, nullptr);
}

void ConstantBuffer::MaterialBufferInit(const Material& material)
{
	HRESULT result;

	// �萔�o�b�t�@�̐���
	auto heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto resourceDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB1) + 0xff) & ~0xff); // ���\�[�X�̐ݒ�
	result = RenderBase::GetInstance()->GetDevice()->
		CreateCommittedResource(
			&heapProperties,
			D3D12_HEAP_FLAG_NONE,
			&resourceDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuffMaterial));
	assert(SUCCEEDED(result));

	// �萔�o�b�t�@�փf�[�^�]��
	result = constBuffMaterial->Map(0, nullptr, (void**)&constMapModelMaterial);
	constMapModelMaterial->ambient = material.ambient;
	constMapModelMaterial->diffuse = material.diffuse;
	constMapModelMaterial->specular = material.specular;
	constMapModelMaterial->alpha = material.alpha;
	constBuffMaterial->Unmap(0, nullptr);
}

void ConstantBuffer::TransformBufferInit()
{
	// �q�[�v�̐ݒ�
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	// GPU�ւ̓]���p
	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff; // 256�o�C�g�A���C�������g
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	HRESULT result;

	// �萔�o�b�t�@�̐���
	result = RenderBase::GetInstance()->GetDevice()->
		CreateCommittedResource(
			&cbHeapProp,	// �q�[�v�̐ݒ�
			D3D12_HEAP_FLAG_NONE,
			&cbResourceDesc, // ���\�[�X�̐ݒ�
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuffTransform));
	assert(SUCCEEDED(result));

	// �萔�o�b�t�@�̃}�b�s���O
	result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform);	// �}�b�s���O
	assert(SUCCEEDED(result));
}

ComPtr<ID3D12Resource> ConstantBuffer::GetConstBuffMaterial()
{
	return constBuffMaterial;
}
ComPtr<ID3D12Resource> ConstantBuffer::GetConstBuffTransform()
{
	return constBuffTransform;
}

void ConstantBuffer::SetColor(const Color& color)
{
	//if (constMapMaterial == nullptr) return;

	HRESULT result;

	// �萔�o�b�t�@�̃}�b�s���O
	result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial);	// �}�b�s���O
	assert(SUCCEEDED(result));

	// �l���������ނƎ����I�ɓ]�������
	constMapMaterial->color =
	{
		color.r / 255,
		color.g / 255,
		color.b / 255,
		color.a / 255
	};

	constBuffMaterial->Unmap(0, nullptr);
}
