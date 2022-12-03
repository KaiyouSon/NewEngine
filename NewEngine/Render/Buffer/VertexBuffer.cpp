#include "VertexBuffer.h"
#include "RenderBase.h"
#include <cassert>

//void VertexBuffer::Initialize(VertexPosNormalUv vertices[], int arrarySize)
//{
//	//this->vertices = vertices;
//	this->arrarySize = arrarySize;
//
//	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
//	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * arrarySize);
//
//	// ���_�o�b�t�@�̐ݒ�
//	D3D12_HEAP_PROPERTIES heapProp{}; // �q�[�v�ݒ�
//	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p
//	// ���\�[�X�ݒ�
//	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
//	resDesc.Width = sizeVB; // ���_�f�[�^�S�̂̃T�C�Y
//	resDesc.Height = 1;
//	resDesc.DepthOrArraySize = 1;
//	resDesc.MipLevels = 1;
//	resDesc.SampleDesc.Count = 1;
//	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
//
//	HRESULT result;
//
//	// ���_�o�b�t�@�̐���
//	result = RenderBase::GetInstance()->GetDevice()->
//		CreateCommittedResource(
//			&heapProp, // �q�[�v�ݒ�
//			D3D12_HEAP_FLAG_NONE,
//			&resDesc, // ���\�[�X�ݒ�
//			D3D12_RESOURCE_STATE_GENERIC_READ,
//			nullptr,
//			IID_PPV_ARGS(&vertBuff));
//	assert(SUCCEEDED(result));
//
//	//----------------------- ���_�o�b�t�@�ւ̃f�[�^�]�� -----------------------//
//	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
//	VertexPosNormalUv* vertMap = nullptr;
//	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
//	assert(SUCCEEDED(result));
//	// �S���_�ɑ΂���
//	for (int i = 0; i < arrarySize; i++)
//	{
//		vertMap[i] = vertices[i]; // ���W���R�s�[
//	}
//	// �q���������
//	//vertBuff->Unmap(0, nullptr);
//
//	// GPU���z�A�h���X
//	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
//	// ���_�o�b�t�@�̃T�C�Y
//	vbView.SizeInBytes = sizeVB;
//	// ���_�P���̃f�[�^�T�C�Y
//	vbView.StrideInBytes = sizeof(vertices[0]);
//}
//void VertexBuffer::Initialize(std::vector<VertexPosNormalUv> vertices)
//{
//	arrarySize = vertices.size();
//
//	//this->vertices = vertices;
//
//	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
//	unsigned int sizeVB = static_cast<unsigned int>(sizeof(VertexPosNormalUv) * vertices.size());
//
//	// ���_�o�b�t�@�̐ݒ�
//	D3D12_HEAP_PROPERTIES heapProp{}; // �q�[�v�ݒ�
//	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p
//	// ���\�[�X�ݒ�
//	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
//	resDesc.Width = sizeVB; // ���_�f�[�^�S�̂̃T�C�Y
//	resDesc.Height = 1;
//	resDesc.DepthOrArraySize = 1;
//	resDesc.MipLevels = 1;
//	resDesc.SampleDesc.Count = 1;
//	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
//
//	HRESULT result;
//
//	// ���_�o�b�t�@�̐���
//	result = RenderBase::GetInstance()->GetDevice()->
//		CreateCommittedResource(
//			&heapProp, // �q�[�v�ݒ�
//			D3D12_HEAP_FLAG_NONE,
//			&resDesc, // ���\�[�X�ݒ�
//			D3D12_RESOURCE_STATE_GENERIC_READ,
//			nullptr,
//			IID_PPV_ARGS(&vertBuff));
//	assert(SUCCEEDED(result));
//
//	//----------------------- ���_�o�b�t�@�ւ̃f�[�^�]�� -----------------------//
//	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
//	VertexPosNormalUv* vertMap = nullptr;
//	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
//	assert(SUCCEEDED(result));
//	// �S���_�ɑ΂���
//	copy(vertices.begin(), vertices.end(), vertMap);
//
//	// �q���������
//	vertBuff->Unmap(0, nullptr);
//
//	// GPU���z�A�h���X
//	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
//	// ���_�o�b�t�@�̃T�C�Y
//	vbView.SizeInBytes = sizeVB;
//	// ���_�P���̃f�[�^�T�C�Y
//	vbView.StrideInBytes = sizeof(vertices[0]);
//
//}
//void VertexBuffer::Initialize(const std::vector<VertexPosUv>& vertices)
//{
//	arrarySize = vertices.size();
//
//	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
//	unsigned int sizeVB = static_cast<unsigned int>(sizeof(VertexPosUv) * vertices.size());
//
//	// ���_�o�b�t�@�̐ݒ�
//	D3D12_HEAP_PROPERTIES heapProp{}; // �q�[�v�ݒ�
//	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p
//
//	// ���\�[�X�ݒ�
//	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
//	resDesc.Width = sizeVB; // ���_�f�[�^�S�̂̃T�C�Y
//	resDesc.Height = 1;
//	resDesc.DepthOrArraySize = 1;
//	resDesc.MipLevels = 1;
//	resDesc.SampleDesc.Count = 1;
//	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
//
//	HRESULT result;
//
//	// ���_�o�b�t�@�̐���
//	result = RenderBase::GetInstance()->GetDevice()->
//		CreateCommittedResource(
//			&heapProp, // �q�[�v�ݒ�
//			D3D12_HEAP_FLAG_NONE,
//			&resDesc, // ���\�[�X�ݒ�
//			D3D12_RESOURCE_STATE_GENERIC_READ,
//			nullptr,
//			IID_PPV_ARGS(&vertBuff));
//	assert(SUCCEEDED(result));
//
//	//----------------------- ���_�o�b�t�@�ւ̃f�[�^�]�� -----------------------//
//	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
//	VertexPosUv* vertMap = nullptr;
//	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
//	assert(SUCCEEDED(result));
//	// �S���_�ɑ΂���
//	copy(vertices.begin(), vertices.end(), vertMap);
//
//	// �q���������
//	vertBuff->Unmap(0, nullptr);
//
//	// GPU���z�A�h���X
//	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
//	// ���_�o�b�t�@�̃T�C�Y
//	vbView.SizeInBytes = sizeVB;
//	// ���_�P���̃f�[�^�T�C�Y
//	vbView.StrideInBytes = sizeof(vertices[0]);
//}
//void VertexBuffer::Initialize(const std::vector<VertexPos>& vertices)
//{
//	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
//	unsigned int sizeVB = static_cast<unsigned int>(sizeof(VertexPos) * vertices.size());
//
//	// ���_�o�b�t�@�̐ݒ�
//	D3D12_HEAP_PROPERTIES heapProp{}; // �q�[�v�ݒ�
//	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p
//	// ���\�[�X�ݒ�
//	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
//	resDesc.Width = sizeVB; // ���_�f�[�^�S�̂̃T�C�Y
//	resDesc.Height = 1;
//	resDesc.DepthOrArraySize = 1;
//	resDesc.MipLevels = 1;
//	resDesc.SampleDesc.Count = 1;
//	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
//
//	HRESULT result;
//
//	// ���_�o�b�t�@�̐���
//	result = RenderBase::GetInstance()->GetDevice()->
//		CreateCommittedResource(
//			&heapProp, // �q�[�v�ݒ�
//			D3D12_HEAP_FLAG_NONE,
//			&resDesc, // ���\�[�X�ݒ�
//			D3D12_RESOURCE_STATE_GENERIC_READ,
//			nullptr,
//			IID_PPV_ARGS(&vertBuff));
//	assert(SUCCEEDED(result));
//
//	//----------------------- ���_�o�b�t�@�ւ̃f�[�^�]�� -----------------------//
//	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
//	VertexPos* vertMap = nullptr;
//	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
//	assert(SUCCEEDED(result));
//	// �S���_�ɑ΂���
//	copy(vertices.begin(), vertices.end(), vertMap);
//
//	// �q���������
//	vertBuff->Unmap(0, nullptr);
//
//	// GPU���z�A�h���X
//	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
//	// ���_�o�b�t�@�̃T�C�Y
//	vbView.SizeInBytes = sizeVB;
//	// ���_�P���̃f�[�^�T�C�Y
//	vbView.StrideInBytes = sizeof(vertices[0]);
//}
//void VertexBuffer::Initialize(const std::vector<VertexPosScale>& vertices)
//{
//	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
//	unsigned int sizeVB = static_cast<unsigned int>(sizeof(VertexPosScale) * vertices.size());
//
//	// ���_�o�b�t�@�̐ݒ�
//	D3D12_HEAP_PROPERTIES heapProp{}; // �q�[�v�ݒ�
//	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p
//	// ���\�[�X�ݒ�
//	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
//	resDesc.Width = sizeVB; // ���_�f�[�^�S�̂̃T�C�Y
//	resDesc.Height = 1;
//	resDesc.DepthOrArraySize = 1;
//	resDesc.MipLevels = 1;
//	resDesc.SampleDesc.Count = 1;
//	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
//
//	HRESULT result;
//
//	// ���_�o�b�t�@�̐���
//	result = RenderBase::GetInstance()->GetDevice()->
//		CreateCommittedResource(
//			&heapProp, // �q�[�v�ݒ�
//			D3D12_HEAP_FLAG_NONE,
//			&resDesc, // ���\�[�X�ݒ�
//			D3D12_RESOURCE_STATE_GENERIC_READ,
//			nullptr,
//			IID_PPV_ARGS(&vertBuff));
//	assert(SUCCEEDED(result));
//
//	//----------------------- ���_�o�b�t�@�ւ̃f�[�^�]�� -----------------------//
//	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
//	VertexPosScale* vertMap = nullptr;
//	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
//	assert(SUCCEEDED(result));
//	// �S���_�ɑ΂���
//	copy(vertices.begin(), vertices.end(), vertMap);
//
//	// �q���������
//	vertBuff->Unmap(0, nullptr);
//
//	// GPU���z�A�h���X
//	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
//	// ���_�o�b�t�@�̃T�C�Y
//	vbView.SizeInBytes = sizeVB;
//	// ���_�P���̃f�[�^�T�C�Y
//	vbView.StrideInBytes = sizeof(vertices[0]);
//}
//
//void VertexBuffer::TransferToBuffer(std::vector<VertexPosNormalUv> vertices)
//{
//	HRESULT result;
//
//	VertexPosNormalUv* vertMap = nullptr;
//	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
//	assert(SUCCEEDED(result));
//	// �S���_�ɑ΂��ăR�s�[
//	copy(vertices.begin(), vertices.end(), vertMap);
//}
//void VertexBuffer::TransferToBuffer(std::vector<VertexPosUv> vertices)
//{
//	HRESULT result;
//
//	VertexPosUv* vertMap = nullptr;
//	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
//	assert(SUCCEEDED(result));
//	// �S���_�ɑ΂��ăR�s�[
//	copy(vertices.begin(), vertices.end(), vertMap);
//}
//void VertexBuffer::TransferToBuffer()
//{
//	HRESULT result;
//
//	VertexPosNormalUv* vertMap = nullptr;
//	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
//	assert(SUCCEEDED(result));
//	// �S���_�ɑ΂��ăR�s�[
//	for (int i = 0; i < arrarySize; i++)
//	{
//		vertMap[i] = vertices[i]; // ���W���R�s�[
//	}
//}
//void VertexBuffer::Unmap()
//{
//	// �q���������
//	vertBuff->Unmap(0, nullptr);
//}
