#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "Transform.h"
#include "Material.h"
#include "TextureManager.h"
#include "Enum.h"
#include <vector>

class TextureAnimeiton;

class Sprite
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
	FlipType mFlipType;

public:
	Vec2 pos;
	Vec2 scale;
	float rot;
	Color color;

private: // �}�e���A���֘A
	void MaterialInit();
	void MaterialTransfer();
	void MaterialDrawCommands();

private:
	void TransferVertexCoord();
	void TransferUVCoord(const Vec2 leftTopPos, const Vec2 rightDownPos);

public:
	Sprite();
	void Update(Transform* parent = nullptr);
	void Draw(const BlendMode blendMode = BlendMode::Alpha);

	template<typename T>
	void AddMaterial()
	{
		std::unique_ptr<IConstantBuffer> iConstatnBuffer = std::make_unique<T>();
		iConstatnBuffer->Create();
		mMaterial->constantBuffers.push_back(std::move(iConstatnBuffer));
	}

public: //�Z�b�^�[

	// �e�N�X�`���[
	void SetTexture(Texture* texture);

	// �`��͈�
	void SetTextureRect(const Vec2 leftTopPos, const Vec2 rightDownPos);

	//�@�T�C�Y
	void SetSize(const Vec2 size);

	// �A���J�[�|�C���g
	void SetAnchorPoint(const Vec2 anchorPoint);

	// �摜���]
	void SetFlipType(const FlipType flipType);

	// �O���t�B�b�N�X�p�C�v���C��
	void SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline);

	template<typename T>
	void SetTransferBuffer(const uint32_t bufferNum, const T& data)
	{
		uint32_t bNum = Min<uint32_t>(bufferNum, (uint32_t)mMaterial->constantBuffers.size());
		TransferDataToConstantBuffer(mMaterial->constantBuffers[bNum].get(), data);
	}

private:
	friend TextureAnimeiton;
};

