#include "ColliderObject.h"
#include "RenderBase.h"
#include "ColliderDrawer.h"
#include "Collider.h"

using namespace ConstantBufferData;

ColliderObject::ColliderObject() :
	mGraphicsPipeline(PipelineManager::GetGraphicsPipeline("ColliderObject")),
	model(nullptr), mCamera(&Camera::current), is3D(true)
{
	// �}�e���A���̏�����
	MaterialInit();
}

void ColliderObject::Update()
{
	// �J�������ݒ肵�ĂȂ��ꍇ
	if (mCamera == nullptr || mCamera == &Camera::current)
	{
		mCamera = &Camera::current;
	}

	if (model == nullptr)
	{
		return;
	}

	transform.Update();

	// �}�e���A���̓]��
	MaterialTransfer();
}
void ColliderObject::Draw()
{
	if (model == nullptr)
	{
		return;
	}

	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	// GraphicsPipeline�`��R�}���h
	mGraphicsPipeline->DrawCommand(BlendMode::Alpha);

	// VBV��IBV�̐ݒ�R�}���h
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, model->mesh.vertexBuffer.GetvbViewAddress());
	renderBase->GetCommandList()->IASetIndexBuffer(model->mesh.indexBuffer.GetibViewAddress());

	MaterialDrawCommands();

	renderBase->GetCommandList()->DrawIndexedInstanced((uint16_t)model->mesh.indices.size(), 1, 0, 0, 0);
}

// --- �}�e���A���֘A --------------------------------------------------- //
void ColliderObject::MaterialInit()
{
	// �C���X�^���X����
	std::unique_ptr<IConstantBuffer> iConstantBuffer;

	// 3D�s��
	iConstantBuffer = std::make_unique<ConstantBuffer<CTransformCollider>>();
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
	CTransformCollider transformColliderData =
	{
		is3D == true ?
		// 3D�̏ꍇ
		transform.GetWorldMat() *
		mCamera->GetViewLookToMat() *
		mCamera->GetPerspectiveProjectionMat():
		// 2D�̏ꍇ
		transform.GetWorldMat() *
		mCamera->GetOrthoGrphicProjectionMat()
	};
	TransferDataToConstantBuffer(mMaterial.constantBuffers[0].get(), transformColliderData);

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
			i, mMaterial.constantBuffers[i]->bufferResource->buffer->GetGPUVirtualAddress());
	}
}

// --- �Z�b�^�[ -------------------------------------------------------- //

// �J����
void ColliderObject::SetCamera(Camera* camera)
{
	mCamera = camera;
}
