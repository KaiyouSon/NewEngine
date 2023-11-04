#pragma once
#include "NewEngine.h"

//　リスポーン地点のクラス
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
	RespawnPoint();
	void Init();
	void Update();
	void DrawModel();

public:	// セッター
	void SetParent(const Transform parent);
	void SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline1, GraphicsPipeline* graphicsPipeline2);

public: // ゲッター
	Vec3 GetPos();
	SphereCollider GetCollider();
};
