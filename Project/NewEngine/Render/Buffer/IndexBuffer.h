#pragma once
#include <vector>
#include <d3d12.h>
#include <wrl.h>
#include <cstdint>

class IndexBuffer
{
private:
	// �C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW mIndexBufferView;
	Microsoft::WRL::ComPtr<ID3D12Resource> mIndexBuffer;

public:
	void Create(const std::vector<uint16_t>& indices);

public:
	// �C���f�b�N�X�o�b�t�@�r���[���擾����֐�
	D3D12_INDEX_BUFFER_VIEW* GetibViewAddress();
};

