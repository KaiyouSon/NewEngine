#include "Line.h"
#include "Camera.h"
#include "RenderBase.h"

using namespace VertexBufferData;
using namespace ConstantBufferData;

Line::Line() :
	mVertexBuffer(std::make_unique<VertexBuffer<VLine>>()),
	mGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Line")),
	startPos(-1), endPos(1)
{
	mVertices.resize(2);
	mVertices[0] = { { 0.f,0.f,0.f} };
	mVertices[1] = { { 1.f,0.f,0.f} };
	mVertexBuffer->Create(mVertices);

	// マテリアルの初期化
	MaterialInit();
}

void Line::Update(Transform* parent)
{
	mTransform.pos = pos;
	mTransform.scale = 1;
	mTransform.rot = rot;
	mTransform.Update();

	if (parent != nullptr)
	{
		mParent = parent;

		Mat4 mat = mTransform.GetWorldMat();
		mat *= mParent->GetWorldMat();
		mTransform.SetWorldMat(mat);
	}

	// マテリアルの転送
	MaterialTransfer();

	mVertices[0].pos = startPos;
	mVertices[1].pos = endPos;

	// 頂点データの転送
	mVertexBuffer->TransferToBuffer(mVertices);
}
void Line::Draw()
{
	RenderBase* renderBase = RenderBase::GetInstance();

	// GraphicsPipeline描画コマンド
	mGraphicsPipeline->DrawCommand(BlendMode::Alpha);

	// VBVとIBVの設定コマンド
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, mVertexBuffer->GetvbViewAddress());

	// マテリアルの描画コマンド
	MaterialDrawCommands();

	// 描画コマンド
	renderBase->GetCommandList()->DrawInstanced((UINT)mVertices.size(), 1, 0, 0);
}

// --- マテリアル関連 --------------------------------------------------- //
void Line::MaterialInit()
{
	// インスタンス生成
	std::unique_ptr<IConstantBuffer> iConstantBuffer;

	// 2D行列
	iConstantBuffer = std::make_unique<ConstantBuffer<CTransform3D>>();
	mMaterial->constantBuffers.push_back(std::move(iConstantBuffer));

	// 色
	iConstantBuffer = std::make_unique<ConstantBuffer<CColor>>();
	mMaterial->constantBuffers.push_back(std::move(iConstantBuffer));

	// 初期化
	mMaterial->Init();
}
void Line::MaterialTransfer()
{
	// マトリックス
	CTransform3D transform3DData =
	{
		Camera::current.GetViewLookToMat() * Camera::current.GetPerspectiveProjectionMat(),
		mTransform.GetWorldMat(),
		Camera::current.pos
	};
	TransferDataToConstantBuffer(mMaterial->constantBuffers[0].get(), transform3DData);

	// 色データ
	CColor colorData = { color.To01() };
	TransferDataToConstantBuffer(mMaterial->constantBuffers[1].get(), colorData);
}
void Line::MaterialDrawCommands()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	for (uint32_t i = 0; i < mMaterial->constantBuffers.size(); i++)
	{
		// CBVの設定コマンド
		renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
			0, mMaterial->constantBuffers[0]->bufferResource->buffer->GetGPUVirtualAddress());
	}
}
