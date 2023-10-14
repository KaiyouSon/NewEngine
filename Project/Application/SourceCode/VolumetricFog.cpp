#include "VolumetricFog.h"

using namespace VertexBufferData;
using namespace ConstantBufferData;

VolumetricFog::VolumetricFog() :
	mVertexBuffer(std::make_unique<VertexBuffer<VSprite>>()),
	mVolumetricFog(TextureManager::GetRenderTexture("VolumetricFog")),
	mGraphicsPipeline(PipelineManager::GetGraphicsPipeline("RenderTexture")), pos(0)
{
	// 頂点バッファの生成
	mVertices.resize(4);
	mVertices[0].uv = { 0.0f,1.0f };
	mVertices[1].uv = { 0.0f,0.0f };
	mVertices[2].uv = { 1.0f,1.0f };
	mVertices[3].uv = { 1.0f,0.0f };
	mVertexBuffer->Create(mVertices);

	// マテリアルの初期化
	MaterialInit();
}

void VolumetricFog::Init()
{
}

void VolumetricFog::Update()
{
	mTransform.pos = pos;
	mTransform.scale = 1.f;
	mTransform.rot = 0.f;
	mTransform.Update();

	// マテリアルの転送(CBVの転送)
	MaterialTransfer();

	mVertexBuffer->TransferToBuffer(mVertices);
}

void VolumetricFog::PrevDrawScene()
{
	mVolumetricFog->PrevDrawScene();
}

void VolumetricFog::PostDrawScene()
{
	mVolumetricFog->PostDrawScene();
}

void VolumetricFog::Draw()
{
	auto* cmdList = RenderBase::GetInstance()->GetCommandList();

	// GraphicsPipelineの描画コマンド
	mGraphicsPipeline->DrawCommand(BlendMode::Alpha);

	// VBV設定
	cmdList->IASetVertexBuffers(0, 1, mVertexBuffer->GetvbViewAddress());

	// マテリアルの描画コマンド(主にCBV設定)
	MaterialDrawCommands();

	// SRVの設定
	uint32_t startIndex = mGraphicsPipeline->GetRootSignature()->GetSRVStartIndex();
	cmdList->SetGraphicsRootDescriptorTable(
		startIndex, mVolumetricFog->GetBufferResources()->begin()->srvHandle.gpu);

	cmdList->DrawInstanced((uint16_t)mVertices.size(), 1, 0, 0);
}

// --- マテリアル関連 --------------------------------------------------- //
void VolumetricFog::MaterialInit()
{
	// インタフェース
	std::unique_ptr<IConstantBuffer> iConstantBuffer;

	// 2Dトランスフォーム
	iConstantBuffer = std::make_unique<ConstantBuffer<CTransform2D>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// 初期化
	mMaterial.Init();
}
void VolumetricFog::MaterialTransfer()
{
	// 2Dトランスフォーム
	CTransform2D transform2DData =
	{
		mTransform.GetWorldMat() * Camera::current.GetOrthoGrphicProjectionMat()
	};
	TransferDataToConstantBuffer(mMaterial.constantBuffers[0].get(), transform2DData);
}
void VolumetricFog::MaterialDrawCommands()
{
	auto* cmdList = RenderBase::GetInstance()->GetCommandList();
	for (uint32_t i = 0; i < mMaterial.constantBuffers.size(); i++)
	{
		// CBVの設定
		cmdList->SetGraphicsRootConstantBufferView(
			i, mMaterial.constantBuffers[i]->bufferResource->buffer->GetGPUVirtualAddress());
	}
}

