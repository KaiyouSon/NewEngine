#pragma once
#include <vector>
#include <d3d12.h>
#include <stdint.h>
#include <wrl.h>

class IndexBuffer
{
private:
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView{};
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff;

public:
	void Initialize(uint16_t indices[], const int& arrarySize);
	void Initialize(std::vector<unsigned short> indices);
public:
	// インデックスバッファビューを取得する関数
	D3D12_INDEX_BUFFER_VIEW* GetibViewAddress();

};

