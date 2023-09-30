#include "ColliderObject.h"
#include "RenderBase.h"
#include "ColliderDrawer.h"
#include "Collider.h"

using namespace ConstantBufferData;

ColliderObject::ColliderObject() :
	mGraphicsPipeline(PipelineManager::GetGraphicsPipeline("ColliderObject")),
	model(nullptr), mCamera(&Camera::current), is3D(true)
{
	// マテリアルの初期化
	MaterialInit();
}

void ColliderObject::Update()
{
	// カメラが設定してない場合
	if (mCamera == nullptr || mCamera == &Camera::current)
	{
		mCamera = &Camera::current;
	}

	if (model == nullptr)
	{
		return;
	}

	transform.Update();

	// マテリアルの転送
	MaterialTransfer();
}
void ColliderObject::Draw()
{
	if (model == nullptr)
	{
		return;
	}

	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	// GraphicsPipeline描画コマンド
	mGraphicsPipeline->DrawCommand(BlendMode::Alpha);

	// VBVとIBVの設定コマンド
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, model->mesh.vertexBuffer.GetvbViewAddress());
	renderBase->GetCommandList()->IASetIndexBuffer(model->mesh.indexBuffer.GetibViewAddress());

	MaterialDrawCommands();

	renderBase->GetCommandList()->DrawIndexedInstanced((uint16_t)model->mesh.indices.size(), 1, 0, 0, 0);
}

// --- マテリアル関連 --------------------------------------------------- //
void ColliderObject::MaterialInit()
{
	// インスタンス生成
	std::unique_ptr<IConstantBuffer> iConstantBuffer;

	// 3D行列
	iConstantBuffer = std::make_unique<ConstantBuffer<CTransformCollider>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// 色
	iConstantBuffer = std::make_unique<ConstantBuffer<CColor>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// 初期化
	mMaterial.Init();
}
void ColliderObject::MaterialTransfer()
{
	// マトリックス
	CTransformCollider transformColliderData =
	{
		is3D == true ?
		// 3Dの場合
		transform.GetWorldMat() *
		mCamera->GetViewLookToMat() *
		mCamera->GetPerspectiveProjectionMat():
		// 2Dの場合
		transform.GetWorldMat() *
		mCamera->GetOrthoGrphicProjectionMat()
	};
	TransferDataToConstantBuffer(mMaterial.constantBuffers[0].get(), transformColliderData);

	// 色データ
	CColor colorData = { color / 255 };
	TransferDataToConstantBuffer(mMaterial.constantBuffers[1].get(), colorData);
}
void ColliderObject::MaterialDrawCommands()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	for (uint32_t i = 0; i < mMaterial.constantBuffers.size(); i++)
	{
		// CBVの設定コマンド
		renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
			i, mMaterial.constantBuffers[i]->bufferResource->buffer->GetGPUVirtualAddress());
	}
}

// --- セッター -------------------------------------------------------- //

// カメラ
void ColliderObject::SetCamera(Camera* camera)
{
	mCamera = camera;
}
