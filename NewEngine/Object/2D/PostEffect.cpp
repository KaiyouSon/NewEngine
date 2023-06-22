#include "PostEffect.h"
#include "Camera.h"
using namespace ConstantBufferData;

#include "InputManager.h"
#include "TextureManager.h"
#include <d3d12.h>

PostEffect::PostEffect() :
	vertexBuffer_(std::make_unique<VertexBuffer<VertexPosUv>>()),
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

	//if (Key::GetKeyDown(DIK_0))
	//{
	//	static int tex = 0;
	//	tex = (tex + 1) % 2;

	//	// SRV�q�[�v�̐擪�n���h�����擾
	//	D3D12_CPU_DESCRIPTOR_HANDLE srvCpuHandle = TextureManager::srvDescHeap_->GetCPUDescriptorHandleForHeapStart();
	//	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = TextureManager::srvDescHeap_->GetGPUDescriptorHandleForHeapStart();

	//	UINT descriptorSize = RenderBase::GetInstance()->GetDevice()->
	//		GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	//	srvCpuHandle.ptr += (SIZE_T)(descriptorSize * TextureManager::srvIncrementIndex_);
	//	srvGpuHandle.ptr += (SIZE_T)(descriptorSize * TextureManager::srvIncrementIndex_);

	//	renderTextures_[0]->cpuHandle = srvCpuHandle;
	//	renderTextures_[0]->gpuHandle = srvGpuHandle;

	//	// �V�F�[�_�[���\�[�X�r���[�ݒ�
	//	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};	// srv�ݒ�\����
	//	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	//	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	// 2D�e�N�X�`��
	//	srvDesc.Texture2D.MipLevels = 1;

	//	// �n���h���̎w���ʒu�ɃV�F�[�_�[���\�[�X�r���[�쐬
	//	RenderBase::GetInstance()->GetDevice()->
	//		CreateShaderResourceView(renderTextures_[0]->buffers[tex].Get(), &srvDesc, srvCpuHandle);

	//	TextureManager::srvIncrementIndex_++;

	//}

	// SRV�ݒ�R�}���h
	for (uint32_t i = 0; i < renderTextures_.size(); i++)
	{
		uint32_t index = renderBase->GetRenderTextureRootSignature()->GetConstantBufferNum();
		renderBase->GetCommandList()->SetGraphicsRootDescriptorTable(index + i, renderTextures_[i]->gpuHandles[rtvIndex]);
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