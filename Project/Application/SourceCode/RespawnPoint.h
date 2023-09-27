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
	// �Z�b�^�[
	void SetParent(const Transform parent);
	void SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline);

public:
	// �Q�b�^�[
	Vec3 GetPos();
	SphereCollider GetCollider();
};

