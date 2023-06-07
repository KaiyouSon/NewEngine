#include "Object3D.h"
#include "RenderBase.h"
#include "LightManager.h"
#include "Camera.h"
#include "FbxModel.h"
#include "TextureManager.h"
#include <DirectXMath.h>

bool Object3D::isAllLighting = false;

#pragma region その他の処理

Object3D::Object3D() :
	pos(0, 0, 0), scale(1, 1, 1), rot(0, 0, 0),
	constantBufferTransform(new ConstantBuffer<ConstantBufferDataTransform3D>),
	constantBufferMaterial(new ConstantBuffer<ConstantBufferDataMaterial>),
	constantBufferColor(new ConstantBuffer<ConstantBufferDataColor>),
	constantBufferSkin(new ConstantBuffer<ConstantBufferDataSkin>),
	graphicsPipeline(GraphicsPipelineManager::GetGraphicsPipeline("Object3D")),
	texture(TextureManager::GetTexture("White")),
	isLighting(false)
{
	// 定数バッファ初期化
	constantBufferTransform->Init();	// 3D行列
	constantBufferMaterial->Init();		// マテリアル
	constantBufferColor->Init();		// 色
	constantBufferSkin->Init();

	texture->isMaterial = true;

	if (isAllLighting == true)
	{
		isLighting = true;
	}
}
Object3D::~Object3D()
{
	delete constantBufferTransform;
	delete constantBufferMaterial;
	delete constantBufferColor;
	delete constantBufferSkin;
}
void Object3D::PlayAnimetion()
{
	if (model->modelType == "FBX")
	{
		// ボーン配列
		auto fbxModel = static_cast<FbxModel*>(model);
		fbxModel->PlayAnimetion();
	}
}
void Object3D::Update()
{
	if (texture->isMaterial == true)
	{
		texture = &model->material.texture;
		if (model->modelType == "OBJ")
		{
			graphicsPipeline = GraphicsPipelineManager::GetGraphicsPipeline("Object3D");
		}
		if (model->modelType == "FBX")
		{
			graphicsPipeline = GraphicsPipelineManager::GetGraphicsPipeline("FbxModel");
		}
	}

	transform_.pos = pos;
	transform_.scale = scale;
	transform_.rot = rot;
	transform_.Update();

	// マトリックス転送
	constantBufferTransform->constantBufferMap->viewMat =
		Camera::current.GetViewLookToMat() *
		Camera::current.GetPerspectiveProjectionMat();
	constantBufferTransform->constantBufferMap->worldMat = transform_.worldMat_;
	constantBufferTransform->constantBufferMap->cameraPos = Camera::current.pos;

	// マテリアルの転送
	if (isLighting == true && isAllLighting == true)
	{
		constantBufferMaterial->constantBufferMap->ambient = Vec3::one - 0.5f;
		//constantBufferMaterial->constantBufferMap->ambient = model->material.ambient;

		constantBufferMaterial->constantBufferMap->diffuse = model->material.diffuse;
		constantBufferMaterial->constantBufferMap->specular = model->material.specular;
	}
	else
	{
		constantBufferMaterial->constantBufferMap->ambient = Vec3::one;
		constantBufferMaterial->constantBufferMap->diffuse = Vec3::zero;
		constantBufferMaterial->constantBufferMap->specular = Vec3::zero;
	}
	constantBufferMaterial->constantBufferMap->alpha = model->material.alpha;

	// 色転送
	constantBufferColor->constantBufferMap->color = color / 255;
	constantBufferColor->constantBufferMap->color.a = color.a / 255;

	if (model->modelType == "FBX")
	{
		// ボーン配列
		auto fbxModel = static_cast<FbxModel*>(model);
		std::vector<FbxModelBone>& bones = fbxModel->bones;

		// 転送
		for (size_t i = 0; i < bones.size(); i++)
		{
			Mat4 currentPoseMat;
			FbxAMatrix currentPoseFbxMat =
				bones[i].fbxCluster->GetLink()->EvaluateGlobalTransform(fbxModel->fbxAnimetion.currentTime);
			ConvertMat4FromFbx(&currentPoseMat, currentPoseFbxMat);

			constantBufferSkin->constantBufferMap->bones[i] = bones[i].invInitPoseMat * currentPoseMat;
		}
	}
}
void Object3D::Draw(const BlendMode blendMode)
{
	SetBlendMode(blendMode);

	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	renderBase->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// VBVとIBVの設定コマンド
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, model->mesh.GetVertexBuffer().GetvbViewAddress());
	renderBase->GetCommandList()->IASetIndexBuffer(model->mesh.GetIndexBuffer().GetibViewAddress());

	// CBVの設定コマンド
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		0, constantBufferTransform->constantBuffer->GetGPUVirtualAddress());
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		1, constantBufferMaterial->constantBuffer->GetGPUVirtualAddress());
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		2, constantBufferColor->constantBuffer->GetGPUVirtualAddress());
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		5, constantBufferSkin->constantBuffer->GetGPUVirtualAddress());
	LightManager::GetInstance()->Draw();

	size_t index = renderBase->GetObject3DRootSignature()->GetConstantBufferNum();
	// SRVヒープの先頭にあるSRVをルートパラメータ2番に設定
	renderBase->GetCommandList()->SetGraphicsRootDescriptorTable(
		UINT(index), texture->GetGpuHandle());

	renderBase->GetCommandList()->DrawIndexedInstanced(
		(unsigned short)model->mesh.GetIndexSize(), 1, 0, 0, 0);
}

#pragma endregion

#pragma region セッター

// 親関係
void Object3D::SetParent(Transform* transform)
{
	Mat4 mat = transform_.GetWorldMat();
	mat *= transform->GetWorldMat();
	transform_.SetWorldMat(mat);
}

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

#pragma endregion


// ブレンドモード
void Object3D::SetBlendMode(const BlendMode blendMode)
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	switch (blendMode)
	{
	case BlendMode::Alpha: // αブレンド
		renderBase->GetCommandList()->SetPipelineState(graphicsPipeline->GetAlphaPipeline());
		break;

	case BlendMode::Add:	// 加算ブレンド
		renderBase->GetCommandList()->SetPipelineState(graphicsPipeline->GetAddPipeline());
		break;

	case BlendMode::Sub:	// 減算ブレンド
		renderBase->GetCommandList()->SetPipelineState(graphicsPipeline->GetSubPipeline());
		break;

	case BlendMode::Inv:	// 反転
		renderBase->GetCommandList()->SetPipelineState(graphicsPipeline->GetInvPipeline());
		break;

	default:
		break;
	}
}

