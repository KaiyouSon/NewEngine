#pragma once
#include "ConstantBuffer.h"
#include "Transform.h"
#include "Model.h"
#include "Texture.h"
#include "Util.h"
#include "Sprite.h"

class CircleGaugeSprite
{
private:
	std::vector<VertexBufferData::VSprite> mVertices;
	std::unique_ptr<VertexBuffer<VertexBufferData::VSprite>> mVertexBuffer;
	std::unique_ptr<Material> mMaterial;
	GraphicsPipeline* mGraphicsPipeline;
	Transform mTransform;
	Transform* mParent;
	Texture* mTexture;
	Vec2 mAnchorPoint;
	Vec2 mSize;

public:
	Vec2 pos;
	Vec2 scale;
	float rot;
	Color color;
	float startRadian;
	float endRadian;

private: // �}�e���A���֘A
	void MaterialInit();
	void MaterialTransfer();
	void MaterialDrawCommands();

private:
	void TransferVertexCoord();
	void TransferUVCoord(const Vec2 leftTopPos, const Vec2 rightDownPos);

public:
	CircleGaugeSprite();
	void Update(Transform* parent);
	void Draw(const BlendMode blendMode = BlendMode::Alpha);


public:	// �Z�b�^�[
	// �e�N�X�`���[
	void SetTexture(Texture* texture);

	// �`��͈�
	void SetTextureRect(const Vec2 leftTopPos, const Vec2 rightDownPos);

	//�@�T�C�Y
	void SetSize(const Vec2 size);

	// �A���J�[�|�C���g
	void SetAnchorPoint(const Vec2 anchorPoint);

	// �O���t�B�b�N�X�p�C�v���C��
	void SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline);

private:
	// �u�����h
	void SetBlendMode(const BlendMode blendMode);
};
