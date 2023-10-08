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
		// 繝・せ繧ｯ繝ｪ繝励ち繝偵・繝励・險ｭ螳・
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		heapDesc.NumDescriptors = mSetting.maxSize;
	}
	break;

	case DescriptorHeapSetting::DSV:
	{
		// 繝・せ繧ｯ繝ｪ繝励ち繝偵・繝励・險ｭ螳・
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		heapDesc.NumDescriptors = mSetting.maxSize;
	}
	break;

	case DescriptorHeapSetting::CBV_SRV_UAV:
	{
		// 繝・せ繧ｯ繝ｪ繝励ち繝偵・繝励・險ｭ螳・
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		heapDesc.NumDescriptors = mSetting.maxSize;
	}
	break;
	}

	// 繝・せ繧ｯ繝ｪ繝励ち繝偵・繝励・逕滓・
	mResult = RenderBase::GetInstance()->GetDevice()->
		CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&mDescriptorHeap));
	assert(SUCCEEDED(mResult));
}

// SRV菴懈・
void DescriptorHeap::CreateSRV(BufferResource* bufferResource, const uint32_t arraySize, const uint32_t byteSize)
{
	if (mSetting.heapType != DescriptorHeapSetting::CBV_SRV_UAV)
	{
		return;
	}

	// 繧､繝ｳ繝・ャ繧ｯ繧ｹ繧貞叙蠕・
	uint32_t incrementIndex = mSetting.startIndex + GetIncrementIndex();

	// 繧ｵ繧､繧ｺ繧貞叙蠕・
	uint32_t incrementSize = GetIncrementSize();

	// 繝偵・繝励・蜈磯ｭ繝上Φ繝峨Ν繧貞叙蠕・
	bufferResource->srvHandle.cpu = mDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	bufferResource->srvHandle.gpu = mDescriptorHeap->GetGPUDescriptorHandleForHeapStart();

	// index蛻・★繧峨☆
	bufferResource->srvHandle.cpu.ptr += (uint32_t)(incrementSize * incrementIndex);
	bufferResource->srvHandle.gpu.ptr += (uint32_t)(incrementSize * incrementIndex);
	bufferResource->index = incrementIndex;

	// SRV縺ｮ險ｭ螳・
	D3D12_SHADER_RESOURCE_VIEW_DESC desc{};	// srv險ｭ螳壽ｧ矩菴・
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
		desc.Buffer.FirstElement = 0;				// 譛蛻昴・隕∫ｴ縺ｮ繧､繝ｳ繝・ャ繧ｯ繧ｹ
		desc.Buffer.NumElements = arraySize;		// 繝舌ャ繝輔ぃ蜀・・隕∫ｴ謨ｰ
		desc.Buffer.StructureByteStride = byteSize;	// 隕∫ｴ1縺､縺ｮ繝舌う繝域焚
	}
	else
	{
		desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		desc.Format = bufferResource->buffer->GetDesc().Format;
		desc.Texture2D.MipLevels = bufferResource->buffer->GetDesc().MipLevels;
	}
	desc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	// 繝上Φ繝峨Ν縺ｮ謖・☆菴咲ｽｮ縺ｫSRV繧剃ｽ懈・
	RenderBase::GetInstance()->GetDevice()->
		CreateShaderResourceView(
			bufferResource->buffer.Get(),
			&desc,
			bufferResource->srvHandle.cpu);
}

// RTV菴懈・
void DescriptorHeap::CreateRTV(BufferResource* bufferResource)
{
	if (mSetting.heapType != DescriptorHeapSetting::RTV)
	{
		return;
	}

	// 繧､繝ｳ繝・ャ繧ｯ繧ｹ繧貞叙蠕・
	uint32_t incrementIndex = mSetting.startIndex + GetIncrementIndex();

	// 繧ｵ繧､繧ｺ繧貞叙蠕・
	uint32_t incrementSize = GetIncrementSize();

	// 繝偵・繝励・蜈磯ｭ繝上Φ繝峨Ν繧貞叙蠕・
	bufferResource->rtvHandle.cpu = mDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	bufferResource->rtvHandle.gpu = mDescriptorHeap->GetGPUDescriptorHandleForHeapStart();

	// index蛻・★繧峨☆
	bufferResource->rtvHandle.cpu.ptr += (uint32_t)(incrementSize * incrementIndex);
	bufferResource->rtvHandle.gpu.ptr += (uint32_t)(incrementSize * incrementIndex);
	bufferResource->index = incrementIndex;

	// 繝ｬ繝ｳ繝繝ｼ繧ｿ繝ｼ繧ｲ繝・ヨ繝薙Η繝ｼ縺ｮ險ｭ螳・
	D3D12_RENDER_TARGET_VIEW_DESC desc{};
	// 繧ｷ繧ｧ繝ｼ繝繝ｼ縺ｮ險育ｮ礼ｵ先棡繧担RGB縺ｫ螟画鋤縺励※譖ｸ縺崎ｾｼ繧
	desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

	// 繝上Φ繝峨Ν縺ｮ謖・☆菴咲ｽｮ縺ｫRTV繧剃ｽ懈・
	RenderBase::GetInstance()->GetDevice()->
		CreateRenderTargetView(
			bufferResource->buffer.Get(),
			&desc,
			bufferResource->rtvHandle.cpu);
}

// DSV菴懈・
void DescriptorHeap::CreateDSV(BufferResource* bufferResource)
{
	if (mSetting.heapType != DescriptorHeapSetting::DSV)
	{
		return;
	}

	// 繧､繝ｳ繝・ャ繧ｯ繧ｹ繧貞叙蠕・
	uint32_t incrementIndex = mSetting.startIndex + GetIncrementIndex();

	// 繧ｵ繧､繧ｺ繧貞叙蠕・
	uint32_t incrementSize = GetIncrementSize();

	// 繝偵・繝励・蜈磯ｭ繝上Φ繝峨Ν繧貞叙蠕・
	bufferResource->dsvHandle.cpu = mDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	bufferResource->dsvHandle.gpu = mDescriptorHeap->GetGPUDescriptorHandleForHeapStart();

	// index蛻・★繧峨☆
	bufferResource->dsvHandle.cpu.ptr += (uint32_t)(incrementSize * incrementIndex);
	bufferResource->dsvHandle.gpu.ptr += (uint32_t)(incrementSize * incrementIndex);
	bufferResource->index = incrementIndex;

	// 豺ｱ蠎ｦ繝薙Η繝ｼ菴懈・
	D3D12_DEPTH_STENCIL_VIEW_DESC desc = {};
	desc.Format = DXGI_FORMAT_D32_FLOAT;	// 豺ｱ蠎ｦ蛟､繝輔か繝ｼ繝槭ャ繝・
	desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;

	// 繝上Φ繝峨Ν縺ｮ謖・☆菴咲ｽｮ縺ｫDSV繧剃ｽ懈・
	RenderBase::GetInstance()->GetDevice()->
		CreateDepthStencilView(
			bufferResource->buffer.Get(),
			&desc,
			bufferResource->dsvHandle.cpu);
}

// UAV菴懈・
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

	// 繧､繝ｳ繝・ャ繧ｯ繧ｹ繧貞叙蠕・
	uint32_t incrementIndex = mSetting.startIndex + GetIncrementIndex();

	// 繧ｵ繧､繧ｺ繧貞叙蠕・
	uint32_t incrementSize = GetIncrementSize();

	// 繝偵・繝励・蜈磯ｭ繝上Φ繝峨Ν繧貞叙蠕・
	bufferResource->uavHandle.cpu = mDescriptorHeap->GetCPUDescriptorHandleForHeapStart();
	bufferResource->uavHandle.gpu = mDescriptorHeap->GetGPUDescriptorHandleForHeapStart();

	// index蛻・★繧峨☆
	bufferResource->uavHandle.cpu.ptr += (uint32_t)(incrementSize * incrementIndex);
	bufferResource->uavHandle.gpu.ptr += (uint32_t)(incrementSize * incrementIndex);
	bufferResource->index = incrementIndex;

	// UAV縺ｮ險ｭ螳・
	D3D12_UNORDERED_ACCESS_VIEW_DESC desc{};
	desc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	desc.Format = DXGI_FORMAT_UNKNOWN;
	desc.Buffer.FirstElement = 0;				// 譛蛻昴・隕∫ｴ縺ｮ繧､繝ｳ繝・ャ繧ｯ繧ｹ
	desc.Buffer.NumElements = arraySize;		// 繝舌ャ繝輔ぃ蜀・・隕∫ｴ謨ｰ
	desc.Buffer.StructureByteStride = byteSize;	// 隕∫ｴ1縺､縺ｮ繝舌う繝域焚

	// 繝上Φ繝峨Ν縺ｮ謖・☆菴咲ｽｮ縺ｫUAV繧剃ｽ懈・
	RenderBase::GetInstance()->GetDevice()->
		CreateUnorderedAccessView(
			bufferResource->buffer.Get(),
			nullptr,
			&desc,
			bufferResource->uavHandle.cpu);
}

// 繝薙Η繝ｼ繧剃ｸ頑嶌縺阪〒縺阪ｋ繧医≧縺ｫ縺吶ｋ
void DescriptorHeap::DestroyView(BufferResource* bufferResource)
{
	mCheckIndex[bufferResource->index] = false;
}

uint32_t DescriptorHeap::GetIncrementIndex()
{
	// ImGUI縺ｧ0逡ｪ菴ｿ縺｣縺ｦ縺ゅｋ縺九ｉ1縺九ｉ菴ｿ縺・
	uint32_t index = 0;

	// mCheckSRVIndex菴ｿ縺｣縺ｦ縺ｪ縺・分蜿ｷ縺ｪ縺・°繧偵メ繧ｧ繝・け
	for (uint32_t i = 0; i < mCheckIndex.size(); i++)
	{
		// i逡ｪ縺・false 縺縺｣縺溘ｉ
		if (mCheckIndex[i] == false)
		{
			index = i + 1;
			mCheckIndex[i] = true;
			break;
		}
	}

	if (index == 0)
	{
		// 譛蠕悟ｰｾ縺ｫ譁ｰ縺励＞繧・▽繧定ｿｽ蜉
		mCheckIndex.push_back(true);
		return (uint32_t)mCheckIndex.size();	// 蜈・°繧永ndex繧医ｊ1螟壹＞縺九ｉ+1縺励↑縺上※縺・＞
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
