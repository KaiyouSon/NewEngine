#include "DescriptorHeap.h"
#include "RenderBase.h"
#include <cassert>

void DescriptorHeap::Create(const DescriptorHeapSetting setting)
{
	mSetting = setting;

	switch (mSetting.heapType)
	{
	case DescriptorHeapSetting::CBV_SRV_UAV:
	{
		// デスクリプタヒープの設定
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		heapDesc.NumDescriptors = mSetting.maxSize;

		// UAV用デスクリプタヒープの生成
		mResult = RenderBase::GetInstance()->GetDevice()->
			CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&mDescriptorHeap));
		assert(SUCCEEDED(mResult));
	}
	break;


	}
}

void DescriptorHeap::CreateSRV(BufferResource* bufferResource, const uint32_t arraySize, const uint32_t byteSize)
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
	bufferResource->cpuHandle = mDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	bufferResource->gpuHandle = mDescriptorHeap->GetGPUDescriptorHandleForHeapStart();

	// index分ずらす
	bufferResource->cpuHandle.ptr += (uint32_t)(incrementSize * incrementIndex);
	bufferResource->gpuHandle.ptr += (uint32_t)(incrementSize * incrementIndex);
	bufferResource->index = incrementIndex;

	// SRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC desc{};	// srv設定構造体
	if (bufferResource->buffer->GetDesc().Format == DXGI_FORMAT_D32_FLOAT)
	{
		desc.Format = DXGI_FORMAT_R32_FLOAT;
		desc.Texture2D.MipLevels = bufferResource->buffer->GetDesc().MipLevels;
	}
	else if (bufferResource->buffer->GetDesc().Format == DXGI_FORMAT_UNKNOWN)
	{
		uint32_t zero = 0;
		desc.Format = bufferResource->buffer->GetDesc().Format;
		desc.Buffer.FirstElement = 0;				// 最初の要素のインデックス
		desc.Buffer.NumElements = arraySize;		// バッファ内の要素数
		desc.Buffer.StructureByteStride = byteSize;	// 要素1つのバイト数
	}
	else
	{
		desc.Format = bufferResource->buffer->GetDesc().Format;
		desc.Texture2D.MipLevels = bufferResource->buffer->GetDesc().MipLevels;
	}
	desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;

	// ハンドルの指す位置にSRVを作成
	RenderBase::GetInstance()->GetDevice()->
		CreateShaderResourceView(
			bufferResource->buffer.Get(),
			&desc,
			bufferResource->cpuHandle);
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
	bufferResource->cpuHandle = mDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	bufferResource->gpuHandle = mDescriptorHeap->GetGPUDescriptorHandleForHeapStart();

	// index分ずらす
	bufferResource->cpuHandle.ptr += (uint32_t)(incrementSize * incrementIndex);
	bufferResource->gpuHandle.ptr += (uint32_t)(incrementSize * incrementIndex);
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
			bufferResource->cpuHandle);
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
		break;
	}
	}

	return incrementSize;
}
ID3D12DescriptorHeap* DescriptorHeap::GetDescriptorHeap()
{
	return mDescriptorHeap.Get();
}