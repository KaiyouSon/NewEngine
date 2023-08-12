#include "PostEffect.h"
#include "TextureManager.h"
#include "Camera.h"

using namespace VertexBufferData;
using namespace ConstantBufferData;

PostEffect::PostEffect() :
	mVertexBuffer(std::make_unique<VertexBuffer<VSprite>>()),
	mGraphicsPipeline(GraphicsPipelineManager::GetGraphicsPipeline("RenderTexture")),
	pos(0), scale(1), rot(0), mAnchorPoint(0.5f), rtvIndex(0)
{
	// 頂点バッファの生成
	mVertices.resize(4);
	mVertices[0].uv = { 0.0f,1.0f };
	mVertices[1].uv = { 0.0f,0.0f };
	mVertices[2].uv = { 1.0f,1.0f };
	mVertices[3].uv = { 1.0f,0.0f };
	mVertexBuffer->Create(mVertices);

	// バッファ生成
	MaterialInit();

}

void PostEffect::Update()
{
	mTransform.pos = pos;
	mTransform.scale = { scale.x,scale.y,1.f };
	mTransform.rot = { 0.f,0.f,rot };
	mTransform.Update();

	// 転送処理
	MaterialTransfer();

	mVertexBuffer->TransferToBuffer(mVertices);
}
void PostEffect::Draw()
{
	if (mRenderTextures.empty() == true) return;

	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	// GraphicsPipeline描画コマンド
	mGraphicsPipeline->DrawCommand(BlendMode::Alpha);

	// VBVとIBVの設定コマンド
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, mVertexBuffer->GetvbViewAddress());

	// マテリアルの設定コマンド
	MaterialDrawCommands();

	// SRV設定コマンド
	uint32_t startIndex = mGraphicsPipeline->GetRootSignature()->GetDescriptorTableStartIndex();
	for (uint32_t i = 0; i < mRenderTextures.size(); i++)
	{
		renderBase->GetCommandList()->
			SetGraphicsRootDescriptorTable(startIndex + i, mRenderTextures[i]->GetGpuHandle(rtvIndex));

		if (mRenderTextures[i]->useDepth == true)
		{
			CD3DX12_RESOURCE_BARRIER barrier =
				CD3DX12_RESOURCE_BARRIER::Transition(
					mRenderTextures[i]->depthTexture->buffer.Get(),
					D3D12_RESOURCE_STATE_DEPTH_WRITE,
					D3D12_RESOURCE_STATE_GENERIC_READ,
					D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES);
			renderBase->GetCommandList()->ResourceBarrier(1, &barrier);

			renderBase->GetCommandList()->
				SetGraphicsRootDescriptorTable(
					(uint32_t)(startIndex + 1),
					mRenderTextures[i]->depthTexture->GetGpuHandle());
		}
		else
		{
			renderBase->GetCommandList()->
				SetGraphicsRootDescriptorTable(startIndex + 1, mRenderTextures[i]->GetGpuHandle(rtvIndex));
		}
	}

	renderBase->GetCommandList()->DrawInstanced((uint16_t)mVertices.size(), 1, 0, 0);

	for (uint32_t i = 0; i < mRenderTextures.size(); i++)
	{
		if (mRenderTextures[i]->useDepth == true)
		{
			CD3DX12_RESOURCE_BARRIER barrier =
				CD3DX12_RESOURCE_BARRIER::Transition(
					mRenderTextures[i]->depthTexture->buffer.Get(),
					D3D12_RESOURCE_STATE_GENERIC_READ,
					D3D12_RESOURCE_STATE_DEPTH_WRITE,
					D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES);
			renderBase->GetCommandList()->ResourceBarrier(1, &barrier);
		}
	}
}

// --- マテリアル関連 --------------------------------------------------- //
void PostEffect::MaterialInit()
{
	// インスタンス生成
	std::unique_ptr<IConstantBuffer> iConstantBuffer;

	// 3D行列
	iConstantBuffer = std::make_unique<ConstantBuffer<CTransform2D>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// 色
	iConstantBuffer = std::make_unique<ConstantBuffer<CColor>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// 初期化
	mMaterial.Init();
}
void PostEffect::MaterialTransfer()
{
	// マトリックス
	CTransform2D transform3DData =
	{
		mTransform.GetWorldMat() * Camera::current.GetOrthoGrphicProjectionMat()
	};
	TransferDataToConstantBuffer(mMaterial.constantBuffers[0].get(), transform3DData);

	// 色データ
	CColor colorData = { color / 255 };
	TransferDataToConstantBuffer(mMaterial.constantBuffers[1].get(), colorData);
}
void PostEffect::MaterialDrawCommands()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	for (uint32_t i = 0; i < mMaterial.constantBuffers.size(); i++)
	{
		// CBVの設定コマンド
		renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
			i, mMaterial.constantBuffers[i]->constantBuffer->GetGPUVirtualAddress());
	}

	//// CBVの設定コマンド
	//renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
	//	1, mMaterial.constantBuffers[1]->constantBuffer->GetGPUVirtualAddress());
}

// --- 頂点データ関連 --------------------------------------------------- //
void PostEffect::TransferTexturePos()
{
	// テクスチャーのサイズ
	float width = mRenderTextures.front()->size.x;
	float height = mRenderTextures.front()->size.y;

	mVertices[0].pos = { (0.0f - mAnchorPoint.x) * width,(1.0f - mAnchorPoint.y) * height,0.0f }; //左下
	mVertices[1].pos = { (0.0f - mAnchorPoint.x) * width,(0.0f - mAnchorPoint.y) * height,0.0f }; //左上
	mVertices[2].pos = { (1.0f - mAnchorPoint.x) * width,(1.0f - mAnchorPoint.y) * height,0.0f }; //右下
	mVertices[3].pos = { (1.0f - mAnchorPoint.x) * width,(0.0f - mAnchorPoint.y) * height,0.0f }; //右上

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

	/*switch (mFlipType)
	{
	case FlipType::X:
		left = -left;
		right = -right;
		break;

	case FlipType::Y:
		up = -up;
		down = -down;
		break;

	case FlipType::XY:
		left = -left;
		right = -right;
		up = -up;
		down = -down;
		break;

	default:
		break;
	}*/

	// 頂点座標
	mVertices[(uint32_t)Point::LD].pos = Vec3(left, down, 0.f);	  //左下
	mVertices[(uint32_t)Point::LU].pos = Vec3(left, up, 0.f);	  //左上
	mVertices[(uint32_t)Point::RD].pos = Vec3(right, down, 0.f);  //右下
	mVertices[(uint32_t)Point::RU].pos = Vec3(right, up, 0.f);	  //右上
}

// --- その他の関数 ----------------------------------------------------- //
void PostEffect::AddRenderTexture(RenderTexture* renderTexture)
{
	mRenderTextures.push_back(renderTexture);

	// テクスチャーが一枚の時にしか頂点座標変えない
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
void PostEffect::SetDrawCommands(const uint32_t registerNum, const uint32_t bufferNum)
{
	// GraphicsPipeline描画コマンド
	//mGraphicsPipeline->DrawCommand(BlendMode::Alpha);

	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	uint32_t bNum = Min<uint32_t>(bufferNum, (uint32_t)mMaterial.constantBuffers.size());

	// CBVの設定コマンド
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		registerNum, mMaterial.constantBuffers[bNum]->constantBuffer->GetGPUVirtualAddress());
}
