#include "ColliderObject.h"
#include "RenderBase.h"
#include "LightManager.h"
#include "Camera.h"
#include "FbxModel.h"
#include "ShadowMap.h"

using namespace ConstantBufferData;

ColliderObject::ColliderObject() :
	pos(0, 0, 0), scale(1, 1, 1), rot(0, 0, 0),
	mGraphicsPipeline(GraphicsPipelineManager::GetGraphicsPipeline("ColliderObject")),
	mModel(nullptr), mCamera(&Camera::current)
{
	// �}�e���A���̏�����
	MaterialInit();

	GenerateSphere();
}

void ColliderObject::Update()
{
	// �J�������ݒ肵�ĂȂ��ꍇ
	if (mCamera == nullptr || mCamera == &Camera::current)
	{
		mCamera = &Camera::current;
	}

	mTransform.pos = pos;
	mTransform.scale = scale;
	mTransform.rot = rot;
	mTransform.Update();

	// �}�e���A���̓]��
	MaterialTransfer();
}
void ColliderObject::Draw()
{

	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	// GraphicsPipeline�`��R�}���h
	mGraphicsPipeline->DrawCommand(BlendMode::Alpha);

	// VBV��IBV�̐ݒ�R�}���h
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, mVertexBuffer->GetvbViewAddress());
	renderBase->GetCommandList()->IASetIndexBuffer(mIndexBuffer->GetibViewAddress());

	MaterialDrawCommands();

	renderBase->GetCommandList()->DrawIndexedInstanced((uint16_t)mIndices.size(), 1, 0, 0, 0);
	//renderBase->GetCommandList()->DrawInstanced((uint16_t)mVertices.size(), 1, 0, 0);
}

// --- �}�e���A���֘A --------------------------------------------------- //
void ColliderObject::MaterialInit()
{
	// �C���X�^���X����
	std::unique_ptr<IConstantBuffer> iConstantBuffer;

	// 3D�s��
	iConstantBuffer = std::make_unique<ConstantBuffer<CTransform3D>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// �F
	iConstantBuffer = std::make_unique<ConstantBuffer<CColor>>();
	mMaterial.constantBuffers.push_back(std::move(iConstantBuffer));

	// ������
	mMaterial.Init();
}
void ColliderObject::MaterialTransfer()
{
	// �}�g���b�N�X
	CTransform3D transform3DData =
	{
		mCamera->GetViewLookToMat() * mCamera->GetPerspectiveProjectionMat(),
		mTransform.GetWorldMat(),
		mCamera->pos,
	};
	TransferDataToConstantBuffer(mMaterial.constantBuffers[0].get(), transform3DData);

	// �F�f�[�^
	CColor colorData = { color / 255 };
	TransferDataToConstantBuffer(mMaterial.constantBuffers[1].get(), colorData);
}
void ColliderObject::MaterialDrawCommands()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	for (uint32_t i = 0; i < mMaterial.constantBuffers.size(); i++)
	{
		// CBV�̐ݒ�R�}���h
		renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
			i, mMaterial.constantBuffers[i]->constantBuffer->GetGPUVirtualAddress());
	}
}

// --- ���_�f�[�^�C���f�b�N�X�f�[�^�̐��� ------------------------------- //

// ��
void ColliderObject::GenerateSphere()
{
	float radius = 2.0f;
	uint32_t sliceCount = 32;
	uint32_t stackCount = 16;

	float phiStep = PI / stackCount;
	float thetaStep = 2.0f * PI / sliceCount;

	for (uint32_t i = 0; i <= stackCount; ++i)
	{
		float phi = i * phiStep;
		for (uint32_t j = 0; j <= sliceCount; ++j)
		{
			float theta = j * thetaStep;

			mVertices.emplace_back();

			// ���_���W���v�Z
			mVertices.back().pos.x = radius * sinf(phi) * cosf(theta);
			mVertices.back().pos.y = radius * cosf(phi);
			mVertices.back().pos.z = radius * sinf(phi) * sinf(theta);
		}
	}

	mVertexBuffer = std::make_unique<VertexBuffer<VertexBufferData::VObjModel>>();
	mVertexBuffer->Create(mVertices);

	for (uint32_t i = 0; i < stackCount; ++i)
	{
		for (uint32_t j = 0; j < sliceCount; ++j)
		{
			uint16_t topLeft = i * (sliceCount + 1) + j;
			uint16_t topRight = topLeft + 1;
			uint16_t bottomLeft = (i + 1) * (sliceCount + 1) + j;
			uint16_t bottomRight = bottomLeft + 1;

			// �㑤�̎O�p�`
			mIndices.push_back(topLeft);
			mIndices.push_back(topRight);
			mIndices.push_back(bottomLeft);

			// �����̎O�p�`
			mIndices.push_back(topRight);
			mIndices.push_back(bottomRight);
			mIndices.push_back(bottomLeft);
		}
	}

	mIndexBuffer = std::make_unique<IndexBuffer>();
	mIndexBuffer->Create(mIndices);
}

// --- �Z�b�^�[ -------------------------------------------------------- //

// ���f��
void ColliderObject::SetModel(Model* model)
{
	mModel = model;
}

// �O���t�B�b�N�X�p�C�v���C��
void ColliderObject::SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline) { mGraphicsPipeline = graphicsPipeline; }

// �J����
void ColliderObject::SetCamera(Camera* camera)
{
	mCamera = camera;
}

// --- �Q�b�^�[ -------------------------------------------------------- //

// �g�����X�t�H�[��
Transform ColliderObject::GetTransform()
{
	return mTransform;
}

// ���f��
Model* ColliderObject::GetModel() { return mModel; }
