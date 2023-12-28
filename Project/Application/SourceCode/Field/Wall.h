#pragma once
#include "NewEngine.h"
#include "IFieldObject.h"

// 城壁のクラス
class Wall : public IFieldObject
{
private:
	Transform mParent;
	std::unique_ptr<Object3D> mWall;
	CapsuleCollider mCollider;

public:
	Wall();
	void Init() override;
	void Update() override;
	void Draw(const bool isDrawDepth = false) override;

public:
	void SetParent(const Transform parent);
	void SetCollider(const CapsuleCollider collider);
	void SetModel(Model* model);
	void SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline);

public:
	Vec3 GetPos();
	CapsuleCollider GetCollider();
};
