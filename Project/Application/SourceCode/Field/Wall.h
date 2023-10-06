#pragma once
#include "NewEngine.h"

class Wall
{
private:
	Transform mParent;
	std::unique_ptr<Object3D> mWall;
	CapsuleCollider mCollider;

public:
	Wall();
	void Init();
	void Update();
	void DrawModel();

public:
	void SetParent(const Transform parent);
	void SetCollider(const CapsuleCollider collider);
	void SetModel(Model* model);
	void SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline);

public:
	Vec3 GetPos();
	CapsuleCollider GetCollider();
};
