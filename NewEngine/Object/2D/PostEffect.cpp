#include "PostEffect.h"
#include "Camera.h"
using namespace ConstantBufferData;

#include "InputManager.h"
#include "TextureManager.h"
#include <d3d12.h>

PostEffect::PostEffect() :
	vertexBuffer_(std::make_unique<VertexBuffer<VertexPosUv>>()),
	graphicsPipeline_(GraphicsPipelineManager::GetGraphicsPipeline("RenderTexture")),
	pos(0), scale(1), rot(0), anchorPoint(0.5f), rtvIndex(0)
{
	// 頂点バッファの生成
	vertices_.resize(4);
	vertices_[0].uv = { 0.0f,1.0f };
	vertices_[1].uv = { 0.0f,0.0f };
	vertices_[2].uv = { 1.0f,1.0f };
	vertices_[3].uv = { 1.0f,0.0f };
	vertexBuffer_->Create(vertices_);

	// バッファ生成
	MaterialInit();
}

void PostEffect::Update()
{
	transform_.pos = pos;
	transform_.scale = { scale.x,scale.y,1.f };
	transform_.rot = { 0.f,0.f,rot };
	transform_.Update();

	// 転送処理
	MaterialTransfer();
}

void PostEffect::Draw()
{
	if (renderTextures_.empty() == true) return;

	//SetBlendMode(blendMode);

	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	renderBase->GetCommandList()->SetPipelineState(graphicsPipeline_->GetAlphaPipeline());

	renderBase->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// VBVとIBVの設定コマンド
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, vertexBuffer_->GetvbViewAddress());

	// マテリアルの設定コマンド
	MaterialDrawCommands();

	//if (Key::GetKeyDown(DIK_0))
	//{
	//	static int tex = 0;
	//	tex = (tex + 1) % 2;

	//	// SRVヒープの先頭ハンドルを取得
	//	D3D12_CPU_DESCRIPTOR_HANDLE srvCpuHandle = TextureManager::srvDescHeap_->GetCPUDescriptorHandleForHeapStart();
	//	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = TextureManager::srvDescHeap_->GetGPUDescriptorHandleForHeapStart();

	//	UINT descriptorSize = RenderBase::GetInstance()->GetDevice()->
	//		GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//	srvCpuHandle.ptr += (SIZE_T)(descriptorSize * TextureManager::srvIncrementIndex_);
	//	srvGpuHandle.ptr += (SIZE_T)(descriptorSize * TextureManager::srvIncrementIndex_);

	//	renderTextures_[0]->cpuHandle = srvCpuHandle;
	//	renderTextures_[0]->gpuHandle = srvGpuHandle;

	//	// シェーダーリソースビュー設定
	//	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};	// srv設定構造体
	//	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	//	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	// 2Dテクスチャ
	//	srvDesc.Texture2D.MipLevels = 1;

	//	// ハンドルの指す位置にシェーダーリソースビュー作成
	//	RenderBase::GetInstance()->GetDevice()->
	//		CreateShaderResourceView(renderTextures_[0]->buffers[tex].Get(), &srvDesc, srvCpuHandle);

	//	TextureManager::srvIncrementIndex_++;

	//}

	// SRV設定コマンド
	for (uint32_t i = 0; i < renderTextures_.size(); i++)
	{
		uint32_t index = renderBase->GetRenderTextureRootSignature()->GetConstantBufferNum();
		renderBase->GetCommandList()->SetGraphicsRootDescriptorTable(index + i, renderTextures_[i]->gpuHandles[rtvIndex]);
	}

	renderBase->GetCommandList()->DrawInstanced((uint16_t)vertices_.size(), 1, 0, 0);
}

// --- マテリアル関連 --------------------------------------------------- //
void PostEffect::MaterialInit()
{
	// インスタンス生成
	std::unique_ptr<IConstantBuffer> iConstantBuffer;

	// 3D行列
	iConstantBuffer = std::make_unique<ConstantBuffer<CTransform2D>>();
	material_.constantBuffers.push_back(std::move(iConstantBuffer));

	// 色
	iConstantBuffer = std::make_unique<ConstantBuffer<CColor>>();
	material_.constantBuffers.push_back(std::move(iConstantBuffer));

	// 初期化
	material_.Init();
}
void PostEffect::MaterialTransfer()
{
	// マトリックス
	CTransform2D transform3DData =
	{
		transform_.GetWorldMat() * Camera::current.GetOrthoGrphicProjectionMat()
	};
	TransferDataToConstantBuffer(material_.constantBuffers[0].get(), transform3DData);

	// 色データ
	CColor colorData = { color / 255 };
	TransferDataToConstantBuffer(material_.constantBuffers[1].get(), colorData);
}
void PostEffect::MaterialDrawCommands()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	// CBVの設定コマンド
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		0, material_.constantBuffers[0]->constantBuffer->GetGPUVirtualAddress());

	// CBVの設定コマンド
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		1, material_.constantBuffers[1]->constantBuffer->GetGPUVirtualAddress());
}

// --- その他の関数 ----------------------------------------------------- //
void PostEffect::AddRenderTexture(RenderTexture* renderTexture)
{
	renderTextures_.push_back(renderTexture);

	// テクスチャーが一枚の時にしか頂点座標変えない
	if (renderTextures_.size() == 1)
	{
		TransferTexturePos();
	}
}

void PostEffect::SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline)
{
	graphicsPipeline_ = graphicsPipeline;
}

void PostEffect::SetDrawCommands(const uint32_t registerNum, const uint32_t bufferNum)
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	uint32_t bNum = Min<uint32_t>(bufferNum, (uint32_t)material_.constantBuffers.size());

	// CBVの設定コマンド
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		registerNum, material_.constantBuffers[bNum]->constantBuffer->GetGPUVirtualAddress());
}

void PostEffect::TransferTexturePos()
{
	// テクスチャーのサイズ
	float width = renderTextures_.front()->size.x;
	float height = renderTextures_.front()->size.y;

	vertices_[0].pos = { (0.0f - anchorPoint.x) * width,(1.0f - anchorPoint.y) * height,0.0f }; //左下
	vertices_[1].pos = { (0.0f - anchorPoint.x) * width,(0.0f - anchorPoint.y) * height,0.0f }; //左上
	vertices_[2].pos = { (1.0f - anchorPoint.x) * width,(1.0f - anchorPoint.y) * height,0.0f }; //右下
	vertices_[3].pos = { (1.0f - anchorPoint.x) * width,(0.0f - anchorPoint.y) * height,0.0f }; //右上

	vertexBuffer_->TransferToBuffer(vertices_);
}