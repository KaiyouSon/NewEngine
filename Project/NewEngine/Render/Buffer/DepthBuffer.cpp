#include "DepthBuffer.h"
#include "RenderBase.h"
#include "RenderWindow.h"
#include <cassert>

void DepthBuffer::Create(const Vec2 size)
{
	HRESULT result;

	Vec2 depthResourceSize = size;
	depthResourceSize = (depthResourceSize != -1) ? size : RenderWindow::GetInstance()->GetWindowSize();

	// リソースの設定
	D3D12_RESOURCE_DESC depthResourceDesc{};
	depthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResourceDesc.Width = (uint32_t)depthResourceSize.x;		// 幅
	depthResourceDesc.Height = (uint32_t)depthResourceSize.y; // 高さ
	depthResourceDesc.DepthOrArraySize = 1;
	depthResourceDesc.Format = DXGI_FORMAT_D32_FLOAT;	// 深度値デフォルト
	depthResourceDesc.SampleDesc.Count = 1;
	depthResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	depthResourceDesc.MipLevels = 1;

	// 深度用ヒーププロパティ
	D3D12_HEAP_PROPERTIES depthHeapProp{};
	depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	// 深度値のクリア設定
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f;	// 深度値1.0f(最大値)でクリア
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;	// 深度値フォーマット

	// リソースの生成
	result = RenderBase::GetInstance()->GetDevice()->
		CreateCommittedResource(
			&depthHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&depthResourceDesc,
			D3D12_RESOURCE_STATE_DEPTH_WRITE, // 深度値書き込みに使用
			&depthClearValue,
			IID_PPV_ARGS(&mBuffer));
	assert(SUCCEEDED(result));

	RenderBase::GetInstance()->CreateDSV(*this);
}

// セッター
void DepthBuffer::SetCpuHandle(const D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle)
{
	mCpuHandle = cpuHandle;
}

// ゲッター
ID3D12Resource* DepthBuffer::GetBuffer() { return mBuffer.Get(); }
ID3D12Resource** DepthBuffer::GetBufferAddress() { return mBuffer.GetAddressOf(); }
D3D12_CPU_DESCRIPTOR_HANDLE DepthBuffer::GetCpuHandle() { return mCpuHandle; }