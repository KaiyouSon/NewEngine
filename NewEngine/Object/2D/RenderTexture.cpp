#include "RenderTexture.h"
#include "Camera.h"
#include "RenderBase.h"
#include "RenderWindow.h"
#include "TextureManager.h"
#include <memory>
using namespace std;

const float RenderTexture::clearColor[4] = { 0.0f,0.0f,0.0f,1.0f };

RenderTexture::RenderTexture()
{
	HRESULT result;
	RenderBase* renderBase = RenderBase::GetInstance();

	// リソース設定
	CD3DX12_RESOURCE_DESC texturenResourceDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_R8G8B8A8_UNORM,
			RenderWindow::GetInstance()->GetWindowSize().x,
			RenderWindow::GetInstance()->GetWindowSize().y,
			1, 1, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

	//CD3DX12_HEAP_PROPERTIES texturenResourceHeapProp =
	//	CD3DX12_HEAP_PROPERTIES(
	//		D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,
	//		D3D12_MEMORY_POOL_L0);

	CD3DX12_HEAP_PROPERTIES texturenResourceHeapProp =
		CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	CD3DX12_CLEAR_VALUE textureResourceClearValue =
		CD3DX12_CLEAR_VALUE(
			DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
			clearColor);

	size = RenderWindow::GetInstance()->GetWindowSize();

	result = renderBase->GetDevice()->
		CreateCommittedResource(
			&texturenResourceHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&texturenResourceDesc,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			&textureResourceClearValue,
			IID_PPV_ARGS(&buffer));
	assert(SUCCEEDED(result));

	//{
	//	const UINT pixelCount =
	//		RenderWindow::GetInstance()->GetWindowSize().x *
	//		RenderWindow::GetInstance()->GetWindowSize().y;

	//	const UINT rowPitch = sizeof(UINT) * RenderWindow::GetInstance()->GetWindowSize().x;
	//	const UINT depthPitch = rowPitch * RenderWindow::GetInstance()->GetWindowSize().y;

	//	UINT* img = new UINT[pixelCount];
	//	for (int i = 0; i < pixelCount; i++)
	//	{
	//		//img[i] = 0xffffffff;
	//		img[i] = 0x00000ff;
	//	}

	//	result = buffer->WriteToSubresource(0, nullptr, img, rowPitch, depthPitch);
	//	assert(SUCCEEDED(result));

	//	delete[] img;
	//}

	// レンダーターゲットビューの設定
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	// シェーダーの計算結果をSRGBに変換して書き込む
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	depthBuffer.Create();
	renderTarget.buffer = buffer;
	TextureManager::CreateSRV(*this);
	renderBase->CreateRTV(renderTarget, &rtvDesc);
	renderBase->CreateDSV(depthBuffer);
}

void RenderTexture::PreDrawScene()
{
	RenderBase* renderBase = RenderBase::GetInstance();
	RenderWindow* renderWindow = RenderWindow::GetInstance().get();

	// リソースバリアを変更（シェーダーリソース -> 描画可能）
	CD3DX12_RESOURCE_BARRIER resourceBarrier =
		CD3DX12_RESOURCE_BARRIER::Transition(
			buffer.Get(),
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			D3D12_RESOURCE_STATE_RENDER_TARGET);
	renderBase->GetCommandList()->ResourceBarrier(1, &resourceBarrier);

	D3D12_CPU_DESCRIPTOR_HANDLE rtvCpuHandle = renderTarget.GetCpuHandle();
	D3D12_CPU_DESCRIPTOR_HANDLE dsvCpuHandle = depthBuffer.GetCpuHandle();

	// レンダーターゲットをセット
	renderBase->GetCommandList()->OMSetRenderTargets(1, &rtvCpuHandle, false, &dsvCpuHandle);

	// ビューポートの設定
	CD3DX12_VIEWPORT viewport =
		CD3DX12_VIEWPORT(
			0.0f, 0.0f,
			renderWindow->GetWindowSize().x,
			renderWindow->GetWindowSize().y);
	renderBase->GetCommandList()->RSSetViewports(1, &viewport);

	// シザリング矩形の設定
	CD3DX12_RECT scissorRect = CD3DX12_RECT(
		0, 0,
		renderWindow->GetWindowSize().x,
		renderWindow->GetWindowSize().y);
	renderBase->GetCommandList()->RSSetScissorRects(1, &scissorRect);

	// 全画面クリア
	renderBase->GetCommandList()->ClearRenderTargetView(rtvCpuHandle, clearColor, 0, nullptr);

	// 深度バッファのクリア
	renderBase->GetCommandList()->ClearDepthStencilView(dsvCpuHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}
void RenderTexture::PostDrawScene()
{
	RenderBase* renderBase = RenderBase::GetInstance();

	// リソースバリアを変更（描画可能 -> シェーダーリソース）
	CD3DX12_RESOURCE_BARRIER resourceBarrier =
		CD3DX12_RESOURCE_BARRIER::Transition(
			buffer.Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	renderBase->GetCommandList()->ResourceBarrier(1, &resourceBarrier);
}