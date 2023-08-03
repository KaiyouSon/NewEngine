#include "PostEffect.h"
#include "TextureManager.h"
#include "Camera.h"

using namespace VertexBufferData;
using namespace ConstantBufferData;

PostEffect::PostEffect() :
	vertexBuffer_(std::make_unique<VertexBuffer<VSprite>>()),
	graphicsPipeline_(GraphicsPipelineManager::GetGraphicsPipeline("RenderTexture")),
	pos(0), scale(1), rot(0), anchorPoint(0.5f), rtvIndex(0)
{
	// ���_�o�b�t�@�̐���
	vertices_.resize(4);
	vertices_[0].uv = { 0.0f,1.0f };
	vertices_[1].uv = { 0.0f,0.0f };
	vertices_[2].uv = { 1.0f,1.0f };
	vertices_[3].uv = { 1.0f,0.0f };
	vertexBuffer_->Create(vertices_);

	// �o�b�t�@����
	MaterialInit();
}

void PostEffect::Update()
{
	transform_.pos = pos;
	transform_.scale = { scale.x,scale.y,1.f };
	transform_.rot = { 0.f,0.f,rot };
	transform_.Update();

	// �]������
	MaterialTransfer();
}

void PostEffect::Draw()
{
	if (renderTextures_.empty() == true) return;

	//SetBlendMode(blendMode);

	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	renderBase->GetCommandList()->SetPipelineState(graphicsPipeline_->GetAlphaPipeline());

	renderBase->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// VBV��IBV�̐ݒ�R�}���h
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, vertexBuffer_->GetvbViewAddress());

	// �}�e���A���̐ݒ�R�}���h
	MaterialDrawCommands();

	// SRV�ݒ�R�}���h
	uint32_t index = renderBase->GetRenderTextureRootSignature()->GetConstantBufferNum();
	for (uint32_t i = 0; i < renderTextures_.size(); i++)
	{
		renderBase->GetCommandList()->
			SetGraphicsRootDescriptorTable(index + i, renderTextures_[i]->GetGpuHandle(rtvIndex));

		//renderBase->GetCommandList()->
		//	SetGraphicsRootDescriptorTable(index + 1, renderTextures_[i]->GetGpuHandle(rtvIndex));

		//if (renderTextures_[i]->useDepth == true)
		//{

		D3D12_RESOURCE_BARRIER  barrier{};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Transition.pResource = renderTextures_[i]->depthTexture->buffer.Get();
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_DEPTH_WRITE;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;

		renderBase->GetCommandList()->ResourceBarrier(1, &barrier);

		renderBase->GetCommandList()->
			SetGraphicsRootDescriptorTable(
				(uint32_t)(index + 1),
				renderTextures_[i]->depthTexture->GetGpuHandle());

		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Transition.pResource = renderTextures_[i]->depthTexture->buffer.Get();
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_GENERIC_READ;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_DEPTH_WRITE;

		renderBase->GetCommandList()->ResourceBarrier(1, &barrier);

		//}
	}

	renderBase->GetCommandList()->DrawInstanced((uint16_t)vertices_.size(), 1, 0, 0);
}

// --- �}�e���A���֘A --------------------------------------------------- //
void PostEffect::MaterialInit()
{
	// �C���X�^���X����
	std::unique_ptr<IConstantBuffer> iConstantBuffer;

	// 3D�s��
	iConstantBuffer = std::make_unique<ConstantBuffer<CTransform2D>>();
	material_.constantBuffers.push_back(std::move(iConstantBuffer));

	// �F
	iConstantBuffer = std::make_unique<ConstantBuffer<CColor>>();
	material_.constantBuffers.push_back(std::move(iConstantBuffer));

	// ������
	material_.Init();
}
void PostEffect::MaterialTransfer()
{
	// �}�g���b�N�X
	CTransform2D transform3DData =
	{
		transform_.GetWorldMat() * Camera::current.GetOrthoGrphicProjectionMat()
	};
	TransferDataToConstantBuffer(material_.constantBuffers[0].get(), transform3DData);

	// �F�f�[�^
	CColor colorData = { color / 255 };
	TransferDataToConstantBuffer(material_.constantBuffers[1].get(), colorData);
}
void PostEffect::MaterialDrawCommands()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	// CBV�̐ݒ�R�}���h
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		0, material_.constantBuffers[0]->constantBuffer->GetGPUVirtualAddress());

	// CBV�̐ݒ�R�}���h
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		1, material_.constantBuffers[1]->constantBuffer->GetGPUVirtualAddress());
}

// --- ���̑��̊֐� ----------------------------------------------------- //
void PostEffect::AddRenderTexture(RenderTexture* renderTexture)
{
	renderTextures_.push_back(renderTexture);

	// �e�N�X�`���[���ꖇ�̎��ɂ������_���W�ς��Ȃ�
	if (renderTextures_.size() == 1)
	{
		TransferTexturePos();
	}
}

void PostEffect::SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline)
{
	graphicsPipeline_ = graphicsPipeline;
}

void PostEffect::SetDrawCommands(const uint32_t registerNum, const uint32_t bufferNum)
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	uint32_t bNum = Min<uint32_t>(bufferNum, (uint32_t)material_.constantBuffers.size());

	// CBV�̐ݒ�R�}���h
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		registerNum, material_.constantBuffers[bNum]->constantBuffer->GetGPUVirtualAddress());
}

void PostEffect::TransferTexturePos()
{
	// �e�N�X�`���[�̃T�C�Y
	float width = renderTextures_.front()->size.x;
	float height = renderTextures_.front()->size.y;

	vertices_[0].pos = { (0.0f - anchorPoint.x) * width,(1.0f - anchorPoint.y) * height,0.0f }; //����
	vertices_[1].pos = { (0.0f - anchorPoint.x) * width,(0.0f - anchorPoint.y) * height,0.0f }; //����
	vertices_[2].pos = { (1.0f - anchorPoint.x) * width,(1.0f - anchorPoint.y) * height,0.0f }; //�E��
	vertices_[3].pos = { (1.0f - anchorPoint.x) * width,(0.0f - anchorPoint.y) * height,0.0f }; //�E��

	vertexBuffer_->TransferToBuffer(vertices_);
}