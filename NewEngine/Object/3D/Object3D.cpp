#include "Object3D.h"
#include "RenderBase.h"
#include "LightManager.h"
#include "Camera.h"
#include "FbxModel.h"
#include "TextureManager.h"

using namespace ConstantBufferData;

bool Object3D::isAllLighting = false;

Object3D::Object3D() :
	pos(0, 0, 0), scale(1, 1, 1), rot(0, 0, 0),
	graphicsPipeline_(GraphicsPipelineManager::GetGraphicsPipeline("Object3D")),
	texture_(TextureManager::GetTexture("White")),
	isLighting(false)
{
	// �}�e���A���̏�����
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

	MaterialTransfer();

	TransferBuffer();
}
void Object3D::Draw(const BlendMode blendMode)
{
	if (texture_ == nullptr) return;

	SetBlendMode(blendMode);

	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	renderBase->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// VBV��IBV�̐ݒ�R�}���h
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, model_->mesh.vertexBuffer.GetvbViewAddress());
	renderBase->GetCommandList()->IASetIndexBuffer(model_->mesh.indexBuffer.GetibViewAddress());

	MaterialDrawCommands();

	//renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
	//	5, constantBufferSkin_->constantBuffer->GetGPUVirtualAddress());
	//LightManager::GetInstance()->Draw();

	size_t index = renderBase->GetObject3DRootSignature()->GetConstantBufferNum();
	// SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^2�Ԃɐݒ�
	renderBase->GetCommandList()->SetGraphicsRootDescriptorTable((UINT)index, texture_->GetGpuHandle());

	renderBase->GetCommandList()->DrawIndexedInstanced((uint16_t)model_->mesh.indices.size(), 1, 0, 0, 0);
}

// �o�b�t�@�]��
void Object3D::TransferBuffer()
{
	//if (model_->modelType == "FBX")
	//{
	//	// �{�[���z��
	//	auto fbxModel = static_cast<FbxModel1*>(model_);
	//	std::vector<FbxModelBone>& bones = fbxModel->bones;

	//	// �]��
	//	for (size_t i = 0; i < bones.size(); i++)
	//	{
	//		Mat4 currentPoseMat;
	//		FbxAMatrix currentPoseFbxMat =
	//			bones[i].fbxCluster->GetLink()->EvaluateGlobalTransform(fbxModel->fbxAnimetion.currentTime);
	//		ConvertMat4FromFbx(&currentPoseMat, currentPoseFbxMat);

	//		constantBufferSkin_->constantBufferMap->bones[i] = bones[i].invInitPoseMat * currentPoseMat;
	//	}
	//}
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

// --- �}�e���A���֘A --------------------------------------------------- //
void Object3D::MaterialInit()
{
	// �C���X�^���X����
	std::unique_ptr<IConstantBuffer> iConstantBuffer;

	// 3D�s��
	iConstantBuffer = std::make_unique<ConstantBuffer<CTransform3D>>();
	material_.constantBuffers.push_back(std::move(iConstantBuffer));

	// �}�e���A���J���[
	iConstantBuffer = std::make_unique<ConstantBuffer<CMaterialColor>>();
	material_.constantBuffers.push_back(std::move(iConstantBuffer));

	// �F
	iConstantBuffer = std::make_unique<ConstantBuffer<CColor>>();
	material_.constantBuffers.push_back(std::move(iConstantBuffer));

	// �X�L�j���O
	iConstantBuffer = std::make_unique<ConstantBuffer<CSkin>>();
	material_.constantBuffers.push_back(std::move(iConstantBuffer));

	// ������
	material_.Init();
}
void Object3D::MaterialTransfer()
{
	// �}�g���b�N�X
	CTransform3D transform3DData =
	{
		Camera::current.GetViewLookToMat() * Camera::current.GetPerspectiveProjectionMat(),
		transform_.GetWorldMat(),
		Camera::current.pos
	};
	TransferDataToConstantBuffer(material_.constantBuffers[0].get(), transform3DData);

	// �}�e���A���J���[
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

	// �F�f�[�^
	CColor colorData = { color / 255 };
	TransferDataToConstantBuffer(material_.constantBuffers[2].get(), colorData);

	// �X�L�����
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

	// CBV�̐ݒ�R�}���h
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		0, material_.constantBuffers[0]->constantBuffer->GetGPUVirtualAddress());

	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		1, material_.constantBuffers[1]->constantBuffer->GetGPUVirtualAddress());

	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		2, material_.constantBuffers[2]->constantBuffer->GetGPUVirtualAddress());

	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		3, material_.constantBuffers[3]->constantBuffer->GetGPUVirtualAddress());

}

void Object3D::SetAnimation(const uint32_t animationIndex, const uint32_t maxFrame, const bool isPlay)
{
	// �X�L�����
	if (model_->format == ModelFormat::Fbx)
	{
		auto fbxModel = static_cast<FbxModel*>(model_);

		fbxModel->animation.index = animationIndex;
		fbxModel->animation.timer.SetLimitTimer(maxFrame);
		fbxModel->animation.isPlay = isPlay;
	}
}

// --- �Z�b�^�[ -------------------------------------------------------- //

// ���f��
void Object3D::SetModel(Model* model)
{
	model_ = model;
	texture_ = model_->texture;

	// �p�C�v���C���ύX
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

// �e�N�X�`���[
void Object3D::SetTexture(Texture* texture) { texture_ = texture; }

// �O���t�B�b�N�X�p�C�v���C��
void Object3D::SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline) { graphicsPipeline_ = graphicsPipeline; }

// --- �Q�b�^�[ -------------------------------------------------------- //

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
