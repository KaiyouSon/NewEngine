#pragma once
#include "Vertex.h"
#include "RenderBase.h"
#include <cassert>
#include <vector>
#include <d3d12.h>
#include <wrl.h>

template<typename T> class VertexBuffer
{
private:
	HRESULT result;

	D3D12_VERTEX_BUFFER_VIEW vbView{};						// 頂点バッファビュー
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffer;	// 頂点バッファ
	T* vertMap;

public:
	~VertexBuffer()
	{
		vertexBuffer->Unmap(0, nullptr);
	}

	// 初期化
	void Create(const std::vector<T>& vertices)
	{
		// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
		unsigned int sizeVB = static_cast<unsigned int>(sizeof(T) * vertices.size());

		// 頂点バッファの設定
		D3D12_HEAP_PROPERTIES heapProp{}; // ヒープ設定
		heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用

		// リソース設定
		D3D12_RESOURCE_DESC resDesc{};
		resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resDesc.Width = sizeVB; // 頂点データ全体のサイズ
		resDesc.Height = 1;
		resDesc.DepthOrArraySize = 1;
		resDesc.MipLevels = 1;
		resDesc.SampleDesc.Count = 1;
		resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		// 頂点バッファの生成
		result = RenderBase::GetInstance()->GetDevice()->
			CreateCommittedResource(
				&heapProp, // ヒープ設定
				D3D12_HEAP_FLAG_NONE,
				&resDesc, // リソース設定
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&vertexBuffer));
		assert(SUCCEEDED(result));

		//----------------------- 頂点バッファへのデータ転送 -----------------------//
		// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
		vertMap = nullptr;
		result = vertexBuffer->Map(0, nullptr, (void**)&vertMap);
		assert(SUCCEEDED(result));
		// 全頂点に対して
		copy(vertices.begin(), vertices.end(), vertMap);

		// 繋がりを解除
		vertexBuffer->Unmap(0, nullptr);

		// GPU仮想アドレス
		vbView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
		// 頂点バッファのサイズ
		vbView.SizeInBytes = sizeVB;
		// 頂点１つ分のデータサイズ
		vbView.StrideInBytes = sizeof(vertices[0]);
	}

	// 転送
	void TransferToBuffer(const std::vector<T>& vertices)
	{
		vertMap = nullptr;
		result = vertexBuffer->Map(0, nullptr, (void**)&vertMap);
		assert(SUCCEEDED(result));
		// 全頂点に対してコピー
		copy(vertices.begin(), vertices.end(), vertMap);
	}

public:
	// 頂点バッファビューのアドレスを取得する関数
	inline D3D12_VERTEX_BUFFER_VIEW* GetvbViewAddress() { return &vbView; }
	inline ID3D12Resource* GetVertexBuffer() { return vertexBuffer.Get(); }

};