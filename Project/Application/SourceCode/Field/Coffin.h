#pragma once
#include "NewEngine.h"
#include "IFieldObject.h"

// 棺桶のクラス
class Coffin : public IFieldObject
{
private:
	Transform mParent;
	std::unique_ptr<Object3D> mCoffinTop;
	std::unique_ptr<Object3D> mCoffinBottom;
	CubeCollider mBottomCollider;

public:
	Coffin();
	void Init() override;
	void Update() override;
	void Draw(const bool isDrawDepth = false) override;

public:
	void SetParent(const Transform parent);
	void SetTopPos(const Vec3 pos);
	void SetBottomPos(const Vec3 pos);
	void SetTopRot(const Vec3 rot);
	void SetBottomRot(const Vec3 rot);
	void SetBottomCollider(const CubeCollider& collider);
	void SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline);

public:
	CubeCollider GetBottomCollider();
	Vec3 GetPos();
	Vec3 GetTopPos();
};
