#include "Object3D.h"
#include "RenderBase.h"
#include "LightManager.h"
#include "Camera.h"
#include "FbxModel.h"
#include "ShadowMap.h"

using namespace ConstantBufferData;

bool Object3D::isAllLighting = false;

Object3D::Object3D() :
	pos(0, 0, 0), scale(1, 1, 1), rot(0, 0, 0), offset(0, 0), tiling(1, 1),
	mGraphicsPipeline(GraphicsPipelineManager::GetGraphicsPipeline("Object3D")),
	mTexture(TextureManager::GetTexture("White")),
	mDissolveTex(TextureManager::GetTexture("DissolveTexture")),
	isLighting(false), mIsShadow(false), mCamera(&Camera::current),
	isUseDissolve(false), dissolve(0.f), colorPower(1), dissolveColor(Color::red)
{
	// マテリアルの初期化
	MaterialInit();

	mTexture->isMaterial = true;

	if (isAllLighting == true)
	{
		isLighting = true;
	}
}

void Object3D::Update(Transform* parent)
{
	// カメラが設定してない場合
	if (mCamera == nullptr || mCamera == &Camera::current)
	{
		mCamera = &Camera::current;
	}

	mTransform.pos = pos;
	mTransform.scale = scale;
	mTransform.rot = rot;
	mTransform.Update();

	if (parent != nullptr)
	{
		mParent = parent;

		Mat4 mat = mTransform.GetWorldMat();
		mat *= mParent->GetWorldMat();
		mTransform.SetWorldMat(mat);
	}

	if (mIsShadow == true)
	{
		ShadowMap::Bind(*this);
	}

	// マテリアルの転送
	MaterialTransfer();
}
void Object3D::Draw(const BlendMode blendMode)
{
	if (mTexture == nullptr || mModel == nullptr) return;

	SetBlendMode(blendMode);

	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	renderBase->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// VBVとIBVの設定コマンド
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, mModel->mesh.vertexBuffer.GetvbViewAddress());
	renderBase->GetCommandList()->IASetIndexBuffer(mModel->mesh.indexBuffer.GetibViewAddress());

	MaterialDrawCommands();
	LightManager::GetInstance()->DrawCommand(5);

	size_t index = renderBase->GetObject3DRootSignature()->GetConstantBufferNum();
	// SRVヒープの先頭にあるSRVをルートパラメータ2番に設定
	renderBase->GetCommandList()->SetGraphicsRootDescriptorTable((UINT)index, mTexture->GetGpuHandle());

	//auto tex = TextureManager::GetRenderTexture("CurrentScene")->depthTexture.get();
	//if (isUseDissolve == true)
	//{
		renderBase->GetCommandList()->SetGraphicsRootDescriptorTable((UINT)index + 1, mDissolveTex->GetGpuHandle());
	//}
	//else
	//{
	//	D3D12_RESOURCE_BARRIER  barrier{};
	//	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	//	barrier.Transition.pResource = tex->buffer.Get();
	//	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	//	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_DEPTH_WRITE;
	//	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;
	//	renderBase->GetCommandList()->ResourceBarrier(1, &barrier);

	//	renderBase->GetCommandList()->SetGraphicsRootDescriptorTable((UINT)index + 1, tex->GetGpuHandle());
	//}

	renderBase->GetCommandList()->DrawIndexedInstanced((uint16_t)mModel->mesh.indices.size(), 1, 0, 0, 0);
	//if (isUseDissolve == false)
	//{
	//	D3D12_RESOURCE_BARRIER  barrier{};
	//	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	//	barrier.Transition.pResource = tex->buffer.Get();
	//	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	//	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_GENERIC_READ;
	//	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_DEPTH_WRITE;
	//	renderBase->GetCommandList()->ResourceBarrier(1, &barrier);
	//}
}

// --- マテリアル関連 --------------------------------------------------- //
void Object3D::MaterialInit()
{
	// インスタンス生成
	std::unique_ptr<IConstantBuffer> iConstantBuffer;

	// 3D行列
	iConstantBuffer = std::make_unique<ConstantBuffer<CTransform3D>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// マテリアルカラー
	iConstantBuffer = std::make_unique<ConstantBuffer<CMaterialColor>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// 色
	iConstantBuffer = std::make_unique<ConstantBuffer<CColor>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// スキニング
	iConstantBuffer = std::make_unique<ConstantBuffer<CSkin>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// UV情報
	iConstantBuffer = std::make_unique<ConstantBuffer<CUVParameter>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// UV情報
	iConstantBuffer = std::make_unique<ConstantBuffer<CDissolve>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// 初期化
	mMaterial.Init();
}
void Object3D::MaterialTransfer()
{
	// マトリックス
	CTransform3D transform3DData =
	{
		mCamera->GetViewLookToMat() * mCamera->GetPerspectiveProjectionMat(),
		mTransform.GetWorldMat(),
		mCamera->pos
	};
	TransferDataToConstantBuffer(mMaterial.constantBuffers[0].get(), transform3DData);

	if (mModel == nullptr)
	{
		return;
	}

	// マテリアルカラー
	CMaterialColor materialColorData;
	if (isLighting == true && isAllLighting == true)
	{
		materialColorData =
		{
			Color(1, 1, 1) - 0.5f,
			mModel->material.diffuse,
			mModel->material.specular,
		};
	}
	else
	{
		materialColorData = { Color::one, Color::zero, Color::zero };
	}
	TransferDataToConstantBuffer(mMaterial.constantBuffers[1].get(), materialColorData);

	// 色データ
	CColor colorData = { color / 255 };
	TransferDataToConstantBuffer(mMaterial.constantBuffers[2].get(), colorData);

	// スキン情報
	if (mModel->format == ModelFormat::Fbx)
	{
		auto fbxModel = static_cast<FbxModel*>(mModel);
		fbxModel->PlayAnimetion();

		CSkin skinData{};
		for (uint32_t i = 0; i < fbxModel->bones.size(); i++)
		{
			skinData.bones[i] = fbxModel->bones[i].currentMat;
		}
		TransferDataToConstantBuffer(mMaterial.constantBuffers[3].get(), skinData);
	}

	// UVデータ
	CUVParameter uvData = { offset,tiling };
	TransferDataToConstantBuffer(mMaterial.constantBuffers[4].get(), uvData);

	// ディゾルブ
	CDissolve dissolveData = { dissolve,colorPower,Vec2(0,0), dissolveColor.To01() };
	TransferDataToConstantBuffer(mMaterial.constantBuffers[5].get(), dissolveData);
}
void Object3D::MaterialDrawCommands()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	// CBVの設定コマンド
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		0, mMaterial.constantBuffers[0]->constantBuffer->GetGPUVirtualAddress());

	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		1, mMaterial.constantBuffers[1]->constantBuffer->GetGPUVirtualAddress());

	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		2, mMaterial.constantBuffers[2]->constantBuffer->GetGPUVirtualAddress());

	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		3, mMaterial.constantBuffers[3]->constantBuffer->GetGPUVirtualAddress());

	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		4, mMaterial.constantBuffers[4]->constantBuffer->GetGPUVirtualAddress());

	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		6, mMaterial.constantBuffers[5]->constantBuffer->GetGPUVirtualAddress());
}

// --- セッター -------------------------------------------------------- //

//  ブレンド
void Object3D::SetBlendMode(const BlendMode blendMode)
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	switch (blendMode)
	{
	case BlendMode::Alpha: // αブレンド
		renderBase->GetCommandList()->SetPipelineState(mGraphicsPipeline->GetAlphaPipeline());
		break;

	case BlendMode::Add:	// 加算ブレンド
		renderBase->GetCommandList()->SetPipelineState(mGraphicsPipeline->GetAddPipeline());
		break;

	case BlendMode::Sub:	// 減算ブレンド
		renderBase->GetCommandList()->SetPipelineState(mGraphicsPipeline->GetSubPipeline());
		break;

	case BlendMode::Inv:	// 反転
		renderBase->GetCommandList()->SetPipelineState(mGraphicsPipeline->GetInvPipeline());
		break;

	default:
		break;
	}
}

// モデル
void Object3D::SetModel(Model* model)
{
	mModel = model;
	mTexture = mModel->texture;

	// パイプライン変更
	if (mModel->format == ModelFormat::Obj)
	{
		mGraphicsPipeline = GraphicsPipelineManager::GetGraphicsPipeline("Object3D");
	}
	if (mModel->format == ModelFormat::Fbx ||
		mModel->format == ModelFormat::DFbx)
	{
		//mGraphicsPipeline = GraphicsPipelineManager::GetGraphicsPipeline("Object3D");
		mGraphicsPipeline = GraphicsPipelineManager::GetGraphicsPipeline("FbxModel");
	}
}

// テクスチャー
void Object3D::SetTexture(Texture* texture) { mTexture = texture; }

// グラフィックスパイプライン
void Object3D::SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline) { mGraphicsPipeline = graphicsPipeline; }

// アニメーション
void Object3D::SetAnimation(const uint32_t animationIndex, const uint32_t maxFrame, const bool isPlay)
{
	// スキン情報
	if (mModel->format == ModelFormat::Fbx)
	{
		auto fbxModel = static_cast<FbxModel*>(mModel);

		fbxModel->animation.index = animationIndex;
		fbxModel->animation.timer.SetLimitTimer(maxFrame);
		fbxModel->animation.isPlay = isPlay;
	}
}

// カメラ
void Object3D::SetCamera(Camera* camera)
{
	mCamera = camera;
}

void Object3D::SetisShadow(const bool isShadow)
{
	mIsShadow = isShadow;

	if (mIsShadow == true)
	{
		ShadowMap::Register();
	}
}

// --- ゲッター -------------------------------------------------------- //

// ワールド座標
Vec3 Object3D::GetWorldPos()
{
	Vec3 worldPos = Vec3MulMat4(pos, mTransform.GetWorldMat(), true);
	return worldPos;
}

// ワールドスケール
Vec3 Object3D::GetWorldScale()
{
	Vec3 worldScale = mTransform.GetWorldMat().GetScale();
	return worldScale;
}

// トランスフォーム
Transform Object3D::GetTransform() { return mTransform; }

// モデル
Model* Object3D::GetModel() { return mModel; }
