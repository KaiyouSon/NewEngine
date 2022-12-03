#pragma once
#include <vector>
#include <d3d12.h>
#include <stdint.h>
#include <wrl.h>

class IndexBuffer
{
private:
	// �C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView{};
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff;

public:
	void Initialize(uint16_t indices[], const int& arrarySize);
	void Initialize(std::vector<unsigned short> indices);
public:
	// �C���f�b�N�X�o�b�t�@�r���[���擾����֐�
	D3D12_INDEX_BUFFER_VIEW* GetibViewAddress();

};

