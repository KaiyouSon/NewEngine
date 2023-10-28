#include "DescriptorHeap.h"
#include "RenderBase.h"
#include <cassert>

void DescriptorHeap::Create(const DescriptorHeapSetting setting)
{
	mSetting = setting;

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};

	// ディスクリプタヒープ設定
	switch (mSetting.heapType)
	{
	case DescriptorHeapSetting::RTV:
	{
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		heapDesc.NumDescriptors = mSetting.maxSize;
	}
	break;

	case DescriptorHeapSetting::DSV:
	{
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		heapDesc.NumDescriptors = mSetting.maxSize;
	}
	break;

	case DescriptorHeapSetting::CBV_SRV_UAV:
	{
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		heapDesc.NumDescriptors = mSetting.maxSize;
	}
	break;
	}

	// ディスクリプタヒープ生成
	mResult = RenderBase::GetInstance()->GetDevice()->
		CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&mDescriptorHeap));
	assert(SUCCEEDED(mResult));

	// 使った分trueにする
	for (uint32_t i = 0; i < mSetting.startIndex; i++)
	{
		mCheckIndex.push_back(true);
	}
}

// シェーダーリソースビュー（SRV）を作成する
void DescriptorHeap::CreateSRV(BufferResource* bufferResource, const uint32_t arraySize, const uint32_t byteSize)
{
	if (mSetting.heapType != DescriptorHeapSetting::CBV_SRV_UAV)
	{
		return;
	}

	// インクリメントインデックスを計算
	uint32_t incrementIndex = mSetting.startIndex + GetIncrementIndex();

	// インクリメントサイズを計算
	uint32_t incrementSize = GetIncrementSize();

	// CPUとGPUハンドルを設定
	bufferResource->srvHandle.cpu = mDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	bufferResource->srvHandle.gpu = mDescriptorHeap->GetGPUDescriptorHandleForHeapStart();

	// インデックスに基づいてハンドルを更新
	bufferResource->srvHandle.cpu.ptr += (uint32_t)(incrementSize * incrementIndex);
	bufferResource->srvHandle.gpu.ptr += (uint32_t)(incrementSize * incrementIndex);
	bufferResource->viewIndexes.push_back(ViewIndex(incrementIndex - mSetting.startIndex, ViewType::SRV));

	// SRVを作成
	D3D12_SHADER_RESOURCE_VIEW_DESC desc{};

	// 2Dテクスチャ
	D3D12_RESOURCE_DESC bufferDesc = bufferResource->buffer->GetDesc();
	if (bufferDesc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE2D)
	{
		// デプステクスチャ
		if (bufferDesc.Format == DXGI_FORMAT_D32_FLOAT)
		{
			desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			desc.Format = DXGI_FORMAT_R32_FLOAT;
			desc.Texture2D.MipLevels = bufferDesc.MipLevels;
		}
		// 通常のテクスチャ
		else
		{
			desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			desc.Format = bufferDesc.Format;
			desc.Texture2D.MipLevels = bufferDesc.MipLevels;
		}
	}
	// 3Dテクスチャ
	else if (bufferDesc.Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE3D)
	{
		desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE3D;
		desc.Format = bufferDesc.Format;
		desc.Texture3D.MipLevels = bufferDesc.MipLevels;
	}
	// バッファ
	else
	{
		desc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
		desc.Format = bufferDesc.Format;
		desc.Buffer.FirstElement = 0;
		desc.Buffer.NumElements = arraySize;
		desc.Buffer.StructureByteStride = byteSize;
	}
	desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	// シェーダーリソースビューを作成
	RenderBase::GetInstance()->GetDevice()->
		CreateShaderResourceView(
			bufferResource->buffer.Get(),
			&desc,
			bufferResource->srvHandle.cpu);
}

// レンダーターゲットビュー（RTV）を作成
void DescriptorHeap::CreateRTV(BufferResource* bufferResource)
{
	if (mSetting.heapType != DescriptorHeapSetting::RTV)
	{
		return;
	}

	// インクリメントインデックスを計算
	uint32_t incrementIndex = mSetting.startIndex + GetIncrementIndex();

	// インクリメントサイズを計算
	uint32_t incrementSize = GetIncrementSize();

	// CPUとGPUハンドルを設定
	bufferResource->rtvHandle.cpu = mDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	bufferResource->rtvHandle.gpu = mDescriptorHeap->GetGPUDescriptorHandleForHeapStart();

	// インデックスに基づいてハンドルを更新
	bufferResource->rtvHandle.cpu.ptr += (uint32_t)(incrementSize * incrementIndex);
	bufferResource->rtvHandle.gpu.ptr += (uint32_t)(incrementSize * incrementIndex);
	bufferResource->viewIndexes.push_back(ViewIndex(incrementIndex - mSetting.startIndex, ViewType::RTV));

	// レンダーターゲットビューの設定
	D3D12_RENDER_TARGET_VIEW_DESC desc{};

	D3D12_RESOURCE_DESC bufferDesc = bufferResource->buffer->GetDesc();
	desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	// バッファを生成
	RenderBase::GetInstance()->GetDevice()->
		CreateRenderTargetView(
			bufferResource->buffer.Get(),
			&desc,
			bufferResource->rtvHandle.cpu);
}

// デプスステンシルビュー（DSV）を作成
void DescriptorHeap::CreateDSV(BufferResource* bufferResource)
{
	if (mSetting.heapType != DescriptorHeapSetting::DSV)
	{
		return;
	}

	// インクリメントインデックスを計算
	uint32_t incrementIndex = mSetting.startIndex + GetIncrementIndex();

	// インクリメントサイズを計算
	uint32_t incrementSize = GetIncrementSize();

	// CPUとGPUハンドルを設定
	bufferResource->dsvHandle.cpu = mDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	bufferResource->dsvHandle.gpu = mDescriptorHeap->GetGPUDescriptorHandleForHeapStart();

	// インデックスに基づいてハンドルを更新
	bufferResource->dsvHandle.cpu.ptr += (uint32_t)(incrementSize * incrementIndex);
	bufferResource->dsvHandle.gpu.ptr += (uint32_t)(incrementSize * incrementIndex);
	bufferResource->viewIndexes.push_back(ViewIndex(incrementIndex - mSetting.startIndex, ViewType::DSV));

	// デプスステンシルビューの設定
	D3D12_DEPTH_STENCIL_VIEW_DESC desc = {};
	desc.Format = DXGI_FORMAT_D32_FLOAT;	// デプスステンシルフォーマットを設定
	desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;

	// インデックスに基づいてDSVを作成
	RenderBase::GetInstance()->GetDevice()->
		CreateDepthStencilView(
			bufferResource->buffer.Get(),
			&desc,
			bufferResource->dsvHandle.cpu);
}

// アンオーダードアクセスビュー（UAV）を作成
void DescriptorHeap::CreateUAV(BufferResource* bufferResource, const uint32_t arraySize, const uint32_t byteSize)
{
	if (mSetting.heapType != DescriptorHeapSetting::CBV_SRV_UAV)
	{
		return;
	}

	// ステートが合わない場合は変更する
	if (bufferResource->bufferState != D3D12_RESOURCE_STATE_UNORDERED_ACCESS)
	{
		RenderBase::GetInstance()->TransitionBufferState(
			bufferResource,
			bufferResource->bufferState,
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS);
	}

	// インクリメントインデックスを計算
	uint32_t incrementIndex = mSetting.startIndex + GetIncrementIndex();

	// インクリメントサイズを計算
	uint32_t incrementSize = GetIncrementSize();

	// CPUとGPUハンドルを設定
	bufferResource->uavHandle.cpu = mDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	bufferResource->uavHandle.gpu = mDescriptorHeap->GetGPUDescriptorHandleForHeapStart();

	// インデックスに基づいてハンドルを更新
	bufferResource->uavHandle.cpu.ptr += (uint32_t)(incrementSize * incrementIndex);
	bufferResource->uavHandle.gpu.ptr += (uint32_t)(incrementSize * incrementIndex);
	bufferResource->viewIndexes.push_back(ViewIndex(incrementIndex - mSetting.startIndex, ViewType::UAV));

	// UAVの設定
	D3D12_UNORDERED_ACCESS_VIEW_DESC desc{};
	desc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.Buffer.FirstElement = 0;				// 識別子の最初の要素を設定
	desc.Buffer.NumElements = arraySize;		// 識別子の要素数を設定
	desc.Buffer.StructureByteStride = byteSize;	// 識別子の1要素のバイト数を設定

	// インデックスに基づいてUAVを作成
	RenderBase::GetInstance()->GetDevice()->
		CreateUnorderedAccessView(
			bufferResource->buffer.Get(),
			nullptr,
			&desc,
			bufferResource->uavHandle.cpu);
}

// ビューを破棄する
void DescriptorHeap::DestroyView(BufferResource* bufferResource)
{
	for (uint32_t i = 0; i < bufferResource->viewIndexes.size(); i++)
	{
		ViewIndex viewIndex = bufferResource->viewIndexes[i];

		if (viewIndex.index == -1)
		{
			continue;
		}

		// CBV_SRV_UAVの場合
		if (mSetting.heapType == DescriptorHeapSetting::CBV_SRV_UAV)
		{
			if (viewIndex.viewType == ViewType::SRV ||
				viewIndex.viewType == ViewType::UAV)
			{
				mCheckIndex[viewIndex.index] = false;
			}
		}
		// RTVの場合
		else if (mSetting.heapType == DescriptorHeapSetting::RTV)
		{
			if (viewIndex.viewType == ViewType::RTV)
			{
				mCheckIndex[viewIndex.index] = false;
			}
		}
		// DSVの場合
		else if (mSetting.heapType == DescriptorHeapSetting::DSV)
		{
			if (viewIndex.viewType == ViewType::DSV)
			{
				mCheckIndex[viewIndex.index] = false;
			}
		}
	}
}

uint32_t DescriptorHeap::GetIncrementIndex()
{
	uint32_t index = 0;

	// 使用されていない番号を探す
	for (uint32_t i = 0; i < mCheckIndex.size(); i++)
	{
		// i番がfalseの場合
		if (mCheckIndex[i] == false)
		{
			index = i;
			mCheckIndex[i] = true;
			break;
		}
	}

	if (index == 0)
	{
		// すべて使用中の場合、新しい番号を追加
		mCheckIndex.push_back(true);
		return (uint32_t)mCheckIndex.size() - 1; // 新しいindexは配列のサイズ-1である
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

	case DescriptorHeapSetting::DSV:
	{
		incrementSize = RenderBase::GetInstance()->GetDevice()->
			GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	}
	break;

	}

	return incrementSize;
}
ID3D12DescriptorHeap* DescriptorHeap::GetDescriptorHeap()
{
	return mDescriptorHeap.Get();
}
