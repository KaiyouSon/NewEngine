#pragma once
#include "Transform.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include "Material.h"
#include "Util.h"
#include "GraphicsPipeline.h"
#include "VertexBuffer.h"
#include "ParticleParam.h"
#include "StructuredBuffer.h"
#include "RWStructuredBuffer.h"

class Emitter
{
private:
	std::vector<VertexBufferData::VParticle> mVertices;
	std::unique_ptr<VertexBuffer<VertexBufferData::VParticle>> mVertexBuffer;

	std::unique_ptr<StructuredBuffer> mStructuredBuffer;
	std::unique_ptr<RWStructuredBuffer<ParticleParameter::Test>> mRWStructuredBuffer;

	Vec3 mWorldPos;
	Vec3 mWorldScale;
	Transform mTransform;
	Transform* mParent;
	Texture* mTexture;
	Material mMaterial;
	GraphicsPipeline* mGraphicsPipeline;
	Billboard mBillboard;

public:
	Vec3 pos;
	Vec3 scale;
	Vec3 rot;
	Color color;
	Vec2 tiling;
	Vec2 offset;
	std::vector<ParticleParameter::PParam0> pParam;
	uint32_t pSize;

private:
	void MaterialInit();
	void MaterialTransfer();
	void MaterialDrawCommands();

public:
	Emitter();
	void Update(Transform* parent = nullptr);
	void Draw(const BlendMode blendMode = BlendMode::Alpha);

public: //�Z�b�^�[

	// �e�N�X�`���[
	void SetTexture(Texture* texture);

	// �O���t�B�b�N�X�p�C�v���C��
	void SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline);

	// �p�[�e�B�N���̐�
	void SetMaxParticle(const uint32_t max);

public: // �Q�b�^�[

	// ���[���h���W
	Vec3 GetWorldPos();

	// ���[���h�X�P�[��
	Vec3 GetWorldScale();

	// �g�����X�t�H�[��
	Transform GetTransform();

};

