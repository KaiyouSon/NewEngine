#include "DescriptorHeap.h"
#include "RenderBase.h"
#include <cassert>

DescriptorHeapSetting::DescriptorHeapSetting() :
	maxSize(64), startIndex(0), heapType(DescriptorHeapSetting::None)
{
}

void DescriptorHeap::Create(const DescriptorHeapSetting setting)
{
	mSetting = setting;

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};

	switch (mSetting.heapType)
	{
	case DescriptorHeapSetting::RTV:
	{
		// �f�X�N���v�^�q�[�v�̐ݒ�
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		heapDesc.NumDescriptors = mSetting.maxSize;
	}
	break;

	case DescriptorHeapSetting::DSV:
	{
		// �f�X�N���v�^�q�[�v�̐ݒ�
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		heapDesc.NumDescriptors = mSetting.maxSize;
	}
	break;

	case DescriptorHeapSetting::CBV_SRV_UAV:
	{
		// �f�X�N���v�^�q�[�v�̐ݒ�
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		heapDesc.NumDescriptors = mSetting.maxSize;
	}
	break;
	}

	// �f�X�N���v�^�q�[�v�̐���
	mResult = RenderBase::GetInstance()->GetDevice()->
		CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&mDescriptorHeap));
	assert(SUCCEEDED(mResult));
}

void DescriptorHeap::CreateSRV(BufferResource* bufferResource, const uint32_t arraySize, const uint32_t byteSize)
{
	if (mSetting.heapType != DescriptorHeapSetting::CBV_SRV_UAV)
	{
		return;
	}

	// �C���f�b�N�X���擾
	uint32_t incrementIndex = mSetting.startIndex + GetIncrementIndex();

	// �T�C�Y���擾
	uint32_t incrementSize = GetIncrementSize();

	// �q�[�v�̐擪�n���h�����擾
	bufferResource->srvHandle.cpu = mDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	bufferResource->srvHandle.gpu = mDescriptorHeap->GetGPUDescriptorHandleForHeapStart();

	// index�����炷
	bufferResource->srvHandle.cpu.ptr += (uint32_t)(incrementSize * incrementIndex);
	bufferResource->srvHandle.gpu.ptr += (uint32_t)(incrementSize * incrementIndex);
	bufferResource->index = incrementIndex;

	// SRV�̐ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC desc{};	// srv�ݒ�\����
	if (bufferResource->buffer->GetDesc().Format == DXGI_FORMAT_D32_FLOAT)
	{
		desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		desc.Format = DXGI_FORMAT_R32_FLOAT;
		desc.Texture2D.MipLevels = bufferResource->buffer->GetDesc().MipLevels;
	}
	else if (bufferResource->buffer->GetDesc().Format == DXGI_FORMAT_UNKNOWN)
	{
		desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
		desc.Format = bufferResource->buffer->GetDesc().Format;
		desc.Buffer.FirstElement = 0;				// �ŏ��̗v�f�̃C���f�b�N�X
		desc.Buffer.NumElements = arraySize;		// �o�b�t�@���̗v�f��
		desc.Buffer.StructureByteStride = byteSize;	// �v�f1�̃o�C�g��
	}
	else
	{
		desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		desc.Format = bufferResource->buffer->GetDesc().Format;
		desc.Texture2D.MipLevels = bufferResource->buffer->GetDesc().MipLevels;
	}
	desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	// �n���h���̎w���ʒu��SRV���쐬
	RenderBase::GetInstance()->GetDevice()->
		CreateShaderResourceView(
			bufferResource->buffer.Get(),
			&desc,
			bufferResource->srvHandle.cpu);
}

void DescriptorHeap::CreateRTV(BufferResource* bufferResource)
{
	if (mSetting.heapType != DescriptorHeapSetting::RTV)
	{
		return;
	}

	// �C���f�b�N�X���擾
	uint32_t incrementIndex = GetIncrementIndex();

	// �T�C�Y���擾
	uint32_t incrementSize = GetIncrementSize();

	// �q�[�v�̐擪�n���h�����擾
	bufferResource->rtvHandle.cpu = mDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	bufferResource->rtvHandle.gpu = mDescriptorHeap->GetGPUDescriptorHandleForHeapStart();

	// index�����炷
	bufferResource->rtvHandle.cpu.ptr += (uint32_t)(incrementSize * incrementIndex);
	bufferResource->rtvHandle.gpu.ptr += (uint32_t)(incrementSize * incrementIndex);
	bufferResource->index = incrementIndex;

	// �����_�[�^�[�Q�b�g�r���[�̐ݒ�
	D3D12_RENDER_TARGET_VIEW_DESC desc{};
	// �V�F�[�_�[�̌v�Z���ʂ�SRGB�ɕϊ����ď�������
	desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	// �n���h���̎w���ʒu��RTV���쐬
	RenderBase::GetInstance()->GetDevice()->
		CreateRenderTargetView(
			bufferResource->buffer.Get(),
			&desc,
			bufferResource->rtvHandle.cpu);
}

void DescriptorHeap::CreateUAV(BufferResource* bufferResource, const uint32_t arraySize, const uint32_t byteSize)
{
	if (mSetting.heapType != DescriptorHeapSetting::CBV_SRV_UAV)
	{
		return;
	}

	// �C���f�b�N�X���擾
	uint32_t incrementIndex = GetIncrementIndex();

	// �T�C�Y���擾
	uint32_t incrementSize = GetIncrementSize();

	// �q�[�v�̐擪�n���h�����擾
	bufferResource->uavHandle.cpu = mDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	bufferResource->uavHandle.gpu = mDescriptorHeap->GetGPUDescriptorHandleForHeapStart();

	// index�����炷
	bufferResource->uavHandle.cpu.ptr += (uint32_t)(incrementSize * incrementIndex);
	bufferResource->uavHandle.gpu.ptr += (uint32_t)(incrementSize * incrementIndex);
	bufferResource->index = incrementIndex;

	// UAV�̐ݒ�
	D3D12_UNORDERED_ACCESS_VIEW_DESC desc{};
	desc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.Buffer.FirstElement = 0;				// �ŏ��̗v�f�̃C���f�b�N�X
	desc.Buffer.NumElements = arraySize;		// �o�b�t�@���̗v�f��
	desc.Buffer.StructureByteStride = byteSize;	// �v�f1�̃o�C�g��

	// �n���h���̎w���ʒu��UAV���쐬
	RenderBase::GetInstance()->GetDevice()->
		CreateUnorderedAccessView(
			bufferResource->buffer.Get(),
			nullptr,
			&desc,
			bufferResource->uavHandle.cpu);
}

uint32_t DescriptorHeap::GetIncrementIndex()
{
	// ImGUI��0�Ԏg���Ă��邩��1����g��
	uint32_t index = 0;

	// mCheckSRVIndex�g���ĂȂ��ԍ��Ȃ������`�F�b�N
	for (uint32_t i = 0; i < mCheckIndex.size(); i++)
	{
		// i�Ԃ� false ��������
		if (mCheckIndex[i] == false)
		{
			index = i + 1;
			mCheckIndex[i] = true;
			break;
		}
	}

	if (index == 0)
	{
		// �Ō���ɐV�������ǉ�
		mCheckIndex.push_back(true);
		return (uint32_t)mCheckIndex.size();	// ������index���1��������+1���Ȃ��Ă���
	}

	return index;
}
uint32_t DescriptorHeap::GetIncrementSize()
{
	uint32_t incrementSize = 0;

	switch (mSetting.heapType)
	{
	case DescriptorHeapSetting::CBV_SRV_UAV:
	{
		incrementSize = RenderBase::GetInstance()->GetDevice()->
			GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	}
	break;

	case DescriptorHeapSetting::RTV:
	{
		incrementSize = RenderBase::GetInstance()->GetDevice()->
			GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}
	break;

	}

	return incrementSize;
}
ID3D12DescriptorHeap* DescriptorHeap::GetDescriptorHeap()
{
	return mDescriptorHeap.Get();
}