#include "Object3D.h"
#include "RenderBase.h"
#include "LightManager.h"
#include "Camera.h"
#include "FbxModel.h"
#include "TextureManager.h"
#include <DirectXMath.h>

bool Object3D::isAllLighting_ = false;

#pragma region �R���X�g���N�^

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
	// �萔�o�b�t�@������
	constantBufferTransform_->Init();	// 3D�s��
	constantBufferMaterial_->Init();		// �}�e���A��
	constantBufferColor_->Init();		// �F
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
	// �萔�o�b�t�@������
	constantBufferTransform_->Init();	// 3D�s��
	constantBufferMaterial_->Init();		// �}�e���A��
	constantBufferColor_->Init();		// �F
	constantBufferSkin_->Init();

	texture_->isMaterial = true;

	if (isAllLighting_ == true)
	{
		isLighting_ = true;
	}
}

#pragma endregion

#pragma region ���̑��̏���

void Object3D::PlayAnimetion()
{
	if (model_->modelType == "FBX")
	{
		// �{�[���z��
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

	// VBV��IBV�̐ݒ�R�}���h
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, model_->mesh.GetVertexBuffer().GetvbViewAddress());
	renderBase->GetCommandList()->IASetIndexBuffer(model_->mesh.GetIndexBuffer().GetibViewAddress());

	// CBV�̐ݒ�R�}���h
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
	// SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^2�Ԃɐݒ�
	renderBase->GetCommandList()->SetGraphicsRootDescriptorTable(
		UINT(index), texture_->GetGpuHandle());

	renderBase->GetCommandList()->DrawIndexedInstanced(
		(unsigned short)model_->mesh.GetIndexSize(), 1, 0, 0, 0);
}

// �o�b�t�@�]��
void Object3D::TransferBuffer()
{
	// �}�g���b�N�X�]��
	constantBufferTransform_->constantBufferMap->viewMat =
		Camera::current.GetViewLookToMat() *
		Camera::current.GetPerspectiveProjectionMat();
	constantBufferTransform_->constantBufferMap->worldMat = transform_.GetWorldMat();
	constantBufferTransform_->constantBufferMap->cameraPos = Camera::current.pos;

	// �}�e���A���̓]��
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

	// �F�]��
	constantBufferColor_->constantBufferMap->color = color_ / 255;

	if (model_->modelType == "FBX")
	{
		// �{�[���z��
		auto fbxModel = static_cast<FbxModel*>(model_);
		std::vector<FbxModelBone>& bones = fbxModel->bones;

		// �]��
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

//  �u�����h�ݒ�
void Object3D::SetBlendMode(const BlendMode blendMode)
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	switch (blendMode)
	{
	case BlendMode::Alpha: // ���u�����h
		renderBase->GetCommandList()->SetPipelineState(graphicsPipeline_->GetAlphaPipeline());
		break;

	case BlendMode::Add:	// ���Z�u�����h
		renderBase->GetCommandList()->SetPipelineState(graphicsPipeline_->GetAddPipeline());
		break;

	case BlendMode::Sub:	// ���Z�u�����h
		renderBase->GetCommandList()->SetPipelineState(graphicsPipeline_->GetSubPipeline());
		break;

	case BlendMode::Inv:	// ���]
		renderBase->GetCommandList()->SetPipelineState(graphicsPipeline_->GetInvPipeline());
		break;

	default:
		break;
	}
}

#pragma endregion

#pragma region �Z�b�^�[

// ���f��
void Object3D::SetModel(Model* model)
{
	model_ = model;
	texture_ = &model_->material.texture;

	// �p�C�v���C���ύX
	if (model_->modelType == "OBJ")
	{
		graphicsPipeline_ = GraphicsPipelineManager::GetGraphicsPipeline("Object3D");
	}
	if (model_->modelType == "FBX")
	{
		graphicsPipeline_ = GraphicsPipelineManager::GetGraphicsPipeline("FbxModel");
	}
}

// �e�N�X�`���[
void Object3D::SetTexture(Texture* texture) { texture_ = texture; }

// �O���t�B�b�N�X�p�C�v���C��
void Object3D::SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline) { graphicsPipeline_ = graphicsPipeline; }

// �F
void Object3D::SetColor(const Color color) { color_ = color; }

#pragma endregion

#pragma region �Q�b�^�[

// ���[���h���W
Vec3 Object3D::GetWorldPos()
{
	Vec3 worldPos = transform_.GetWorldMat().GetTrans();
	return worldPos;
}

// ���[���h�X�P�[��
Vec3 Object3D::GetWorldScale()
{
	Vec3 worldScale = transform_.GetWorldMat().GetScale();
	return worldScale;
}

// �g�����X�t�H�[��
Transform Object3D::GetTransform() { return transform_; }

// ���f��
Model* Object3D::GetModel() { return model_; }

#pragma endregion