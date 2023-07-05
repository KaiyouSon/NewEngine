#pragma once
#include "Transform.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include "Material.h"
#include "Util.h"
#include "GraphicsPipeline.h"

class Object3D
{
private:
	Vec3 worldPos_;
	Vec3 worldScale_;
	Transform transform_;
	Transform* parent_;
	Model* model_;
	Texture* texture_;
	Material material_;
	GraphicsPipeline* graphicsPipeline_;

	FbxAnimation animation;

public:
	Vec3 pos;
	Vec3 scale;
	Vec3 rot;
	Color color;
	Vec2 tiling;
	Vec2 offset;

	bool isLighting;
	static bool isAllLighting;

private:
	void MaterialInit();
	void MaterialTransfer();
	void MaterialDrawCommands();

private:
	//  �u�����h�ݒ�
	void SetBlendMode(const BlendMode blendMode);

public:
	Object3D();
	void Update(Transform* parent = nullptr);
	void Draw(const BlendMode blendMode = BlendMode::Alpha);

public: //�Z�b�^�[

	// ���f��
	void SetModel(Model* model);

	// �e�N�X�`���[
	void SetTexture(Texture* texture);

	// �O���t�B�b�N�X�p�C�v���C��
	void SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline);

	// �A�j���[�V����
	void SetAnimation(const uint32_t animationIndex, const uint32_t maxFrame, const bool isPlay = true);

public: // �Q�b�^�[

	// ���[���h���W
	Vec3 GetWorldPos();

	// ���[���h�X�P�[��
	Vec3 GetWorldScale();

	// �g�����X�t�H�[��
	Transform GetTransform();

	// ���f��
	Model* GetModel();


};
