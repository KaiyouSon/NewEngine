#include "PostEffect.h"
#include "TextureManager.h"
#include "Camera.h"

using namespace VertexBufferData;
using namespace ConstantBufferData;

PostEffect::PostEffect() :
	mVertexBuffer(std::make_unique<VertexBuffer<VSprite>>()),
	mMaterial(std::make_unique<Material>()),
	mCSMaterial(std::make_unique<Material>()),
	mGraphicsPipeline(PipelineManager::GetGraphicsPipeline("PostEffect")),
	mComputePipeline(nullptr),
	pos(0), scale(1), rot(0), mAnchorPoint(0.5f), rtvIndex(0)
{
	// 頂点バッファ生成
	mVertices.resize(4);
	mVertices[0].uv = { 0.0f,1.0f };
	mVertices[1].uv = { 0.0f,0.0f };
	mVertices[2].uv = { 1.0f,1.0f };
	mVertices[3].uv = { 1.0f,0.0f };
	mVertexBuffer->Create(mVertices);

	// マテリアルの初期化
	MaterialInit();
}

void PostEffect::Update()
{
	mTransform.pos = pos;
	mTransform.scale = { scale.x,scale.y,1.f };
	mTransform.rot = { 0.f,0.f,rot };
	mTransform.Update();

	// マテリアルの転送
	MaterialTransfer();

	mVertexBuffer->TransferToBuffer(mVertices);
}
void PostEffect::ExecuteCS(const uint32_t threadX, const uint32_t threadY, const uint32_t threadZ)
{
	if (mComputePipeline == nullptr) return;

	auto* cmdList = RenderBase::GetInstance()->GetCommandList();

	CSMaterialTransfer();

	mComputePipeline->DrawCommand();

	CSMaterialDrawCommands();

	// ディスパッチ
	cmdList->Dispatch(threadX, threadY, threadZ);

}
void PostEffect::Draw()
{
	if (mRenderTextures.empty() == true) return;

	auto* renderBase = RenderBase::GetInstance();// .get();
	auto* cmdList = renderBase->GetCommandList();

	// GraphicsPipelineの描画コマンド
	mGraphicsPipeline->DrawCommand(BlendMode::Alpha);

	// VBVとのセット
	cmdList->IASetVertexBuffers(0, 1, mVertexBuffer->GetvbViewAddress());

	// マテリアルの描画コマンド
	MaterialDrawCommands();

	// SRVの設定
	uint32_t startIndex = mGraphicsPipeline->GetRootSignature()->GetSRVStartIndex();
	for (uint32_t i = 0; i < mRenderTextures.size(); i++)
	{
		// RenderTexture
		cmdList->SetGraphicsRootDescriptorTable(
			startIndex + i,
			mRenderTextures[i]->GetBufferResources()->at(rtvIndex).srvHandle.gpu);

		// DepthTexture
		if (mRenderTextures[i]->useDepth == true)
		{
			renderBase->TransitionBufferState(
				mRenderTextures[i]->GetDepthTexture()->GetBufferResource(),
				D3D12_RESOURCE_STATE_DEPTH_WRITE,
				D3D12_RESOURCE_STATE_GENERIC_READ);

			cmdList->SetGraphicsRootDescriptorTable(
				(uint32_t)(startIndex + 1),
				mRenderTextures[i]->GetDepthTexture()->GetBufferResource()->srvHandle.gpu);
		}
		//else
		//{
		//	cmdList->SetGraphicsRootDescriptorTable(
		//		startIndex + 1,
		//		mRenderTextures[i]->GetBufferResources()->at(rtvIndex).srvHandle.gpu);
		//}
	}
	for (uint32_t i = 0; i < mVolumeTextures.size(); i++)
	{
		// VolumeTexture
		cmdList->SetGraphicsRootDescriptorTable(
			startIndex + (uint32_t)mRenderTextures.size() + i,
			mVolumeTextures[i]->GetBufferResource()->srvHandle.gpu);
	}

	// CSOutput
	uint32_t offsetIndex = startIndex + (uint32_t)mRenderTextures.size();
	for (uint32_t i = 0; i < mCSMaterial->structuredBuffers.size(); i++)
	{
		cmdList->SetGraphicsRootDescriptorTable(
			offsetIndex + i,
			mCSMaterial->structuredBuffers[i]->GetBufferResource()->srvHandle.gpu);
	}

	cmdList->DrawInstanced((uint16_t)mVertices.size(), 1, 0, 0);

	for (uint32_t i = 0; i < mRenderTextures.size(); i++)
	{
		if (mRenderTextures[i]->useDepth == true)
		{
			renderBase->TransitionBufferState(
				mRenderTextures[i]->GetDepthTexture()->GetBufferResource(),
				D3D12_RESOURCE_STATE_GENERIC_READ,
				D3D12_RESOURCE_STATE_DEPTH_WRITE);
		}
	}
}

// --- マテリアル関連 --------------------------------------------------- //
void PostEffect::MaterialInit()
{
	// インターフェース
	std::unique_ptr<IConstantBuffer> iConstantBuffer;

	// トランスフォーム
	iConstantBuffer = std::make_unique<ConstantBuffer<CTransform2D>>();
	mMaterial->constantBuffers.push_back(std::move(iConstantBuffer));

	// 色
	iConstantBuffer = std::make_unique<ConstantBuffer<CColor>>();
	mMaterial->constantBuffers.push_back(std::move(iConstantBuffer));

	// 初期化
	mMaterial->Init();
}
void PostEffect::MaterialTransfer()
{
	// トランスフォーム
	CTransform2D transformData =
	{
		mTransform.GetWorldMat() * Camera::current.GetOrthogrphicMat()
	};
	TransferDataToConstantBuffer(mMaterial->constantBuffers[0].get(), transformData);

	// 色
	CColor colorData = { color.To01() };
	TransferDataToConstantBuffer(mMaterial->constantBuffers[1].get(), colorData);
}
void PostEffect::MaterialDrawCommands()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	for (uint32_t i = 0; i < mMaterial->constantBuffers.size(); i++)
	{
		// CBVのセット
		renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
			i, mMaterial->constantBuffers[i]->bufferResource->buffer->GetGPUVirtualAddress());
	}
}

// --- CSマテリアル関連 ------------------------------------------------- //
void PostEffect::CSMaterialTransfer()
{
}
void PostEffect::CSMaterialDrawCommands()
{
	auto* cmdList = RenderBase::GetInstance()->GetCommandList();

	// ディスクリプターヒープ設定
	auto srvDescHeap = DescriptorHeapManager::GetDescriptorHeap("SRV")->GetDescriptorHeap();
	cmdList->SetDescriptorHeaps(1, &srvDescHeap);

	// CBV
	uint32_t cbvStartIndex = mComputePipeline->GetRootSignature()->GetCBVStartIndex();
	for (uint32_t i = 0; i < mCSMaterial->constantBuffers.size(); i++)
	{
		cmdList->SetComputeRootConstantBufferView(
			cbvStartIndex + i, mCSMaterial->constantBuffers[i]->bufferResource->buffer->GetGPUVirtualAddress());
	}

	// UAV
	uint32_t uavStartIndex = mComputePipeline->GetRootSignature()->GetUAVStartIndex();
	// その他のUAVデータ
	for (uint32_t i = 0; i < mCSMaterial->structuredBuffers.size(); i++)
	{
		cmdList->SetComputeRootDescriptorTable(
			uavStartIndex + i, mCSMaterial->structuredBuffers[i]->GetBufferResource()->uavHandle.gpu);
	}
}

// --- 頂点データ関連 --------------------------------------------------- //
void PostEffect::TransferTexturePos()
{
	// テクスチャーのサイズ
	float width = mRenderTextures.front()->GetInitalSize().x;
	float height = mRenderTextures.front()->GetInitalSize().y;

	mVertices[0].pos = { (0.0f - mAnchorPoint.x) * width,(1.0f - mAnchorPoint.y) * height,0.0f }; //蟾ｦ荳・
	mVertices[1].pos = { (0.0f - mAnchorPoint.x) * width,(0.0f - mAnchorPoint.y) * height,0.0f }; //蟾ｦ荳・
	mVertices[2].pos = { (1.0f - mAnchorPoint.x) * width,(1.0f - mAnchorPoint.y) * height,0.0f }; //蜿ｳ荳・
	mVertices[3].pos = { (1.0f - mAnchorPoint.x) * width,(0.0f - mAnchorPoint.y) * height,0.0f }; //蜿ｳ荳・

	mVertexBuffer->TransferToBuffer(mVertices);
}
void PostEffect::TransferVertexCoord()
{
	enum class Point { LD, LU, RD, RU };

	// 四辺
	float left = (0.f - mAnchorPoint.x) * mSize.x;
	float right = (1.f - mAnchorPoint.x) * mSize.x;
	float up = (0.f - mAnchorPoint.y) * mSize.y;
	float down = (1.f - mAnchorPoint.y) * mSize.y;

	// 頂点座標
	mVertices[(uint32_t)Point::LD].pos = Vec3(left, down, 0.f);	  // 左下
	mVertices[(uint32_t)Point::LU].pos = Vec3(left, up, 0.f);	  // 左上
	mVertices[(uint32_t)Point::RD].pos = Vec3(right, down, 0.f);  // 右下
	mVertices[(uint32_t)Point::RU].pos = Vec3(right, up, 0.f);	  // 右上
}

// --- その他の関数 ----------------------------------------------------- //
void PostEffect::AddRenderTexture(RenderTexture* renderTexture)
{
	if (renderTexture == nullptr)
	{
		return;
	}

	mRenderTextures.push_back(renderTexture);

	// テクスチャが初めて追加されたときにサイズ設定
	if (mRenderTextures.size() == 1)
	{
		TransferTexturePos();
	}
}

// --- セッター --------------------------------------------------------- //
void PostEffect::SetSize(const Vec2 size)
{
	mSize = size;
	TransferVertexCoord();
}
void PostEffect::SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline)
{
	mGraphicsPipeline = graphicsPipeline;
}
void PostEffect::SetComputePipeline(ComputePipeline* computePipeline)
{
	mComputePipeline = computePipeline;
}
