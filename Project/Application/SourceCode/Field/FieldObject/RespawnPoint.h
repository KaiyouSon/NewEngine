#pragma once
#include "NewEngine.h"
#include "IFieldObject.h"

//　リスポーン地点のクラス
class RespawnPoint : public IFieldObject
{
private:
	Transform mParent;
	std::unique_ptr<Object3D> mRipple;
	std::unique_ptr<Object3D> mRhombus;
	std::unique_ptr<PointLight> mPointLight;
	Timer mAngle;
	SphereCollider mCollider;

private:
	void ColliderUpdatge();

public:
	RespawnPoint();
	void Init() override;
	void Update() override;
	void ExecuteCS() override {};
	void Draw(const bool isDrawDepth = false) override;

public:	// セッター
	void SetParent(const Transform parent);

public: // ゲッター
	Vec3 GetPos();
	SphereCollider GetCollider();
};
