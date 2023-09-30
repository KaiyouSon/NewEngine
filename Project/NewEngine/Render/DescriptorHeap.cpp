#include "DescriptorHeap.h"
#include "RenderBase.h"
#include <cassert>

void DescriptorHeap::Create(const DescriptorHeapSetting setting)
{
	mSetting = setting;

	switch (mSetting.heapType)
	{
	case DescriptorHeapSetting::CBV_SRV_UAV:
	{
		// �f�X�N���v�^�q�[�v�̐ݒ�
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		heapDesc.NumDescriptors = mSetting.maxSize;

		// UAV�p�f�X�N���v�^�q�[�v�̐���
		mResult = RenderBase::GetInstance()->GetDevice()->
			CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&mDescriptorHeap));
		assert(SUCCEEDED(mResult));
	}
	break;


	}
}

void DescriptorHeap::CreateSRV(BufferResource* bufferResource, const uint32_t arraySize, const uint32_t byteSize)
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
	bufferResource->cpuHandle = mDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	bufferResource->gpuHandle = mDescriptorHeap->GetGPUDescriptorHandleForHeapStart();

	// index�����炷
	bufferResource->cpuHandle.ptr += (uint32_t)(incrementSize * incrementIndex);
	bufferResource->gpuHandle.ptr += (uint32_t)(incrementSize * incrementIndex);
	bufferResource->index = incrementIndex;

	// SRV�̐ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC desc{};	// srv�ݒ�\����
	if (bufferResource->buffer->GetDesc().Format == DXGI_FORMAT_D32_FLOAT)
	{
		desc.Format = DXGI_FORMAT_R32_FLOAT;
		desc.Texture2D.MipLevels = bufferResource->buffer->GetDesc().MipLevels;
	}
	else if (bufferResource->buffer->GetDesc().Format == DXGI_FORMAT_UNKNOWN)
	{
		uint32_t zero = 0;
		desc.Format = bufferResource->buffer->GetDesc().Format;
		desc.Buffer.FirstElement = 0;				// �ŏ��̗v�f�̃C���f�b�N�X
		desc.Buffer.NumElements = arraySize;		// �o�b�t�@���̗v�f��
		desc.Buffer.StructureByteStride = byteSize;	// �v�f1�̃o�C�g��
	}
	else
	{
		desc.Format = bufferResource->buffer->GetDesc().Format;
		desc.Texture2D.MipLevels = bufferResource->buffer->GetDesc().MipLevels;
	}
	desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;

	// �n���h���̎w���ʒu��SRV���쐬
	RenderBase::GetInstance()->GetDevice()->
		CreateShaderResourceView(
			bufferResource->buffer.Get(),
			&desc,
			bufferResource->cpuHandle);
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
	bufferResource->cpuHandle = mDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	bufferResource->gpuHandle = mDescriptorHeap->GetGPUDescriptorHandleForHeapStart();

	// index�����炷
	bufferResource->cpuHandle.ptr += (uint32_t)(incrementSize * incrementIndex);
	bufferResource->gpuHandle.ptr += (uint32_t)(incrementSize * incrementIndex);
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
			bufferResource->cpuHandle);
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
		break;
	}
	}

	return incrementSize;
}
ID3D12DescriptorHeap* DescriptorHeap::GetDescriptorHeap()
{
	return mDescriptorHeap.Get();
}