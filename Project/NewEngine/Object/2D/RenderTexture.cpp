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
	for (int i = 0; i < buffers.size(); i++)
	{
		CD3DX12_RESOURCE_BARRIER resourceBarrier =
			CD3DX12_RESOURCE_BARRIER::Transition(
				buffers[i].Get(),
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
				D3D12_RESOURCE_STATE_RENDER_TARGET);
		renderBase->GetCommandList()->ResourceBarrier(1, &resourceBarrier);
	}

	// RTV CPUハンドル
	std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> rtvCpuHandle;
	for (int i = 0; i < buffers.size(); i++)
	{
		rtvCpuHandle.push_back(renderTargets[i].GetCpuHandle());
	}

	// DSV CPUハンドル
	D3D12_CPU_DESCRIPTOR_HANDLE dsvCpuHandle = depthBuffer.GetCpuHandle();

	// レンダーターゲットをセット
	renderBase->GetCommandList()->OMSetRenderTargets((uint32_t)renderTargets.size(), rtvCpuHandle.data(), false, &dsvCpuHandle);

	// ビューポートの設定
	mViewports.resize(buffers.size());
	for (int i = 0; i < buffers.size(); i++)
	{
		mViewports[i] =
			CD3DX12_VIEWPORT(
				0.f, 0.f,
				size.x,
				size.y);
	}
	renderBase->GetCommandList()->RSSetViewports((uint32_t)mViewports.size(), mViewports.data());

	// シザー矩形の設定
	mScissorRects.resize(buffers.size());
	for (int i = 0; i < buffers.size(); i++)
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
	for (int i = 0; i < buffers.size(); i++)
	{
		CD3DX12_RESOURCE_BARRIER resourceBarrier =
			CD3DX12_RESOURCE_BARRIER::Transition(
				buffers[i].Get(),
				D3D12_RESOURCE_STATE_RENDER_TARGET,
				D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
		renderBase->GetCommandList()->ResourceBarrier(1, &resourceBarrier);
	}
}

void RenderTexture::AddSRVIndex(const uint32_t index)
{
	mSrvIndexes.push_back(index);
}

// セッター
void RenderTexture::SetHandleNum(const uint32_t num)
{
	mCpuHandles.resize(num);
	mGpuHandles.resize(num);
}
void RenderTexture::SetCpuHandle(const uint32_t index, const D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle)
{
	mCpuHandles[index] = cpuHandle;
}
void RenderTexture::SetGpuHandle(const uint32_t index, const D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle)
{
	mGpuHandles[index] = gpuHandle;
}

// ゲッター
D3D12_CPU_DESCRIPTOR_HANDLE RenderTexture::GetCpuHandle(const uint32_t index)
{
	uint32_t min = 0;
	uint32_t max = (uint32_t)(mCpuHandles.size() - 1);
	uint32_t i = Clamp<uint32_t>(index, min, max);

	return mCpuHandles[i];
}
D3D12_GPU_DESCRIPTOR_HANDLE RenderTexture::GetGpuHandle(const uint32_t index)
{
	uint32_t min = 0;
	uint32_t max = (uint32_t)(mGpuHandles.size() - 1);
	uint32_t i = Clamp<uint32_t>(index, min, max);

	return mGpuHandles[i];
}

std::vector<uint32_t>* RenderTexture::GetSRVIndexes()
{
	return &mSrvIndexes;
}
