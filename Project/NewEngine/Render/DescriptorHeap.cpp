#include "DescriptorHeap.h"
#include "RenderBase.h"
#include <cassert>

DescriptorHeapSetting::DescriptorHeapSetting() :
	maxSize(64), startIndex(0), heapType(DescriptorHeapSetting::None)
{
}

void DescriptorHeap::Create(const DescriptorHeapSetting setting)
{
	mSetting = setting;

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};

	switch (mSetting.heapType)
	{
	case DescriptorHeapSetting::RTV:
	{
		// デスクリプタヒープの設定
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		heapDesc.NumDescriptors = mSetting.maxSize;
	}
	break;

	case DescriptorHeapSetting::DSV:
	{
		// デスクリプタヒープの設定
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		heapDesc.NumDescriptors = mSetting.maxSize;
	}
	break;

	case DescriptorHeapSetting::CBV_SRV_UAV:
	{
		// デスクリプタヒープの設定
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		heapDesc.NumDescriptors = mSetting.maxSize;
	}
	break;
	}

	// デスクリプタヒープの生成
	mResult = RenderBase::GetInstance()->GetDevice()->
		CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&mDescriptorHeap));
	assert(SUCCEEDED(mResult));
}

void DescriptorHeap::CreateSRV(BufferResource* bufferResource, const uint32_t arraySize, const uint32_t byteSize)
{
	if (mSetting.heapType != DescriptorHeapSetting::CBV_SRV_UAV)
	{
		return;
	}

	// インデックスを取得
	uint32_t incrementIndex = mSetting.startIndex + GetIncrementIndex();

	// サイズを取得
	uint32_t incrementSize = GetIncrementSize();

	// ヒープの先頭ハンドルを取得
	bufferResource->srvHandle.cpu = mDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	bufferResource->srvHandle.gpu = mDescriptorHeap->GetGPUDescriptorHandleForHeapStart();

	// index分ずらす
	bufferResource->srvHandle.cpu.ptr += (uint32_t)(incrementSize * incrementIndex);
	bufferResource->srvHandle.gpu.ptr += (uint32_t)(incrementSize * incrementIndex);
	bufferResource->index = incrementIndex;

	// SRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC desc{};	// srv設定構造体
	if (bufferResource->buffer->GetDesc().Format == DXGI_FORMAT_D32_FLOAT)
	{
		desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		desc.Format = DXGI_FORMAT_R32_FLOAT;
		desc.Texture2D.MipLevels = bufferResource->buffer->GetDesc().MipLevels;
	}
	else if (bufferResource->buffer->GetDesc().Format == DXGI_FORMAT_UNKNOWN)
	{
		desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
		desc.Format = bufferResource->buffer->GetDesc().Format;
		desc.Buffer.FirstElement = 0;				// 最初の要素のインデックス
		desc.Buffer.NumElements = arraySize;		// バッファ内の要素数
		desc.Buffer.StructureByteStride = byteSize;	// 要素1つのバイト数
	}
	else
	{
		desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		desc.Format = bufferResource->buffer->GetDesc().Format;
		desc.Texture2D.MipLevels = bufferResource->buffer->GetDesc().MipLevels;
	}
	desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	// ハンドルの指す位置にSRVを作成
	RenderBase::GetInstance()->GetDevice()->
		CreateShaderResourceView(
			bufferResource->buffer.Get(),
			&desc,
			bufferResource->srvHandle.cpu);
}

void DescriptorHeap::CreateRTV(BufferResource* bufferResource)
{
	if (mSetting.heapType != DescriptorHeapSetting::RTV)
	{
		return;
	}

	// インデックスを取得
	uint32_t incrementIndex = GetIncrementIndex();

	// サイズを取得
	uint32_t incrementSize = GetIncrementSize();

	// ヒープの先頭ハンドルを取得
	bufferResource->rtvHandle.cpu = mDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	bufferResource->rtvHandle.gpu = mDescriptorHeap->GetGPUDescriptorHandleForHeapStart();

	// index分ずらす
	bufferResource->rtvHandle.cpu.ptr += (uint32_t)(incrementSize * incrementIndex);
	bufferResource->rtvHandle.gpu.ptr += (uint32_t)(incrementSize * incrementIndex);
	bufferResource->index = incrementIndex;

	// レンダーターゲットビューの設定
	D3D12_RENDER_TARGET_VIEW_DESC desc{};
	// シェーダーの計算結果をSRGBに変換して書き込む
	desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	// ハンドルの指す位置にRTVを作成
	RenderBase::GetInstance()->GetDevice()->
		CreateRenderTargetView(
			bufferResource->buffer.Get(),
			&desc,
			bufferResource->rtvHandle.cpu);
}

void DescriptorHeap::CreateUAV(BufferResource* bufferResource, const uint32_t arraySize, const uint32_t byteSize)
{
	if (mSetting.heapType != DescriptorHeapSetting::CBV_SRV_UAV)
	{
		return;
	}

	// インデックスを取得
	uint32_t incrementIndex = GetIncrementIndex();

	// サイズを取得
	uint32_t incrementSize = GetIncrementSize();

	// ヒープの先頭ハンドルを取得
	bufferResource->uavHandle.cpu = mDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	bufferResource->uavHandle.gpu = mDescriptorHeap->GetGPUDescriptorHandleForHeapStart();

	// index分ずらす
	bufferResource->uavHandle.cpu.ptr += (uint32_t)(incrementSize * incrementIndex);
	bufferResource->uavHandle.gpu.ptr += (uint32_t)(incrementSize * incrementIndex);
	bufferResource->index = incrementIndex;

	// UAVの設定
	D3D12_UNORDERED_ACCESS_VIEW_DESC desc{};
	desc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.Buffer.FirstElement = 0;				// 最初の要素のインデックス
	desc.Buffer.NumElements = arraySize;		// バッファ内の要素数
	desc.Buffer.StructureByteStride = byteSize;	// 要素1つのバイト数

	// ハンドルの指す位置にUAVを作成
	RenderBase::GetInstance()->GetDevice()->
		CreateUnorderedAccessView(
			bufferResource->buffer.Get(),
			nullptr,
			&desc,
			bufferResource->uavHandle.cpu);
}

uint32_t DescriptorHeap::GetIncrementIndex()
{
	// ImGUIで0番使ってあるから1から使う
	uint32_t index = 0;

	// mCheckSRVIndex使ってない番号ないかをチェック
	for (uint32_t i = 0; i < mCheckIndex.size(); i++)
	{
		// i番が false だったら
		if (mCheckIndex[i] == false)
		{
			index = i + 1;
			mCheckIndex[i] = true;
			break;
		}
	}

	if (index == 0)
	{
		// 最後尾に新しいやつを追加
		mCheckIndex.push_back(true);
		return (uint32_t)mCheckIndex.size();	// 元からindexより1多いから+1しなくていい
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
	}
	break;

	case DescriptorHeapSetting::RTV:
	{
		incrementSize = RenderBase::GetInstance()->GetDevice()->
			GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}
	break;

	}

	return incrementSize;
}
ID3D12DescriptorHeap* DescriptorHeap::GetDescriptorHeap()
{
	return mDescriptorHeap.Get();
}