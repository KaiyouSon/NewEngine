#include "CircleGaugeSprite.h"
#include "Camera.h"

using namespace VertexBufferData;
using namespace ConstantBufferData;

CircleGaugeSprite::CircleGaugeSprite() :
	pos(0), scale(1), rot(0), mAnchorPoint(0.5f),
	startRadian(0), endRadian(0),
	mVertexBuffer(std::make_unique<VertexBuffer<VSprite>>()),
	mGraphicsPipeline(PipelineManager::GetGraphicsPipeline("CircleGaugeSprite"))
{
	mVertices.resize(4);
	mVertices[0].uv = { 0.0f,1.0f };
	mVertices[1].uv = { 0.0f,0.0f };
	mVertices[2].uv = { 1.0f,1.0f };
	mVertices[3].uv = { 1.0f,0.0f };
	mVertexBuffer->Create(mVertices);

	// マテリアルの初期化
	MaterialInit();
}

void CircleGaugeSprite::Update(Transform* parent)
{
	mTransform.pos = pos;
	mTransform.scale = { scale.x,scale.y,1 };
	mTransform.rot = { 0,0,rot };
	mTransform.Update();

	if (parent != nullptr)
	{
		mParent = parent;

		Mat4 mat = mTransform.GetWorldMat();
		mat *= mParent->GetWorldMat();
		mTransform.SetWorldMat(mat);
	}

	endRadian = Clamp<float>(endRadian, 0, Radian(360));

	// マテリアルの転送
	MaterialTransfer();

	// 頂点データの転送
	mVertexBuffer->TransferToBuffer(mVertices);
}
void CircleGaugeSprite::Draw(const BlendMode blendMode)
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	// GraphicsPipeline描画コマンド
	mGraphicsPipeline->DrawCommand(blendMode);

	// VBVとIBVの設定コマンド
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, mVertexBuffer->GetvbViewAddress());

	// マテリアルの描画コマンド
	MaterialDrawCommands();

	// SRVヒープの先頭にあるSRVをルートパラメータ2番に設定
	uint32_t startIndex = mGraphicsPipeline->GetRootSignature()->GetSRVStartIndex();
	renderBase->GetCommandList()->SetGraphicsRootDescriptorTable(startIndex, mTexture->GetBufferResource()->gpuHandle);

	renderBase->GetCommandList()->DrawInstanced((unsigned short)mVertices.size(), 1, 0, 0);
}

// --- マテリアル関連 --------------------------------------------------- //
void CircleGaugeSprite::MaterialInit()
{
	// インスタンス生成
	std::unique_ptr<IConstantBuffer> iConstantBuffer;

	// 2D行列
	iConstantBuffer = std::make_unique<ConstantBuffer<CTransform2D>>();
	mMaterial->constantBuffers.push_back(std::move(iConstantBuffer));

	// 色
	iConstantBuffer = std::make_unique<ConstantBuffer<CColor>>();
	mMaterial->constantBuffers.push_back(std::move(iConstantBuffer));

	// 円ゲージ
	iConstantBuffer = std::make_unique<ConstantBuffer<CCircleGauge>>();
	mMaterial->constantBuffers.push_back(std::move(iConstantBuffer));

	// 初期化
	mMaterial->Init();
}
void CircleGaugeSprite::MaterialTransfer()
{
	// マトリックス
	CTransform2D transform2DData =
	{
		mTransform.GetWorldMat() * Camera::current.GetOrthoGrphicProjectionMat()
	};
	TransferDataToConstantBuffer(mMaterial->constantBuffers[0].get(), transform2DData);

	// 色データ
	CColor colorData = { color.To01() };
	TransferDataToConstantBuffer(mMaterial->constantBuffers[1].get(), colorData);

	// 円ゲージ
	CCircleGauge circleGaugeData = { startRadian ,endRadian };
	TransferDataToConstantBuffer(mMaterial->constantBuffers[2].get(), circleGaugeData);
}
void CircleGaugeSprite::MaterialDrawCommands()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	for (uint32_t i = 0; i < mMaterial->constantBuffers.size(); i++)
	{
		// CBVの設定コマンド
		renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
			i, mMaterial->constantBuffers[i]->bufferResource->buffer->GetGPUVirtualAddress());
	}
}

// --- 頂点データ関連 --------------------------------------------------- //
void CircleGaugeSprite::TransferVertexCoord()
{
	enum { LD, LU, RD, RU };

	// 四辺
	float left = (0.f - mAnchorPoint.x) * mSize.x;
	float right = (1.f - mAnchorPoint.x) * mSize.x;
	float up = (0.f - mAnchorPoint.y) * mSize.y;
	float down = (1.f - mAnchorPoint.y) * mSize.y;

	// 頂点座標
	mVertices[LD].pos = Vec3(left, down, 0.f);	  //左下
	mVertices[LU].pos = Vec3(left, up, 0.f);	  //左上
	mVertices[RD].pos = Vec3(right, down, 0.f);  //右下
	mVertices[RU].pos = Vec3(right, up, 0.f);	  //右上
}
void CircleGaugeSprite::TransferUVCoord(const Vec2 leftTopPos, const Vec2 rightDownPos)
{
	enum { LD, LU, RD, RU };

	// 四辺
	float left = leftTopPos.x / mTexture->size.x;
	float right = rightDownPos.x / mTexture->size.x;
	float up = leftTopPos.y / mTexture->size.y;
	float down = rightDownPos.y / mTexture->size.y;

	// uv座標
	mVertices[LD].uv = Vec2(left, down);	 //左下
	mVertices[LU].uv = Vec2(left, up);		 //左上
	mVertices[RD].uv = Vec2(right, down);	 //右下
	mVertices[RU].uv = Vec2(right, up);	 //右上
}

// --- セッター -------------------------------------------------------- //

// テクスチャー
void CircleGaugeSprite::SetTexture(Texture* texture)
{
	mTexture = texture;
	SetSize(texture->size);
}

// 描画範囲
void CircleGaugeSprite::SetTextureRect(const Vec2 leftTopPos, const Vec2 rightDownPos)
{
	TransferUVCoord(leftTopPos, rightDownPos);
	mVertexBuffer->TransferToBuffer(mVertices);
}

// サイズ
void CircleGaugeSprite::SetSize(const Vec2 size)
{
	mSize = size;

	TransferVertexCoord();
	mVertexBuffer->TransferToBuffer(mVertices);
}

// アンカーポイント
void CircleGaugeSprite::SetAnchorPoint(const Vec2 anchorPoint)
{
	mAnchorPoint = anchorPoint;

	TransferVertexCoord();
	mVertexBuffer->TransferToBuffer(mVertices);
}

// グラフィックスパイプライン
void CircleGaugeSprite::SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline)
{
	mGraphicsPipeline = graphicsPipeline;
}