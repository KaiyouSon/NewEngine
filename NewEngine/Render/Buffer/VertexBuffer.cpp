#include "VertexBuffer.h"
#include "RenderBase.h"
#include <cassert>

//void VertexBuffer::Initialize(VertexPosNormalUv vertices[], int arrarySize)
//{
//	//this->vertices = vertices;
//	this->arrarySize = arrarySize;
//
//	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
//	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * arrarySize);
//
//	// 頂点バッファの設定
//	D3D12_HEAP_PROPERTIES heapProp{}; // ヒープ設定
//	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用
//	// リソース設定
//	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
//	resDesc.Width = sizeVB; // 頂点データ全体のサイズ
//	resDesc.Height = 1;
//	resDesc.DepthOrArraySize = 1;
//	resDesc.MipLevels = 1;
//	resDesc.SampleDesc.Count = 1;
//	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
//
//	HRESULT result;
//
//	// 頂点バッファの生成
//	result = RenderBase::GetInstance()->GetDevice()->
//		CreateCommittedResource(
//			&heapProp, // ヒープ設定
//			D3D12_HEAP_FLAG_NONE,
//			&resDesc, // リソース設定
//			D3D12_RESOURCE_STATE_GENERIC_READ,
//			nullptr,
//			IID_PPV_ARGS(&vertBuff));
//	assert(SUCCEEDED(result));
//
//	//----------------------- 頂点バッファへのデータ転送 -----------------------//
//	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
//	VertexPosNormalUv* vertMap = nullptr;
//	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
//	assert(SUCCEEDED(result));
//	// 全頂点に対して
//	for (int i = 0; i < arrarySize; i++)
//	{
//		vertMap[i] = vertices[i]; // 座標をコピー
//	}
//	// 繋がりを解除
//	//vertBuff->Unmap(0, nullptr);
//
//	// GPU仮想アドレス
//	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
//	// 頂点バッファのサイズ
//	vbView.SizeInBytes = sizeVB;
//	// 頂点１つ分のデータサイズ
//	vbView.StrideInBytes = sizeof(vertices[0]);
//}
//void VertexBuffer::Initialize(std::vector<VertexPosNormalUv> vertices)
//{
//	arrarySize = vertices.size();
//
//	//this->vertices = vertices;
//
//	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
//	unsigned int sizeVB = static_cast<unsigned int>(sizeof(VertexPosNormalUv) * vertices.size());
//
//	// 頂点バッファの設定
//	D3D12_HEAP_PROPERTIES heapProp{}; // ヒープ設定
//	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用
//	// リソース設定
//	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
//	resDesc.Width = sizeVB; // 頂点データ全体のサイズ
//	resDesc.Height = 1;
//	resDesc.DepthOrArraySize = 1;
//	resDesc.MipLevels = 1;
//	resDesc.SampleDesc.Count = 1;
//	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
//
//	HRESULT result;
//
//	// 頂点バッファの生成
//	result = RenderBase::GetInstance()->GetDevice()->
//		CreateCommittedResource(
//			&heapProp, // ヒープ設定
//			D3D12_HEAP_FLAG_NONE,
//			&resDesc, // リソース設定
//			D3D12_RESOURCE_STATE_GENERIC_READ,
//			nullptr,
//			IID_PPV_ARGS(&vertBuff));
//	assert(SUCCEEDED(result));
//
//	//----------------------- 頂点バッファへのデータ転送 -----------------------//
//	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
//	VertexPosNormalUv* vertMap = nullptr;
//	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
//	assert(SUCCEEDED(result));
//	// 全頂点に対して
//	copy(vertices.begin(), vertices.end(), vertMap);
//
//	// 繋がりを解除
//	vertBuff->Unmap(0, nullptr);
//
//	// GPU仮想アドレス
//	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
//	// 頂点バッファのサイズ
//	vbView.SizeInBytes = sizeVB;
//	// 頂点１つ分のデータサイズ
//	vbView.StrideInBytes = sizeof(vertices[0]);
//
//}
//void VertexBuffer::Initialize(const std::vector<VertexPosUv>& vertices)
//{
//	arrarySize = vertices.size();
//
//	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
//	unsigned int sizeVB = static_cast<unsigned int>(sizeof(VertexPosUv) * vertices.size());
//
//	// 頂点バッファの設定
//	D3D12_HEAP_PROPERTIES heapProp{}; // ヒープ設定
//	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用
//
//	// リソース設定
//	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
//	resDesc.Width = sizeVB; // 頂点データ全体のサイズ
//	resDesc.Height = 1;
//	resDesc.DepthOrArraySize = 1;
//	resDesc.MipLevels = 1;
//	resDesc.SampleDesc.Count = 1;
//	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
//
//	HRESULT result;
//
//	// 頂点バッファの生成
//	result = RenderBase::GetInstance()->GetDevice()->
//		CreateCommittedResource(
//			&heapProp, // ヒープ設定
//			D3D12_HEAP_FLAG_NONE,
//			&resDesc, // リソース設定
//			D3D12_RESOURCE_STATE_GENERIC_READ,
//			nullptr,
//			IID_PPV_ARGS(&vertBuff));
//	assert(SUCCEEDED(result));
//
//	//----------------------- 頂点バッファへのデータ転送 -----------------------//
//	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
//	VertexPosUv* vertMap = nullptr;
//	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
//	assert(SUCCEEDED(result));
//	// 全頂点に対して
//	copy(vertices.begin(), vertices.end(), vertMap);
//
//	// 繋がりを解除
//	vertBuff->Unmap(0, nullptr);
//
//	// GPU仮想アドレス
//	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
//	// 頂点バッファのサイズ
//	vbView.SizeInBytes = sizeVB;
//	// 頂点１つ分のデータサイズ
//	vbView.StrideInBytes = sizeof(vertices[0]);
//}
//void VertexBuffer::Initialize(const std::vector<VertexPos>& vertices)
//{
//	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
//	unsigned int sizeVB = static_cast<unsigned int>(sizeof(VertexPos) * vertices.size());
//
//	// 頂点バッファの設定
//	D3D12_HEAP_PROPERTIES heapProp{}; // ヒープ設定
//	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用
//	// リソース設定
//	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
//	resDesc.Width = sizeVB; // 頂点データ全体のサイズ
//	resDesc.Height = 1;
//	resDesc.DepthOrArraySize = 1;
//	resDesc.MipLevels = 1;
//	resDesc.SampleDesc.Count = 1;
//	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
//
//	HRESULT result;
//
//	// 頂点バッファの生成
//	result = RenderBase::GetInstance()->GetDevice()->
//		CreateCommittedResource(
//			&heapProp, // ヒープ設定
//			D3D12_HEAP_FLAG_NONE,
//			&resDesc, // リソース設定
//			D3D12_RESOURCE_STATE_GENERIC_READ,
//			nullptr,
//			IID_PPV_ARGS(&vertBuff));
//	assert(SUCCEEDED(result));
//
//	//----------------------- 頂点バッファへのデータ転送 -----------------------//
//	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
//	VertexPos* vertMap = nullptr;
//	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
//	assert(SUCCEEDED(result));
//	// 全頂点に対して
//	copy(vertices.begin(), vertices.end(), vertMap);
//
//	// 繋がりを解除
//	vertBuff->Unmap(0, nullptr);
//
//	// GPU仮想アドレス
//	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
//	// 頂点バッファのサイズ
//	vbView.SizeInBytes = sizeVB;
//	// 頂点１つ分のデータサイズ
//	vbView.StrideInBytes = sizeof(vertices[0]);
//}
//void VertexBuffer::Initialize(const std::vector<VertexPosScale>& vertices)
//{
//	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
//	unsigned int sizeVB = static_cast<unsigned int>(sizeof(VertexPosScale) * vertices.size());
//
//	// 頂点バッファの設定
//	D3D12_HEAP_PROPERTIES heapProp{}; // ヒープ設定
//	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用
//	// リソース設定
//	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
//	resDesc.Width = sizeVB; // 頂点データ全体のサイズ
//	resDesc.Height = 1;
//	resDesc.DepthOrArraySize = 1;
//	resDesc.MipLevels = 1;
//	resDesc.SampleDesc.Count = 1;
//	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
//
//	HRESULT result;
//
//	// 頂点バッファの生成
//	result = RenderBase::GetInstance()->GetDevice()->
//		CreateCommittedResource(
//			&heapProp, // ヒープ設定
//			D3D12_HEAP_FLAG_NONE,
//			&resDesc, // リソース設定
//			D3D12_RESOURCE_STATE_GENERIC_READ,
//			nullptr,
//			IID_PPV_ARGS(&vertBuff));
//	assert(SUCCEEDED(result));
//
//	//----------------------- 頂点バッファへのデータ転送 -----------------------//
//	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
//	VertexPosScale* vertMap = nullptr;
//	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
//	assert(SUCCEEDED(result));
//	// 全頂点に対して
//	copy(vertices.begin(), vertices.end(), vertMap);
//
//	// 繋がりを解除
//	vertBuff->Unmap(0, nullptr);
//
//	// GPU仮想アドレス
//	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
//	// 頂点バッファのサイズ
//	vbView.SizeInBytes = sizeVB;
//	// 頂点１つ分のデータサイズ
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
//	// 全頂点に対してコピー
//	copy(vertices.begin(), vertices.end(), vertMap);
//}
//void VertexBuffer::TransferToBuffer(std::vector<VertexPosUv> vertices)
//{
//	HRESULT result;
//
//	VertexPosUv* vertMap = nullptr;
//	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
//	assert(SUCCEEDED(result));
//	// 全頂点に対してコピー
//	copy(vertices.begin(), vertices.end(), vertMap);
//}
//void VertexBuffer::TransferToBuffer()
//{
//	HRESULT result;
//
//	VertexPosNormalUv* vertMap = nullptr;
//	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
//	assert(SUCCEEDED(result));
//	// 全頂点に対してコピー
//	for (int i = 0; i < arrarySize; i++)
//	{
//		vertMap[i] = vertices[i]; // 座標をコピー
//	}
//}
//void VertexBuffer::Unmap()
//{
//	// 繋がりを解除
//	vertBuff->Unmap(0, nullptr);
//}
