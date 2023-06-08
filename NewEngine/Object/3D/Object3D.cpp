#include "Object3D.h"
#include "RenderBase.h"
#include "LightManager.h"
#include "Camera.h"
#include "FbxModel.h"
#include "TextureManager.h"
#include <DirectXMath.h>

bool Object3D::isAllLighting_ = false;

#pragma region コンストラクタ

Object3D::Object3D() :
	pos_(0, 0, 0), scale_(1, 1, 1), rot_(0, 0, 0),
	constantBufferTransform_(std::make_unique<ConstantBuffer<ConstantBufferDataTransform3D>>()),
	constantBufferMaterial_(std::make_unique<ConstantBuffer<ConstantBufferDataMaterial>>()),
	constantBufferColor_(std::make_unique<ConstantBuffer<ConstantBufferDataColor>>()),
	constantBufferSkin_(std::make_unique<ConstantBuffer<ConstantBufferDataSkin>>()),
	graphicsPipeline_(GraphicsPipelineManager::GetGraphicsPipeline("Object3D")),
	texture_(TextureManager::GetTexture("White")),
	isLighting_(false)
{
	// 定数バッファ初期化
	constantBufferTransform_->Init();	// 3D行列
	constantBufferMaterial_->Init();		// マテリアル
	constantBufferColor_->Init();		// 色
	constantBufferSkin_->Init();

	texture_->isMaterial = true;

	if (isAllLighting_ == true)
	{
		isLighting_ = true;
	}
}

Object3D::Object3D(Model* model) :
	pos_(0), scale_(1), rot_(0),
	constantBufferTransform_(std::make_unique<ConstantBuffer<ConstantBufferDataTransform3D>>()),
	constantBufferMaterial_(std::make_unique<ConstantBuffer<ConstantBufferDataMaterial>>()),
	constantBufferColor_(std::make_unique<ConstantBuffer<ConstantBufferDataColor>>()),
	constantBufferSkin_(std::make_unique<ConstantBuffer<ConstantBufferDataSkin>>()),
	graphicsPipeline_(GraphicsPipelineManager::GetGraphicsPipeline("Object3D")),
	texture_(TextureManager::GetTexture("White")),
	isLighting_(false), model_(model)
{
	// 定数バッファ初期化
	constantBufferTransform_->Init();	// 3D行列
	constantBufferMaterial_->Init();		// マテリアル
	constantBufferColor_->Init();		// 色
	constantBufferSkin_->Init();

	texture_->isMaterial = true;

	if (isAllLighting_ == true)
	{
		isLighting_ = true;
	}
}

#pragma endregion

#pragma region その他の処理

void Object3D::PlayAnimetion()
{
	if (model_->modelType == "FBX")
	{
		// ボーン配列
		auto fbxModel = static_cast<FbxModel*>(model_);
		fbxModel->PlayAnimetion();
	}
}
void Object3D::Update(Transform* parent)
{
	transform_.pos = pos_;
	transform_.scale = scale_;
	transform_.rot = rot_;
	transform_.Update();

	if (parent != nullptr)
	{
		parent_ = parent;

		Mat4 mat = transform_.GetWorldMat();
		mat *= parent_->GetWorldMat();
		transform_.SetWorldMat(mat);
	}

	TransferBuffer();
}
void Object3D::Draw(const BlendMode blendMode)
{
	if (texture_ == nullptr) return;

	SetBlendMode(blendMode);

	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	renderBase->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// VBVとIBVの設定コマンド
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, model_->mesh.GetVertexBuffer().GetvbViewAddress());
	renderBase->GetCommandList()->IASetIndexBuffer(model_->mesh.GetIndexBuffer().GetibViewAddress());

	// CBVの設定コマンド
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		0, constantBufferTransform_->constantBuffer->GetGPUVirtualAddress());
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		1, constantBufferMaterial_->constantBuffer->GetGPUVirtualAddress());
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		2, constantBufferColor_->constantBuffer->GetGPUVirtualAddress());
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		5, constantBufferSkin_->constantBuffer->GetGPUVirtualAddress());
	LightManager::GetInstance()->Draw();

	size_t index = renderBase->GetObject3DRootSignature()->GetConstantBufferNum();
	// SRVヒープの先頭にあるSRVをルートパラメータ2番に設定
	renderBase->GetCommandList()->SetGraphicsRootDescriptorTable(
		UINT(index), texture_->GetGpuHandle());

	renderBase->GetCommandList()->DrawIndexedInstanced(
		(unsigned short)model_->mesh.GetIndexSize(), 1, 0, 0, 0);
}

// バッファ転送
void Object3D::TransferBuffer()
{
	// マトリックス転送
	constantBufferTransform_->constantBufferMap->viewMat =
		Camera::current.GetViewLookToMat() *
		Camera::current.GetPerspectiveProjectionMat();
	constantBufferTransform_->constantBufferMap->worldMat = transform_.GetWorldMat();
	constantBufferTransform_->constantBufferMap->cameraPos = Camera::current.pos;

	// マテリアルの転送
	if (isLighting_ == true && isAllLighting_ == true)
	{
		constantBufferMaterial_->constantBufferMap->ambient = Vec3::one - 0.5f;
		//constantBufferMaterial_->constantBufferMap->ambient = model->material.ambient;

		constantBufferMaterial_->constantBufferMap->diffuse = model_->material.diffuse;
		constantBufferMaterial_->constantBufferMap->specular = model_->material.specular;
	}
	else
	{
		constantBufferMaterial_->constantBufferMap->ambient = Vec3::one;
		constantBufferMaterial_->constantBufferMap->diffuse = Vec3::zero;
		constantBufferMaterial_->constantBufferMap->specular = Vec3::zero;
	}
	constantBufferMaterial_->constantBufferMap->alpha = model_->material.alpha;

	// 色転送
	constantBufferColor_->constantBufferMap->color = color_ / 255;

	if (model_->modelType == "FBX")
	{
		// ボーン配列
		auto fbxModel = static_cast<FbxModel*>(model_);
		std::vector<FbxModelBone>& bones = fbxModel->bones;

		// 転送
		for (size_t i = 0; i < bones.size(); i++)
		{
			Mat4 currentPoseMat;
			FbxAMatrix currentPoseFbxMat =
				bones[i].fbxCluster->GetLink()->EvaluateGlobalTransform(fbxModel->fbxAnimetion.currentTime);
			ConvertMat4FromFbx(&currentPoseMat, currentPoseFbxMat);

			constantBufferSkin_->constantBufferMap->bones[i] = bones[i].invInitPoseMat * currentPoseMat;
		}
	}
}

//  ブレンド設定
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

#pragma endregion

#pragma region セッター

// モデル
void Object3D::SetModel(Model* model)
{
	model_ = model;
	texture_ = &model_->material.texture;

	// パイプライン変更
	if (model_->modelType == "OBJ")
	{
		graphicsPipeline_ = GraphicsPipelineManager::GetGraphicsPipeline("Object3D");
	}
	if (model_->modelType == "FBX")
	{
		graphicsPipeline_ = GraphicsPipelineManager::GetGraphicsPipeline("FbxModel");
	}
}

// テクスチャー
void Object3D::SetTexture(Texture* texture) { texture_ = texture; }

// グラフィックスパイプライン
void Object3D::SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline) { graphicsPipeline_ = graphicsPipeline; }

// 色
void Object3D::SetColor(const Color color) { color_ = color; }

#pragma endregion

#pragma region ゲッター

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

#pragma endregion