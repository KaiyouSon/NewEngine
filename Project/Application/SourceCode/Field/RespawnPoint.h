#pragma once
#include "NewEngine.h"

class RespawnPoint
{
private:
	Transform mParent;
	std::unique_ptr<Object3D> mRipple;
	std::unique_ptr<Object3D> mRhombus;
	Timer mAngle;
	SphereCollider mCollider;

private:
	void ColliderUpdatge();

public:
	static void CreateGraphicsPipeline();
	RespawnPoint();
	void Init();
	void Update();
	void DrawModel();

public:
	// 繧ｻ繝・ち繝ｼ
	void SetParent(const Transform parent);
	void SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline);

public:
	// 繧ｲ繝・ち繝ｼ
	Vec3 GetPos();
	SphereCollider GetCollider();
};
