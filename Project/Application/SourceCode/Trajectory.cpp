#include "Trajectory.h"
using namespace VertexBufferData;
using namespace ConstantBufferData;

Trajectory::Trajectory() :
	mTexture(TextureManager::GetTexture("White")),
	moveSpeed(0.01f), tiling(1), offset(0), color(Color::white),
	mVertexBuffer(std::make_unique<VertexBuffer<VSprite>>()),
	mMaterial(std::make_unique<Material>()),
	mGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Trajectory"))
{
	mVertices.resize(4);
	mVertices[LD].uv = { 0.0f,1.0f };
	mVertices[LT].uv = { 0.0f,0.0f };
	mVertices[RD].uv = { 1.0f,1.0f };
	mVertices[RT].uv = { 1.0f,0.0f };
	mVertexBuffer->Create(mVertices);

	// マテリアルの初期化
	MaterialInit();
}

void Trajectory::Update()
{
	mTransform.pos = 0;
	mTransform.scale = 1;
	mTransform.rot = 0;
	mTransform.Update();

	// マテリアルの転送
	MaterialTransfer();

	// 追跡する
	Vec3 v1 = pos[LD] - pos[RD];
	pos[RD] += v1.Norm() * moveSpeed;
	if (v1.Length() <= moveSpeed)
	{
		pos[RD] = pos[LD];
	}

	Vec3 v2 = pos[LT] - pos[RT];
	pos[RT] += v2.Norm() * moveSpeed;
	if (v2.Length() <= moveSpeed)
	{
		pos[RT] = pos[LT];
	}

	mVertices[LD].pos = pos[LD];
	mVertices[LT].pos = pos[LT];
	mVertices[RD].pos = pos[RD];
	mVertices[RT].pos = pos[RT];

	// 頂点データの転送
	mVertexBuffer->TransferToBuffer(mVertices);
}
void Trajectory::Draw(const BlendMode blendMode)
{
	if (mTexture == nullptr)
	{
		return;
	}

	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	// GraphicsPipeline描画コマンド
	mGraphicsPipeline->DrawCommand(blendMode);

	// VBVの設定コマンド
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, mVertexBuffer->GetvbViewAddress());

	// マテリアルの描画コマンド
	MaterialDrawCommands();

	uint32_t startIndex = mGraphicsPipeline->GetRootSignature()->GetSRVStartIndex();
	uint32_t endIndex = mGraphicsPipeline->GetRootSignature()->GetUAVStartIndex();

	for (uint32_t i = startIndex; i < endIndex; i++)
	{
		// SRVヒープの先頭にあるSRVをルートパラメータ2番に設定
		renderBase->GetCommandList()->SetGraphicsRootDescriptorTable(startIndex, mTexture->GetBufferResource()->srvHandle.gpu);
	}

	renderBase->GetCommandList()->DrawInstanced((uint16_t)mVertices.size(), 1, 0, 0);
}

void Trajectory::SetTexture(Texture* texture)
{
	mTexture = texture;
}

// --- マテリアル関連 --------------------------------------------------- //
void Trajectory::MaterialInit()
{
	// インスタンス生成
	std::unique_ptr<IConstantBuffer> iConstantBuffer;

	// 3D行列
	iConstantBuffer = std::make_unique<ConstantBuffer<CTransform3D>>();
	mMaterial->constantBuffers.push_back(std::move(iConstantBuffer));

	// 色
	iConstantBuffer = std::make_unique<ConstantBuffer<CColor>>();
	mMaterial->constantBuffers.push_back(std::move(iConstantBuffer));

	// UVパラメータ
	iConstantBuffer = std::make_unique<ConstantBuffer<CUVParameter>>();
	mMaterial->constantBuffers.push_back(std::move(iConstantBuffer));

	// 初期化
	mMaterial->Init();
}
void Trajectory::MaterialTransfer()
{
	// マトリックス
	CTransform3D transformData =
	{
		Camera::current.GetViewLookToMat() * Camera::current.GetPerspectiveProjectionMat(),
		mTransform.GetWorldMat(),
		Camera::current.pos
	};
	TransferDataToConstantBuffer(mMaterial->constantBuffers[0].get(), transformData);

	// 色データ
	CColor colorData = { color.To01() };
	TransferDataToConstantBuffer(mMaterial->constantBuffers[1].get(), colorData);

	// UVデータ
	CUVParameter uvData = { offset,tiling };
	TransferDataToConstantBuffer(mMaterial->constantBuffers[2].get(), uvData);
}
void Trajectory::MaterialDrawCommands()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	for (uint32_t i = 0; i < mMaterial->constantBuffers.size(); i++)
	{
		// CBVの設定コマンド
		renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
			i, mMaterial->constantBuffers[i]->bufferResource->buffer->GetGPUVirtualAddress());
	}
}