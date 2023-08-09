#pragma once
#include "Transform.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include "Material.h"
#include "Util.h"
#include "GraphicsPipeline.h"
#include "Camera.h"

class Object3D
{
private:
	Vec3 mWorldPos;
	Vec3 mWorldScale;
	Transform mTransform;
	Transform* mParent;
	Model* mModel;
	Texture* mTexture;
	Texture* mDissolveTex;
	Material mMaterial;
	GraphicsPipeline* mGraphicsPipeline;
	Camera* mCamera;
	bool mIsWriteShadow;
	bool mIsWriteDepth;

private:
	// �㐮������
	Texture* mWhiteTex;

public:
	Vec3 pos;
	Vec3 scale;
	Vec3 rot;
	Color color;
	Vec2 tiling;
	Vec2 offset;

	float dissolve;
	float colorPower;
	Color dissolveColor;


	bool isLighting;
	bool isUseDissolve;
	static bool isAllLighting;

private:
	void MaterialInit();
	void MaterialTransfer();
	void MaterialDrawCommands();

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

	// �J����
	void SetCamera(Camera* camera = nullptr);

	// �e
	void SetisShadow(const bool isWriteShadow, const bool isWriteDepth);

	// �e
	void SetParent(Transform* parent);

public: // �Q�b�^�[

	// ���[���h���W
	Vec3 GetWorldPos();

	// ���[���h�X�P�[��
	Vec3 GetWorldScale();

	// �g�����X�t�H�[��
	Transform GetTransform();

	// �e�̃g�����X�t�H�[��
	Transform* GetParent();

	// ���f��
	Model* GetModel();
};

