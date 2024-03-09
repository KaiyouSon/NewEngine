#include "Trajectory.h"
using namespace VertexBufferData;
using namespace ConstantBufferData;

Trajectory::Trajectory() :
	mTexture(TextureManager::GetTexture("White")),
	tiling(1), offset(0), color(Color::white),
	mVertexBuffer(std::make_unique<VertexBuffer<VSprite>>()),
	mMaterial(std::make_unique<Material>()),
	mGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Trajectory"))
{
	mVertices.resize(20);
	// 上下に点を置くから半分になる
	uint32_t halfSize = (uint32_t)(mVertices.size() / 2);
	for (uint32_t i = 0; i < halfSize; i++)
	{
		// 上下
		for (uint32_t j = 0; j < 2; j++)
		{
			// 間隔数
			uint32_t spaceCount = halfSize - 1;

			Vec2 uv;
			uv.x = i * (1.f / spaceCount);
			uv.y = (float)i;

			// 上下の点
			mVertices[i * 2 + j].uv.x = i * (1.f / spaceCount);
			mVertices[i * 2 + j].uv.y = 1 - (float)j;
		}
	}
	mVertexBuffer->Create(mVertices);

	// マテリアルの初期化
	MaterialInit();

	mPrevPos.resize(mVertices.size());
}

void Trajectory::PrevUpdate()
{
	for (uint32_t i = 0; i < mVertices.size(); i++)
	{
		mPrevPos[i] = mVertices[i].pos;
	}
}

void Trajectory::PostUpdate()
{
	mTransform.pos = 0;
	mTransform.scale = 1;
	mTransform.rot = 0;
	mTransform.Update();

	// マテリアルの転送
	MaterialTransfer();

	// 上下に点を置くから半分になる
	uint32_t halfSize = (uint32_t)(mVertices.size() / 2);
	for (uint32_t i = 0; i < halfSize; i++)
	{
		// 最初の二点
		if (i == 0)
		{
			mVertices[Down].pos = mTargetPos[Down];
			mVertices[Top].pos = mTargetPos[Top];
		}
		else
		{
			// 上下
			for (uint32_t j = 0; j < 2; j++)
			{
				uint32_t index = i * 2 + j;
				Vec3 target = mPrevPos[index - 2];
				Vec3 current = mVertices[index].pos;
				Vec3 v = target - current;
				mVertices[index].pos = target;
			}
		}
	}

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
		Camera::current.GetViewMat() * Camera::current.GetPerspectiveMat(),
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

void Trajectory::SetTexture(Texture* texture)
{
	mTexture = texture;
}

void Trajectory::SetTargetPos(const Vec3 downPos, const Vec3 topPos)
{
	mTargetPos[Down] = downPos;
	mTargetPos[Top] = topPos;
}

Vec3 Trajectory::GetDownPos()
{
	return mTargetPos[Down];
}

Vec3 Trajectory::GetTopPos()
{
	return mTargetPos[Top];
}
