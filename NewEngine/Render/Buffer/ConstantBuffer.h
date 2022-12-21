#pragma once
#include "RenderBase.h"
#include "ConstantBufferData.h"
#include <d3d12.h>
#include <cassert>
#include <wrl.h>

template<typename T>
class ConstantBuffer
{
public:
	Microsoft::WRL::ComPtr<ID3D12Resource> constantBuffer;//	�萔�o�b�t�@
	T* constantBufferMap;	// �}�b�s���O�p

public:
	ConstantBuffer() :constantBufferMap(nullptr) {}
	~ConstantBuffer()
	{
		constantBuffer->Unmap(0, nullptr);
	}

	void Init()
	{
		HRESULT result;

		// �q�[�v�̐ݒ�
		D3D12_HEAP_PROPERTIES cbHeapProp{};
		cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;	// GPU�ւ̓]���p
		// ���\�[�X�ݒ�
		D3D12_RESOURCE_DESC cbResourceDesc{};
		cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		cbResourceDesc.Width = (sizeof(T) + 0xff) & ~0xff; // 256�o�C�g�A���C�������g
		cbResourceDesc.Height = 1;
		cbResourceDesc.DepthOrArraySize = 1;
		cbResourceDesc.MipLevels = 1;
		cbResourceDesc.SampleDesc.Count = 1;
		cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		// �萔�o�b�t�@�̐���
		result = RenderBase::GetInstance()->GetDevice()->
			CreateCommittedResource(
				&cbHeapProp,	// �q�[�v�̐ݒ�
				D3D12_HEAP_FLAG_NONE,
				&cbResourceDesc, // ���\�[�X�̐ݒ�
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&constantBuffer));
		assert(SUCCEEDED(result));

		//constantBuffer->SetName(L"ConstantBuffer");

		// �萔�o�b�t�@�̃}�b�s���O
		result = constantBuffer->Map(0, nullptr, (void**)&constantBufferMap);	// �}�b�s���O
		assert(SUCCEEDED(result));
	}
};