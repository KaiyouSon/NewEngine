#include "Sprite.h"
#include "MathUtil.h"
#include "RenderBase.h"
#include "Camera.h"

using namespace VertexBufferData;
using namespace ConstantBufferData;

Sprite::Sprite() :
	mTexture(TextureManager::GetTexture("White")),
	pos(0), scale(1), rot(0), color(Color::white),
	mAnchorPoint(0.5f), mFlipType(FlipType::None),
	mVertexBuffer(std::make_unique<VertexBuffer<VSprite>>()),
	mMaterial(std::make_unique<Material>()),
	mGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Sprite"))
{
	mVertices.resize(4);
	mVertices[0].uv = { 0.0f,1.0f };
	mVertices[1].uv = { 0.0f,0.0f };
	mVertices[2].uv = { 1.0f,1.0f };
	mVertices[3].uv = { 1.0f,0.0f };
	mVertexBuffer->Create(mVertices);

	// �}�e���A���̏�����
	MaterialInit();
}

void Sprite::Update(Transform* parent)
{
	mTransform.pos = pos;
	mTransform.scale = { scale.x,scale.y,1 };
	mTransform.rot = { 0,0,rot };
	mTransform.Update();

	if (parent != nullptr)
	{
		mParent = parent;

		Mat4 mat = mTransform.GetWorldMat();
		mat *= mParent->GetWorldMat();
		mTransform.SetWorldMat(mat);
	}

	// �}�e���A���̓]��
	MaterialTransfer();

	// ���_�f�[�^�̓]��
	mVertexBuffer->TransferToBuffer(mVertices);
}
void Sprite::Draw(const BlendMode blendMode)
{
	if (mTexture == nullptr)
	{
		return;
	}

	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	// GraphicsPipeline�`��R�}���h
	mGraphicsPipeline->DrawCommand(blendMode);

	// VBV��IBV�̐ݒ�R�}���h
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, mVertexBuffer->GetvbViewAddress());

	// �}�e���A���̕`��R�}���h
	MaterialDrawCommands();

	uint32_t startIndex = mGraphicsPipeline->GetRootSignature()->GetSRVStartIndex();
	uint32_t endIndex = mGraphicsPipeline->GetRootSignature()->GetUAVStartIndex();

	for (uint32_t i = startIndex; i < endIndex; i++)
	{
		// SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^2�Ԃɐݒ�
		renderBase->GetCommandList()->SetGraphicsRootDescriptorTable(startIndex, mTexture->GetBufferResource()->srvHandle.gpu);
	}

	renderBase->GetCommandList()->DrawInstanced((uint16_t)mVertices.size(), 1, 0, 0);
}

// --- �}�e���A���֘A --------------------------------------------------- //
void Sprite::MaterialInit()
{
	// �C���X�^���X����
	std::unique_ptr<IConstantBuffer> iConstantBuffer;

	// 2D�s��
	iConstantBuffer = std::make_unique<ConstantBuffer<CTransform2D>>();
	mMaterial->constantBuffers.push_back(std::move(iConstantBuffer));

	// �F
	iConstantBuffer = std::make_unique<ConstantBuffer<CColor>>();
	mMaterial->constantBuffers.push_back(std::move(iConstantBuffer));

	// ������
	mMaterial->Init();
}
void Sprite::MaterialTransfer()
{
	// �}�g���b�N�X
	CTransform2D transform2DData =
	{
		mTransform.GetWorldMat() * Camera::current.GetOrthoGrphicProjectionMat()
	};
	TransferDataToConstantBuffer(mMaterial->constantBuffers[0].get(), transform2DData);

	// �F�f�[�^
	CColor colorData = { color.To01() };
	TransferDataToConstantBuffer(mMaterial->constantBuffers[1].get(), colorData);
}
void Sprite::MaterialDrawCommands()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	for (uint32_t i = 0; i < mMaterial->constantBuffers.size(); i++)
	{
		// CBV�̐ݒ�R�}���h
		renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
			i, mMaterial->constantBuffers[i]->bufferResource->buffer->GetGPUVirtualAddress());
	}
}

// --- ���_�f�[�^�֘A --------------------------------------------------- //
void Sprite::TransferVertexCoord()
{
	enum class Point { LD, LU, RD, RU };

	// �l��
	float left = (0.f - mAnchorPoint.x) * mSize.x;
	float right = (1.f - mAnchorPoint.x) * mSize.x;
	float up = (0.f - mAnchorPoint.y) * mSize.y;
	float down = (1.f - mAnchorPoint.y) * mSize.y;

	switch (mFlipType)
	{
	case FlipType::X:
		left = -left;
		right = -right;
		break;

	case FlipType::Y:
		up = -up;
		down = -down;
		break;

	case FlipType::XY:
		left = -left;
		right = -right;
		up = -up;
		down = -down;
		break;

	default:
		break;
	}

	// ���_���W
	mVertices[(uint32_t)Point::LD].pos = Vec3(left, down, 0.f);	  //����
	mVertices[(uint32_t)Point::LU].pos = Vec3(left, up, 0.f);	  //����
	mVertices[(uint32_t)Point::RD].pos = Vec3(right, down, 0.f);  //�E��
	mVertices[(uint32_t)Point::RU].pos = Vec3(right, up, 0.f);	  //�E��
}
void Sprite::TransferUVCoord(const Vec2 leftTopPos, const Vec2 rightDownPos)
{
	enum class Point { LD, LU, RD, RU };

	// �l��
	float left = leftTopPos.x / mTexture->size.x;
	float right = rightDownPos.x / mTexture->size.x;
	float up = leftTopPos.y / mTexture->size.y;
	float down = rightDownPos.y / mTexture->size.y;

	// uv���W
	mVertices[(uint32_t)Point::LD].uv = Vec2(left, down);	 //����
	mVertices[(uint32_t)Point::LU].uv = Vec2(left, up);		 //����
	mVertices[(uint32_t)Point::RD].uv = Vec2(right, down);	 //�E��
	mVertices[(uint32_t)Point::RU].uv = Vec2(right, up);	 //�E��
}

// --- �Z�b�^�[ -------------------------------------------------------- //

// �e�N�X�`���[
void Sprite::SetTexture(Texture* texture)
{
	mTexture = texture;
	SetSize(texture->size);
}

// �`��͈�
void Sprite::SetTextureRect(const Vec2 leftTopPos, const Vec2 rightDownPos)
{
	TransferUVCoord(leftTopPos, rightDownPos);
	mVertexBuffer->TransferToBuffer(mVertices);
}

// �T�C�Y
void Sprite::SetSize(const Vec2 size)
{
	mSize = size;

	TransferVertexCoord();
	mVertexBuffer->TransferToBuffer(mVertices);
}

// �A���J�[�|�C���g
void Sprite::SetAnchorPoint(const Vec2 anchorPoint)
{
	mAnchorPoint = anchorPoint;

	TransferVertexCoord();
	mVertexBuffer->TransferToBuffer(mVertices);
}

// �摜���]
void Sprite::SetFlipType(const FlipType flipType)
{
	mFlipType = flipType;

	TransferVertexCoord();
	mVertexBuffer->TransferToBuffer(mVertices);
}

// �O���t�B�b�N�X�p�C�v���C��
void Sprite::SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline)
{
	mGraphicsPipeline = graphicsPipeline;
}
