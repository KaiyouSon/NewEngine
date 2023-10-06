#pragma once
#include "Transform.h"
#include "ModelManager.h"
#include "Material.h"
#include "Util.h"
#include "GraphicsPipeline.h"
#include "Camera.h"
#include "Enum.h"
#include "ICollider.h"

class ColliderObject
{
private:
	Material mMaterial;
	GraphicsPipeline* mGraphicsPipeline;
	Camera* mCamera;

public:
	Transform transform;
	Model* model;
	Color color;
	bool is3D;

private:
	void MaterialInit();
	void MaterialTransfer();
	void MaterialDrawCommands();

public:
	ColliderObject();
	void Update();
	void Draw();

public: //セッター

	// カメラ
	void SetCamera(Camera* camera = nullptr);
};

