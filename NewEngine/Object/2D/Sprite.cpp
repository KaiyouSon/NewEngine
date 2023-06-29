#include "Sprite.h"
#include "MathUtil.h"
#include "RenderBase.h"
#include "Camera.h"

using namespace VertexBufferData;
using namespace ConstantBufferData;

Sprite::Sprite() :
	texture_(TextureManager::GetTexture("White")),
	pos(0), scale(1), rot(0), color(Color::white), anchorPoint_(0.5f),
	vertexBuffer_(std::make_unique<VertexBuffer<VSprite>>()),
	material_(std::make_unique<Material>()),
	graphicsPipeline_(GraphicsPipelineManager::GetGraphicsPipeline("Sprite"))
{
	vertices_.resize(4);
	vertices_[0].uv = { 0.0f,1.0f };
	vertices_[1].uv = { 0.0f,0.0f };
	vertices_[2].uv = { 1.0f,1.0f };
	vertices_[3].uv = { 1.0f,0.0f };
	vertexBuffer_->Create(vertices_);

	// �}�e���A���̏�����
	MaterialInit();
}

void Sprite::Update(Transform* parent)
{
	transform_.pos = pos;
	transform_.scale = { scale.x,scale.y,1 };
	transform_.rot = { 0,0,rot };
	transform_.Update();

	if (parent != nullptr)
	{
		parent_ = parent;

		Mat4 mat = transform_.GetWorldMat();
		mat *= parent_->GetWorldMat();
		transform_.SetWorldMat(mat);
	}

	// �}�e���A���̓]��
	MaterialTransfer();


	vertexBuffer_->TransferToBuffer(vertices_);
}
void Sprite::Draw(const BlendMode blendMode)
{
	SetBlendMode(blendMode);

	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	renderBase->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// VBV��IBV�̐ݒ�R�}���h
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, vertexBuffer_->GetvbViewAddress());

	// �}�e���A���̕`��R�}���h
	MaterialDrawCommands();

	size_t max = renderBase->GetSpriteRootSignature()->GetConstantBufferNum();

	for (int i = 0; i < 1; i++)
	{
		// SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^2�Ԃɐݒ�
		renderBase->GetCommandList()->SetGraphicsRootDescriptorTable(
			UINT(max + i), texture_->GetGpuHandle());
	}

	renderBase->GetCommandList()->DrawInstanced((uint16_t)vertices_.size(), 1, 0, 0);
}

// --- �}�e���A���֘A --------------------------------------------------- //
void Sprite::MaterialInit()
{
	// �C���X�^���X����
	std::unique_ptr<IConstantBuffer> iConstantBuffer;

	// 3D�s��
	iConstantBuffer = std::make_unique<ConstantBuffer<CTransform2D>>();
	material_->constantBuffers.push_back(std::move(iConstantBuffer));

	// �F
	iConstantBuffer = std::make_unique<ConstantBuffer<CColor>>();
	material_->constantBuffers.push_back(std::move(iConstantBuffer));

	// ������
	material_->Init();
}
void Sprite::MaterialTransfer()
{
	// �}�g���b�N�X
	CTransform2D transform2DData =
	{
		transform_.GetWorldMat() * Camera::current.GetOrthoGrphicProjectionMat()
	};
	TransferDataToConstantBuffer(material_->constantBuffers[0].get(), transform2DData);

	// �F�f�[�^
	CColor colorData = { color / 255 };
	TransferDataToConstantBuffer(material_->constantBuffers[1].get(), colorData);
}
void Sprite::MaterialDrawCommands()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	// CBV�̐ݒ�R�}���h
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		0, material_->constantBuffers[0]->constantBuffer->GetGPUVirtualAddress());

	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		1, material_->constantBuffers[1]->constantBuffer->GetGPUVirtualAddress());
}

// --- ���_�f�[�^�֘A --------------------------------------------------- //
void Sprite::TransferVertexCoord()
{
	enum class Point { LD, LU, RD, RU };

	// �l��
	float left = (0.f - anchorPoint_.x) * size_.x;
	float right = (1.f - anchorPoint_.x) * size_.x;
	float up = (0.f - anchorPoint_.y) * size_.y;
	float down = (1.f - anchorPoint_.y) * size_.y;

	// ���_���W
	vertices_[(uint32_t)Point::LD].pos = Vec3(left, down, 0.f);	  //����
	vertices_[(uint32_t)Point::LU].pos = Vec3(left, up, 0.f);	  //����
	vertices_[(uint32_t)Point::RD].pos = Vec3(right, down, 0.f);  //�E��
	vertices_[(uint32_t)Point::RU].pos = Vec3(right, up, 0.f);	  //�E��
}
void Sprite::TransferUVCoord(const Vec2 leftTopPos, const Vec2 rightDownPos)
{
	enum class Point { LD, LU, RD, RU };

	// �l��
	float left = leftTopPos.x / texture_->size.x;
	float right = rightDownPos.x / texture_->size.x;
	float up = leftTopPos.y / texture_->size.y;
	float down = rightDownPos.x / texture_->size.y;

	// uv���W
	vertices_[(uint32_t)Point::LD].uv = Vec2(left, down);	 //����
	vertices_[(uint32_t)Point::LU].uv = Vec2(left, up);		 //����
	vertices_[(uint32_t)Point::RD].uv = Vec2(right, down);	 //�E��
	vertices_[(uint32_t)Point::RU].uv = Vec2(right, up);	 //�E��
}

// --- �Z�b�^�[ -------------------------------------------------------- //

// �e�N�X�`���[
void Sprite::SetTexture(Texture* texture) { texture_ = texture; SetSize(texture->size); }

// �`��͈�
void Sprite::SetTextureRect(const Vec2 leftTopPos, const Vec2 rightDownPos)
{
	TransferUVCoord(leftTopPos, rightDownPos);
	vertexBuffer_->TransferToBuffer(vertices_);
}

// �T�C�Y
void Sprite::SetSize(const Vec2 size)
{
	size_ = size;

	TransferVertexCoord();
	vertexBuffer_->TransferToBuffer(vertices_);
}

// �A���J�[�|�C���g
void Sprite::SetAnchorPoint(const Vec2 anchorPoint)
{
	anchorPoint_ = anchorPoint;

	TransferVertexCoord();
	vertexBuffer_->TransferToBuffer(vertices_);
}

// �u�����h
void Sprite::SetBlendMode(const BlendMode blendMode)
{
	RenderBase* renderBase = RenderBase::GetInstance();//.get();

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

// �O���t�B�b�N�X�p�C�v���C��
void Sprite::SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline)
{
	graphicsPipeline_ = graphicsPipeline;
}
