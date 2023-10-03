#include "RenderTexture.h"
#include "Camera.h"
#include "RenderBase.h"
#include "RenderWindow.h"
#include "TextureManager.h"
#include <memory>
using namespace std;

const float RenderTexture::sClearColor[4] = { 0.f,0.f,0.f,1.0f };

void RenderTexture::PrevDrawScene()
{
	RenderBase* renderBase = RenderBase::GetInstance();
	RenderWindow* renderWindow = RenderWindow::GetInstance().get();

	// リソースバリアを変更（シェーダーリソース -> 描画可能）
	for (int i = 0; i < mBufferResources.size(); i++)
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
	for (int i = 0; i < mBufferResources.size(); i++)
	{
		rtvCpuHandle.push_back(renderTargets[i].GetBufferResource()->rtvHandle.cpu);
	}

	// DSV CPUハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE dsvCpuHandle = depthBuffer.GetBufferResource()->dsvHandle.cpu;

	// レンダーターゲットをセット
	renderBase->GetCommandList()->OMSetRenderTargets((uint32_t)renderTargets.size(), rtvCpuHandle.data(), false, &dsvCpuHandle);

	// ビューポートの設定
	mViewports.resize(mBufferResources.size());
	for (int i = 0; i < mBufferResources.size(); i++)
	{
		mViewports[i] =
			CD3DX12_VIEWPORT(
				0.f, 0.f,
				size.x,
				size.y);
	}
	renderBase->GetCommandList()->RSSetViewports((uint32_t)mViewports.size(), mViewports.data());

	// シザー矩形の設定
	mScissorRects.resize(mBufferResources.size());
	for (int i = 0; i < mBufferResources.size(); i++)
	{
		mScissorRects[i] =
			CD3DX12_RECT(
				0, 0,
				(LONG)size.x,
				(LONG)size.y);
	}
	renderBase->GetCommandList()->RSSetScissorRects((uint32_t)mScissorRects.size(), mScissorRects.data());

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
