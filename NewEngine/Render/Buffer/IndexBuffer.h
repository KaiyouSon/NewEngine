#pragma once
#include <vector>
#include <d3d12.h>
#include <wrl.h>

class IndexBuffer
{
private:
	// �C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView{};
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff;

public:
	void Create(std::vector<unsigned short> indices);

public:
	// �C���f�b�N�X�o�b�t�@�r���[���擾����֐�
	inline D3D12_INDEX_BUFFER_VIEW* GetibViewAddress() { return &ibView; }
};

