#pragma once
#include "NewEngine.h"

class ShadowObj
{
private:
	Transform mTransform;
	Transform* mParent;
	Model* mModel;
	Material mMaterial;
	GraphicsPipeline* mGraphicsPipeline;
	Camera* mCamera;

public:
	Vec3 pos;
	Vec3 scale;
	Vec3 rot;

private:
	void MaterialInit();
	void MaterialTransfer();
	void MaterialDrawCommands();

public:
	ShadowObj();
	static void CreateGraphicsPipeline();
	void Update(Transform* parent = nullptr);
	void Draw();

public: //�Z�b�^�[

	// ���f��
	void SetModel(Model* model);

	// �J����
	void SetCamera(Camera* camera = nullptr);

	// �e
	void SetParent(Transform* parent);

};

