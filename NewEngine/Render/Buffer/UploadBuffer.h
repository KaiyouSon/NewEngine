#pragma once
#include "RenderBase.h"
#include <cassert>
#include <wrl.h>
#include <d3dx12.h>

template<typename T>
class UploadBuffer
{
private:
	HRESULT result;

	Microsoft::WRL::ComPtr<ID3D12Resource> uploadBuffer;//	定数バッファ
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT  footprint;
	T* uploadBufferMap = nullptr;	// マッピング用

public:
	void Create(const CD3DX12_RESOURCE_DESC& textureResourcesDesc)
	{
		UINT64 uploadSize;

		// レイアウトの取得
		RenderBase::GetInstance()->GetDevice()->
			GetCopyableFootprints(
				&textureResourcesDesc,
				0,
				1,
				0,
				&footprint,
				nullptr,
				nullptr,
				&uploadSize);

		// ヒープの設定
		CD3DX12_HEAP_PROPERTIES uploadHeapProp =
			CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

		// ソースの設定
		CD3DX12_RESOURCE_DESC uploadResourceDesc =
			CD3DX12_RESOURCE_DESC::Buffer(uploadSize);

		// テクスチャバッファの生成
		result = RenderBase::GetInstance()->GetDevice()->
			CreateCommittedResource(
				&uploadHeapProp,
				D3D12_HEAP_FLAG_NONE,
				&uploadResourceDesc,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&uploadBuffer));
		assert(SUCCEEDED(result));
	}

	void Transfer(ID3D12Resource* textureBuffer, const T& data)
	{
		uploadBuffer->Map(0, nullptr, (void**)&uploadBufferMap);
		assert(SUCCEEDED(result));
		Color col = { 1,1,1,1 };
		memcpy(uploadBufferMap, &data, sizeof(T));

		D3D12_TEXTURE_COPY_LOCATION destLocation;
		destLocation.pResource = textureBuffer;
		destLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
		destLocation.SubresourceIndex = 0;

		D3D12_TEXTURE_COPY_LOCATION sourceLocation;
		sourceLocation.pResource = uploadBuffer.Get();
		sourceLocation.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
		sourceLocation.PlacedFootprint = footprint;

		RenderBase::GetInstance()->GetCommandList()->
			CopyTextureRegion(&destLocation, 0, 0, 0, &sourceLocation, nullptr);

		D3D12_RESOURCE_BARRIER  barrier{};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Transition.pResource = textureBuffer;
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;
	}

	static void ExecutionCommand()
	{
		RenderBase::GetInstance()->GetCommandList()->Close();

		ID3D12CommandList* list[] = { RenderBase::GetInstance()->GetCommandList() };
		RenderBase::GetInstance()->GetCommandQueue()->ExecuteCommandLists(1, list);

		RenderBase::GetInstance()->PreIncrimentFenceValue();

		// コマンドの実行完了を待つ
		RenderBase::GetInstance()->GetCommandQueue()->Signal(
			RenderBase::GetInstance()->GetFence(),
			RenderBase::GetInstance()->GetFenceValue());

		auto test = RenderBase::GetInstance()->GetFence()->GetCompletedValue();
		if (RenderBase::GetInstance()->GetFence()->GetCompletedValue() != RenderBase::GetInstance()->GetFenceValue())
		{
			HANDLE event = CreateEvent(nullptr, false, false, nullptr);
			RenderBase::GetInstance()->GetFence()->SetEventOnCompletion(RenderBase::GetInstance()->GetFenceValue(), event);
			WaitForSingleObject(event, INFINITE);
			CloseHandle(event);
		}

		HRESULT result;
		// キューをクリア
		result = RenderBase::GetInstance()->GetCommandAllocator()->Reset();
		assert(SUCCEEDED(result));
		// 再びコマンドリストを貯める準備
		result = RenderBase::GetInstance()->GetCommandList()->
			Reset(RenderBase::GetInstance()->GetCommandAllocator(), nullptr);
		assert(SUCCEEDED(result));
	}
};

