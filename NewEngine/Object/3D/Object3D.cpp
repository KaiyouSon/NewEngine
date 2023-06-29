#include "Object3D.h"
#include "RenderBase.h"
#include "LightManager.h"
#include "Camera.h"
#include "FbxModel.h"

using namespace ConstantBufferData;

bool Object3D::isAllLighting = false;

Object3D::Object3D() :
	pos(0, 0, 0), scale(1, 1, 1), rot(0, 0, 0),
	graphicsPipeline_(GraphicsPipelineManager::GetGraphicsPipeline("Object3D")),
	texture_(TextureManager::GetTexture("White")),
	isLighting(false)
{
	// マテリアルの初期化
	MaterialInit();

	texture_->isMaterial = true;

	if (isAllLighting == true)
	{
		isLighting = true;
	}
}

void Object3D::Update(Transform* parent)
{
	transform_.pos = pos;
	transform_.scale = scale;
	transform_.rot = rot;
	transform_.Update();

	if (parent != nullptr)
	{
		parent_ = parent;

		Mat4 mat = transform_.GetWorldMat();
		mat *= parent_->GetWorldMat();
		transform_.SetWorldMat(mat);
	}

	// マテリアルの転送
	MaterialTransfer();
}
void Object3D::Draw(const BlendMode blendMode)
{
	if (texture_ == nullptr) return;

	SetBlendMode(blendMode);

	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	renderBase->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// VBVとIBVの設定コマンド
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, model_->mesh.vertexBuffer.GetvbViewAddress());
	renderBase->GetCommandList()->IASetIndexBuffer(model_->mesh.indexBuffer.GetibViewAddress());

	MaterialDrawCommands();

	//renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
	//	5, constantBufferSkin_->constantBuffer->GetGPUVirtualAddress());
	//LightManager::GetInstance()->Draw();

	size_t index = renderBase->GetObject3DRootSignature()->GetConstantBufferNum();
	// SRVヒープの先頭にあるSRVをルートパラメータ2番に設定
	renderBase->GetCommandList()->SetGraphicsRootDescriptorTable((UINT)index, texture_->GetGpuHandle());

	renderBase->GetCommandList()->DrawIndexedInstanced((uint16_t)model_->mesh.indices.size(), 1, 0, 0, 0);
}

// --- マテリアル関連 --------------------------------------------------- //
void Object3D::MaterialInit()
{
	// インスタンス生成
	std::unique_ptr<IConstantBuffer> iConstantBuffer;

	// 3D行列
	iConstantBuffer = std::make_unique<ConstantBuffer<CTransform3D>>();
	material_.constantBuffers.push_back(std::move(iConstantBuffer));

	// マテリアルカラー
	iConstantBuffer = std::make_unique<ConstantBuffer<CMaterialColor>>();
	material_.constantBuffers.push_back(std::move(iConstantBuffer));

	// 色
	iConstantBuffer = std::make_unique<ConstantBuffer<CColor>>();
	material_.constantBuffers.push_back(std::move(iConstantBuffer));

	// スキニング
	iConstantBuffer = std::make_unique<ConstantBuffer<CSkin>>();
	material_.constantBuffers.push_back(std::move(iConstantBuffer));

	// 初期化
	material_.Init();
}
void Object3D::MaterialTransfer()
{
	// マトリックス
	CTransform3D transform3DData =
	{
		Camera::current.GetViewLookToMat() * Camera::current.GetPerspectiveProjectionMat(),
		transform_.GetWorldMat(),
		Camera::current.pos
	};
	TransferDataToConstantBuffer(material_.constantBuffers[0].get(), transform3DData);

	// マテリアルカラー
	CMaterialColor materialColorData;
	if (isLighting == true && isAllLighting == true)
	{
		materialColorData =
		{
			Color(1, 1, 1) - 0.5f,
			//model_->material.ambient,
			model_->material.diffuse,
			model_->material.specular,
		};
	}
	else
	{
		materialColorData = { Color::one, Color::zero, Color::zero };
	}
	TransferDataToConstantBuffer(material_.constantBuffers[1].get(), materialColorData);

	// 色データ
	CColor colorData = { color / 255 };
	TransferDataToConstantBuffer(material_.constantBuffers[2].get(), colorData);

	// スキン情報
	if (model_->format == ModelFormat::Fbx)
	{
		auto fbxModel = static_cast<FbxModel*>(model_);
		fbxModel->PlayAnimetion();

		CSkin skinData{};
		for (uint32_t i = 0; i < fbxModel->bones.size(); i++)
		{
			skinData.bones[i] = fbxModel->bones[i].currentMat;
		}
		TransferDataToConstantBuffer(material_.constantBuffers[3].get(), skinData);
	}
}
void Object3D::MaterialDrawCommands()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	// CBVの設定コマンド
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		0, material_.constantBuffers[0]->constantBuffer->GetGPUVirtualAddress());

	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		1, material_.constantBuffers[1]->constantBuffer->GetGPUVirtualAddress());

	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		2, material_.constantBuffers[2]->constantBuffer->GetGPUVirtualAddress());

	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		3, material_.constantBuffers[3]->constantBuffer->GetGPUVirtualAddress());

}

// --- セッター -------------------------------------------------------- //

//  ブレンド
void Object3D::SetBlendMode(const BlendMode blendMode)
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	switch (blendMode)
	{
	case BlendMode::Alpha: // αブレンド
		renderBase->GetCommandList()->SetPipelineState(graphicsPipeline_->GetAlphaPipeline());
		break;

	case BlendMode::Add:	// 加算ブレンド
		renderBase->GetCommandList()->SetPipelineState(graphicsPipeline_->GetAddPipeline());
		break;

	case BlendMode::Sub:	// 減算ブレンド
		renderBase->GetCommandList()->SetPipelineState(graphicsPipeline_->GetSubPipeline());
		break;

	case BlendMode::Inv:	// 反転
		renderBase->GetCommandList()->SetPipelineState(graphicsPipeline_->GetInvPipeline());
		break;

	default:
		break;
	}
}

// モデル
void Object3D::SetModel(Model* model)
{
	model_ = model;
	texture_ = model_->texture;

	// パイプライン変更
	if (model_->format == ModelFormat::Obj)
	{
		graphicsPipeline_ = GraphicsPipelineManager::GetGraphicsPipeline("Object3D");
	}
	if (model_->format == ModelFormat::Fbx ||
		model_->format == ModelFormat::DFbx)
	{
		//graphicsPipeline_ = GraphicsPipelineManager::GetGraphicsPipeline("Object3D");
		graphicsPipeline_ = GraphicsPipelineManager::GetGraphicsPipeline("FbxModel");
	}
}

// テクスチャー
void Object3D::SetTexture(Texture* texture) { texture_ = texture; }

// グラフィックスパイプライン
void Object3D::SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline) { graphicsPipeline_ = graphicsPipeline; }

// アニメーション
void Object3D::SetAnimation(const uint32_t animationIndex, const uint32_t maxFrame, const bool isPlay)
{
	// スキン情報
	if (model_->format == ModelFormat::Fbx)
	{
		auto fbxModel = static_cast<FbxModel*>(model_);

		fbxModel->animation.index = animationIndex;
		fbxModel->animation.timer.SetLimitTimer(maxFrame);
		fbxModel->animation.isPlay = isPlay;
	}
}

// --- ゲッター -------------------------------------------------------- //

// ワールド座標
Vec3 Object3D::GetWorldPos()
{
	Vec3 worldPos = transform_.GetWorldMat().GetTrans();
	return worldPos;
}

// ワールドスケール
Vec3 Object3D::GetWorldScale()
{
	Vec3 worldScale = transform_.GetWorldMat().GetScale();
	return worldScale;
}

// トランスフォーム
Transform Object3D::GetTransform() { return transform_; }

// モデル
Model* Object3D::GetModel() { return model_; }
