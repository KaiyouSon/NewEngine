#pragma once
#include "VertexBufferData.h"
#include "RenderBase.h"
#include <cassert>
#include <vector>
#include <d3d12.h>
#include <wrl.h>

template<typename T> class VertexBuffer
{
private:
	HRESULT result;

	D3D12_VERTEX_BUFFER_VIEW vbView{};						// ���_�o�b�t�@�r���[
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffer;	// ���_�o�b�t�@
	T* vertMap;

public:
	// ������
	void Create(const std::vector<T>& vertices)
	{
		// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
		unsigned int sizeVB = static_cast<unsigned int>(sizeof(T) * vertices.size());

		// ���_�o�b�t�@�̐ݒ�
		D3D12_HEAP_PROPERTIES heapProp{}; // �q�[�v�ݒ�
		heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p

		// ���\�[�X�ݒ�
		D3D12_RESOURCE_DESC resDesc{};
		resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resDesc.Width = sizeVB; // ���_�f�[�^�S�̂̃T�C�Y
		resDesc.Height = 1;
		resDesc.DepthOrArraySize = 1;
		resDesc.MipLevels = 1;
		resDesc.SampleDesc.Count = 1;
		resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		// ���_�o�b�t�@�̐���
		result = RenderBase::GetInstance()->GetDevice()->
			CreateCommittedResource(
				&heapProp, // �q�[�v�ݒ�
				D3D12_HEAP_FLAG_NONE,
				&resDesc, // ���\�[�X�ݒ�
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&vertexBuffer));
		assert(SUCCEEDED(result));

		//----------------------- ���_�o�b�t�@�ւ̃f�[�^�]�� -----------------------//
		// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
		vertMap = nullptr;
		result = vertexBuffer->Map(0, nullptr, (void**)&vertMap);
		assert(SUCCEEDED(result));
		// �S���_�ɑ΂���
		copy(vertices.begin(), vertices.end(), vertMap);

		// GPU���z�A�h���X
		vbView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
		// ���_�o�b�t�@�̃T�C�Y
		vbView.SizeInBytes = sizeVB;
		// ���_�P���̃f�[�^�T�C�Y
		vbView.StrideInBytes = sizeof(vertices[0]);
	}

	// �]��
	void TransferToBuffer(const std::vector<T>& vertices)
	{
		// �S���_�ɑ΂��ăR�s�[
		copy(vertices.begin(), vertices.end(), vertMap);
	}

public:
	// ���_�o�b�t�@�r���[�̃A�h���X���擾����֐�
	inline D3D12_VERTEX_BUFFER_VIEW* GetvbViewAddress() { return &vbView; }
	inline ID3D12Resource* GetVertexBuffer() { return vertexBuffer.Get(); }

};