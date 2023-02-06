#include "RenderTexture.h"
#include "Camera.h"
#include "RenderBase.h"
#include "RenderWindow.h"
#include <memory>
using namespace std;

const float RenderTexture::clearColor[4] = { 0.0f,0.0f,0.0f,0.0f };

RenderTexture::RenderTexture() :
	pos(0), scale(1), rot(0), anchorPoint(0.5f),
	vertexBuffer(new VertexBuffer<VertexPosUv>),
	constantBufferTransform(new ConstantBuffer<ConstantBufferDataTransform2D>),
	constantBufferColor(new ConstantBuffer<ConstantBufferDataColor>)
{
	HRESULT result;
	RenderBase* renderBase = RenderBase::GetInstance();

	vertices.resize(4);
	vertices[0].uv = { 0.0f,1.0f };
	vertices[1].uv = { 0.0f,0.0f };
	vertices[2].uv = { 1.0f,1.0f };
	vertices[3].uv = { 1.0f,0.0f };
	vertexBuffer->Create(vertices);

	// 定数バッファ
	constantBufferTransform->Init();
	constantBufferColor->Init();

	// リソース設定
	CD3DX12_RESOURCE_DESC texturenResourceDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_R8G8B8A8_UNORM,
			RenderWindow::GetInstance()->GetWindowSize().x,
			RenderWindow::GetInstance()->GetWindowSize().y,
			1, 1, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);

	CD3DX12_HEAP_PROPERTIES texturenResourceHeapProp =
		CD3DX12_HEAP_PROPERTIES(
			D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,
			D3D12_MEMORY_POOL_L0);

	CD3DX12_CLEAR_VALUE textureResourceClearValue =
		CD3DX12_CLEAR_VALUE(
			DXGI_FORMAT_R8G8B8A8_UNORM_SRGB,
			clearColor);

	texture.size = RenderWindow::GetInstance()->GetWindowSize();

	result = renderBase->GetDevice()->
		CreateCommittedResource(
			&texturenResourceHeapProp,
			D3D12_HEAP_FLAG_NONE,
			&texturenResourceDesc,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			&textureResourceClearValue,
			IID_PPV_ARGS(&texture.buffer));
	assert(SUCCEEDED(result));

	{
		const UINT pixelCount =
			RenderWindow::GetInstance()->GetWindowSize().x *
			RenderWindow::GetInstance()->GetWindowSize().y;

		const UINT rowPitch = sizeof(UINT) * RenderWindow::GetInstance()->GetWindowSize().x;
		const UINT depthPitch = rowPitch * RenderWindow::GetInstance()->GetWindowSize().y;

		UINT* img = new UINT[pixelCount];
		for (int i = 0; i < pixelCount; i++)
		{
			//img[i] = 0xffffffff;
			img[i] = 0x00000ff;
		}

		result = texture.buffer->WriteToSubresource(0, nullptr, img, rowPitch, depthPitch);
		assert(SUCCEEDED(result));

		delete[] img;
	}

	// レンダーターゲットビューの設定
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	// シェーダーの計算結果をSRGBに変換して書き込む
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	depthBuffer.Create();
	renderTarget.buffer = texture.buffer;
	renderBase->CreateSRV(texture, texturenResourceDesc);
	renderBase->CreateRTV(renderTarget, &rtvDesc);
	renderBase->CreateDSV(depthBuffer);
}

RenderTexture::~RenderTexture()
{
	delete vertexBuffer;
	delete constantBufferTransform;
	delete constantBufferColor;
}

void RenderTexture::Update()
{
	transform.pos = pos;
	transform.scale = { scale.x,scale.y,1 };
	transform.rot = { 0,0,rot };
	transform.Update();

	// 定数バッファに転送
	constantBufferTransform->constantBufferMap->mat =
		transform.worldMat *
		Camera::current.GetOrthoGrphicProjectionMat();

	// 色転送
	constantBufferColor->constantBufferMap->color = color / 255;
	constantBufferColor->constantBufferMap->color.a = color.a / 255;

	// 頂点バッファーに頂点を転送
	TransferTexturePos();
}

void RenderTexture::PreDrawScene()
{
	RenderBase* renderBase = RenderBase::GetInstance();
	RenderWindow* renderWindow = RenderWindow::GetInstance().get();

	// リソースバリアを変更（シェーダーリソース -> 描画可能）
	CD3DX12_RESOURCE_BARRIER resourceBarrier =
		CD3DX12_RESOURCE_BARRIER::Transition(
			texture.buffer.Get(),
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
			texture.buffer.Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	renderBase->GetCommandList()->ResourceBarrier(1, &resourceBarrier);
}
void RenderTexture::Draw()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	renderBase->GetCommandList()->SetPipelineState(renderBase->GetRenderTexturePipeline()->GetAlphaPipeline());
	renderBase->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// VBVとIBVの設定コマンド
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, vertexBuffer->GetvbViewAddress());

	// マテリアルとトランスフォームのCBVの設定コマンド
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		0, constantBufferTransform->constantBuffer->GetGPUVirtualAddress());
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		1, constantBufferColor->constantBuffer->GetGPUVirtualAddress());

	// SRVヒープの設定コマンド
	auto temp = renderBase->GetSrvDescHeap();
	renderBase->GetCommandList()->SetDescriptorHeaps(1, &temp);
	// SRVヒープの先頭にあるSRVをルートパラメータ2番に設定
	renderBase->GetCommandList()->SetGraphicsRootDescriptorTable(
		renderBase->GetSpriteRootSignature()->GetRootDescriptorTableIndex(), texture.GetGpuHandle());

	renderBase->GetCommandList()->DrawInstanced((unsigned short)vertices.size(), 1, 0, 0);
}

void RenderTexture::TransferTexturePos()
{
	// 新しいのサイズ
	float width = texture.size.x;
	float height = texture.size.y;

	// 現在のサイズ
	float width2 = vertices[0].pos.x - vertices[2].pos.x;
	float height2 = vertices[1].pos.x - vertices[3].pos.x;

	if (width != fabsf(width2) || width != fabsf(height2))
	{
		vertices[0].pos = { (0.0f - anchorPoint.x) * width,(1.0f - anchorPoint.y) * height,0.0f }; //左下
		vertices[1].pos = { (0.0f - anchorPoint.x) * width,(0.0f - anchorPoint.y) * height,0.0f }; //左上
		vertices[2].pos = { (1.0f - anchorPoint.x) * width,(1.0f - anchorPoint.y) * height,0.0f }; //右下
		vertices[3].pos = { (1.0f - anchorPoint.x) * width,(0.0f - anchorPoint.y) * height,0.0f }; //右上

		vertexBuffer->TransferToBuffer(vertices);
	}
}

void RenderTexture::SetCutPosAndSize(const Vec2& cutPos, const Vec2& cutSize)
{
	//float texLeft = cutPos.x / componentManager->GetComponent<Texture>()->GetTextureSize().x;
	//float texRight = (cutPos.x + cutSize.x) / componentManager->GetComponent<Texture>()->GetTextureSize().x;
	//float texUp = cutPos.y / componentManager->GetComponent<Texture>()->GetTextureSize().y;
	//float texDown = (cutPos.y + cutSize.y) / componentManager->GetComponent<Texture>()->GetTextureSize().y;

	//vertices[0].uv = { texLeft , texDown };	// 左下
	//vertices[1].uv = { texLeft ,   texUp };	// 左上
	//vertices[2].uv = { texRight ,texDown }; // 右下
	//vertices[3].uv = { texRight ,  texUp }; // 右上

	//vertexBuffer->TransferToBuffer(vertices);
}