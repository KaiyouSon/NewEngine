#include "CircleGaugeSprite.h"
#include "Camera.h"

using namespace VertexBufferData;
using namespace ConstantBufferData;

CircleGaugeSprite::CircleGaugeSprite() :
	pos(0), scale(1), rot(0), mAnchorPoint(0.5f),
	startRadian(0), endRadian(0),
	mVertexBuffer(std::make_unique<VertexBuffer<VSprite>>()),
	mGraphicsPipeline(GraphicsPipelineManager::GetGraphicsPipeline("CircleGaugeSprite"))
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

void CircleGaugeSprite::Update(Transform* parent)
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

	endRadian = Clamp<float>(endRadian, 0, Radian(360));

	// �}�e���A���̓]��
	MaterialTransfer();

	// ���_�f�[�^�̓]��
	mVertexBuffer->TransferToBuffer(mVertices);
}
void CircleGaugeSprite::Draw(const BlendMode blendMode)
{
	SetBlendMode(blendMode);

	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	renderBase->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	// VBV��IBV�̐ݒ�R�}���h
	renderBase->GetCommandList()->IASetVertexBuffers(0, 1, mVertexBuffer->GetvbViewAddress());

	// �}�e���A���̕`��R�}���h
	MaterialDrawCommands();

	// SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^2�Ԃɐݒ�
	uint32_t startIndex = renderBase->GetSpriteRootSignature()->GetDescriptorTableStartIndex();
	renderBase->GetCommandList()->SetGraphicsRootDescriptorTable(startIndex, mTexture->GetGpuHandle());

	renderBase->GetCommandList()->DrawInstanced((unsigned short)mVertices.size(), 1, 0, 0);
}

// --- �}�e���A���֘A --------------------------------------------------- //
void CircleGaugeSprite::MaterialInit()
{
	// �C���X�^���X����
	std::unique_ptr<IConstantBuffer> iConstantBuffer;

	// 2D�s��
	iConstantBuffer = std::make_unique<ConstantBuffer<CTransform2D>>();
	mMaterial->constantBuffers.push_back(std::move(iConstantBuffer));

	// �F
	iConstantBuffer = std::make_unique<ConstantBuffer<CColor>>();
	mMaterial->constantBuffers.push_back(std::move(iConstantBuffer));

	// �~�Q�[�W
	iConstantBuffer = std::make_unique<ConstantBuffer<CCircleGauge>>();
	mMaterial->constantBuffers.push_back(std::move(iConstantBuffer));

	// ������
	mMaterial->Init();
}
void CircleGaugeSprite::MaterialTransfer()
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

	// �~�Q�[�W
	CCircleGauge circleGaugeData = { startRadian ,endRadian };
	TransferDataToConstantBuffer(mMaterial->constantBuffers[2].get(), circleGaugeData);
}
void CircleGaugeSprite::MaterialDrawCommands()
{
	RenderBase* renderBase = RenderBase::GetInstance();// .get();

	// CBV�̐ݒ�R�}���h
	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		0, mMaterial->constantBuffers[0]->constantBuffer->GetGPUVirtualAddress());

	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		1, mMaterial->constantBuffers[1]->constantBuffer->GetGPUVirtualAddress());

	renderBase->GetCommandList()->SetGraphicsRootConstantBufferView(
		2, mMaterial->constantBuffers[2]->constantBuffer->GetGPUVirtualAddress());
}

// --- ���_�f�[�^�֘A --------------------------------------------------- //
void CircleGaugeSprite::TransferVertexCoord()
{
	enum { LD, LU, RD, RU };

	// �l��
	float left = (0.f - mAnchorPoint.x) * mSize.x;
	float right = (1.f - mAnchorPoint.x) * mSize.x;
	float up = (0.f - mAnchorPoint.y) * mSize.y;
	float down = (1.f - mAnchorPoint.y) * mSize.y;

	// ���_���W
	mVertices[LD].pos = Vec3(left, down, 0.f);	  //����
	mVertices[LU].pos = Vec3(left, up, 0.f);	  //����
	mVertices[RD].pos = Vec3(right, down, 0.f);  //�E��
	mVertices[RU].pos = Vec3(right, up, 0.f);	  //�E��
}
void CircleGaugeSprite::TransferUVCoord(const Vec2 leftTopPos, const Vec2 rightDownPos)
{
	enum { LD, LU, RD, RU };

	// �l��
	float left = leftTopPos.x / mTexture->size.x;
	float right = rightDownPos.x / mTexture->size.x;
	float up = leftTopPos.y / mTexture->size.y;
	float down = rightDownPos.y / mTexture->size.y;

	// uv���W
	mVertices[LD].uv = Vec2(left, down);	 //����
	mVertices[LU].uv = Vec2(left, up);		 //����
	mVertices[RD].uv = Vec2(right, down);	 //�E��
	mVertices[RU].uv = Vec2(right, up);	 //�E��
}

// --- �Z�b�^�[ -------------------------------------------------------- //

// �e�N�X�`���[
void CircleGaugeSprite::SetTexture(Texture* texture)
{
	mTexture = texture;
	SetSize(texture->size);
}

// �`��͈�
void CircleGaugeSprite::SetTextureRect(const Vec2 leftTopPos, const Vec2 rightDownPos)
{
	TransferUVCoord(leftTopPos, rightDownPos);
	mVertexBuffer->TransferToBuffer(mVertices);
}

// �T�C�Y
void CircleGaugeSprite::SetSize(const Vec2 size)
{
	mSize = size;

	TransferVertexCoord();
	mVertexBuffer->TransferToBuffer(mVertices);
}

// �A���J�[�|�C���g
void CircleGaugeSprite::SetAnchorPoint(const Vec2 anchorPoint)
{
	mAnchorPoint = anchorPoint;

	TransferVertexCoord();
	mVertexBuffer->TransferToBuffer(mVertices);
}

// �u�����h���[�h
void CircleGaugeSprite::SetBlendMode(const BlendMode blendMode)
{
	RenderBase* renderBase = RenderBase::GetInstance();//.get();

	switch (blendMode)
	{
	case BlendMode::Alpha: // ���u�����h
		renderBase->GetCommandList()->SetPipelineState(mGraphicsPipeline->GetAlphaPipeline());
		break;

	case BlendMode::Add:	// ���Z�u�����h
		renderBase->GetCommandList()->SetPipelineState(mGraphicsPipeline->GetAddPipeline());
		break;

	case BlendMode::Sub:	// ���Z�u�����h
		renderBase->GetCommandList()->SetPipelineState(mGraphicsPipeline->GetSubPipeline());
		break;

	case BlendMode::Inv:	// ���]
		renderBase->GetCommandList()->SetPipelineState(mGraphicsPipeline->GetInvPipeline());
		break;

	default:
		break;
	}
}

// �O���t�B�b�N�X�p�C�v���C��
void CircleGaugeSprite::SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline)
{
	mGraphicsPipeline = graphicsPipeline;
}