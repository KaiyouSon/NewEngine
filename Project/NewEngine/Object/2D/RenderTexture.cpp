#include "RenderTexture.h"
#include "Camera.h"
#include "RenderBase.h"
#include "RenderWindow.h"
#include "TextureManager.h"
#include <memory>

DirectX12WarningDisableBegin
#include <d3dx12.h>
DirectX12WarningDisableEnd

using namespace std;

const float RenderTexture::sClearColor[4] = { 0.f,0.f,0.f,1.0f };

void RenderTexture::Create(const RenderTextureSetting setting)
{
	mType = TextureType::Render;

	mInitalSize = setting.texSize;
	mBufferResources.resize(setting.rtvNum);
	mViewports.resize(setting.rtvNum);
	mScissorRects.resize(setting.rtvNum);

	mDepthBuffer = std::make_unique<DepthBuffer>();
	mDepthTexture = std::make_unique<DepthTexture>();

	// ヒープ設定
	D3D12_HEAP_PROPERTIES heapProp =
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	// リソース設定
	D3D12_RESOURCE_DESC resourceDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
			(uint64_t)setting.texSize.x, (uint32_t)setting.texSize.y,
			1, 1, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

	// クリアバリュー
	D3D12_CLEAR_VALUE clearValue =
		CD3DX12_CLEAR_VALUE(
			DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
			RenderTexture::sClearColor);

	HRESULT result;
	for (uint32_t i = 0; i < setting.rtvNum; i++)
	{
		result = RenderBase::GetInstance()->GetDevice()->
			CreateCommittedResource(
				&heapProp,
				D3D12_HEAP_FLAG_NONE,
				&resourceDesc,
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
				&clearValue,
				IID_PPV_ARGS(&mBufferResources[i].buffer));
		assert(SUCCEEDED(result));

		mBufferResources[i].buffer->SetName(L"RenderTexture");
		mBufferResources[i].bufferState = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

		// テクスチャのSRVとRTV作成
		DescriptorHeapManager::GetDescriptorHeap("RTV")->CreateRTV(&GetBufferResources()->at(i));
	}
	DescriptorHeapManager::GetDescriptorHeap("SRV")->CreateSRV(&GetBufferResources()->at(0));

	// 深度バッファの生成
	mDepthBuffer->Create(setting.depthSize);

	// DSV作成
	DescriptorHeapManager::GetDescriptorHeap("DSV")->CreateDSV(mDepthBuffer->GetBufferResource());

	if (setting.isUseDepthTexture == true)
	{
		// 深度テクスチャの作成
		mDepthTexture->Create(mDepthBuffer.get());

		// 深度テクスチャのSRV
		DescriptorHeapManager::GetDescriptorHeap("SRV")->CreateSRV(mDepthTexture->GetBufferResource());
	}
}

void RenderTexture::PrevDrawScene()
{
	RenderBase* renderBase = RenderBase::GetInstance();

	// リソースバリアを変更（シェーダーリソース -> 描画可能）
	for (uint32_t i = 0; i < mBufferResources.size(); i++)
	{
		CD3DX12_RESOURCE_BARRIER resourceBarrier =
			CD3DX12_RESOURCE_BARRIER::Transition(
				mBufferResources[i].buffer.Get(),
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
				D3D12_RESOURCE_STATE_RENDER_TARGET);
		renderBase->GetCommandList()->ResourceBarrier(1, &resourceBarrier);
	}

	// RTV CPUハンドル
	std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> rtvCpuHandle;
	for (uint32_t i = 0; i < mBufferResources.size(); i++)
	{
		rtvCpuHandle.push_back(mBufferResources[i].rtvHandle.cpu);
	}

	// DSV CPUハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE dsvCpuHandle = mDepthBuffer->GetBufferResource()->dsvHandle.cpu;

	// レンダーターゲットをセット
	renderBase->GetCommandList()->OMSetRenderTargets(
		(uint32_t)mBufferResources.size(), rtvCpuHandle.data(), false, &dsvCpuHandle);

	// ビューポートの設定
	for (int i = 0; i < mBufferResources.size(); i++)
	{
		mViewports[i].SetViewport(Vec2::zero, Vec2(mInitalSize.x, mInitalSize.y), 0, 1);
		mViewports[i].DrawCommands();
	}

	// シザー矩形の設定
	for (int i = 0; i < mBufferResources.size(); i++)
	{
		mScissorRects[i].SetRectAngle(RectAngle(Vec2::zero, Vec2(mInitalSize.x, mInitalSize.y)));
		mScissorRects[i].DrawCommands();
	}

	// 全画面クリア
	for (int i = 0; i < rtvCpuHandle.size(); i++)
	{
		renderBase->GetCommandList()->ClearRenderTargetView(rtvCpuHandle[i], sClearColor, 0, nullptr);
	}

	// 深度バッファのクリア
	renderBase->GetCommandList()->ClearDepthStencilView(dsvCpuHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}
void RenderTexture::PostDrawScene()
{
	RenderBase* renderBase = RenderBase::GetInstance();

	// リソースバリアを変更（描画可能 -> シェーダーリソース）
	for (int i = 0; i < mBufferResources.size(); i++)
	{
		CD3DX12_RESOURCE_BARRIER resourceBarrier =
			CD3DX12_RESOURCE_BARRIER::Transition(
				mBufferResources[i].buffer.Get(),
				D3D12_RESOURCE_STATE_RENDER_TARGET,
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		renderBase->GetCommandList()->ResourceBarrier(1, &resourceBarrier);
	}
}

std::vector<BufferResource>* RenderTexture::GetBufferResources()
{
	return &mBufferResources;
}

DepthTexture* RenderTexture::GetDepthTexture()
{
	return mDepthTexture.get();
}

DepthBuffer* RenderTexture::GetDepthBuffer()
{
	return mDepthBuffer.get();
}
