#include "IndexBuffer.h"
#include "RenderBase.h"
#include <cassert>

void IndexBuffer::Create(const std::vector<uint16_t>& indices)
{
	// ���_�f�[�^�S�̂̃T�C�Y
	uint32_t sizeIB = static_cast<uint32_t>(sizeof(uint16_t) * indices.size());

	// ���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp{}; // �q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p
	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB; // ���_�f�[�^�S�̂̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	HRESULT result;

	// �C���f�b�N�X�o�b�t�@�̐���
	result = RenderBase::GetInstance()->GetDevice()->
		CreateCommittedResource(
			&heapProp, // �q�[�v�ݒ�
			D3D12_HEAP_FLAG_NONE,
			&resDesc, // ���\�[�X�ݒ�
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&mIndexBuffer));
	assert(SUCCEEDED(result));

	//------------------- �C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]�� -------------------//
	uint16_t* indexMap = nullptr;
	result = mIndexBuffer->Map(0, nullptr, (void**)&indexMap);
	assert(SUCCEEDED(result));
	// �S���_�ɑ΂���
	for (int i = 0; i < indices.size(); i++)
	{
		indexMap[i] = indices[i]; // ���W���R�s�[
	}
	// �q���������
	mIndexBuffer->Unmap(0, nullptr);

	// �C���f�b�N�X�o�b�t�@�r���[�̍쐬
	mIndexBufferView.BufferLocation = mIndexBuffer->GetGPUVirtualAddress();
	mIndexBufferView.Format = DXGI_FORMAT_R16_UINT;
	mIndexBufferView.SizeInBytes = sizeIB;
}

D3D12_INDEX_BUFFER_VIEW* IndexBuffer::GetibViewAddress() { return &mIndexBufferView; }