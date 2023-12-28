#pragma once
#include "NewEngine.h"
#include "IFieldObject.h"

// 木のクラス
class Tree : public IFieldObject
{
private:
	Transform mParent;
	std::unique_ptr<Object3D> mTree;
	std::unique_ptr<Object3D> mBranch;
	SphereCollider mCollider;

public:
	Tree();
	void Init() override;
	void Update() override;
	void ExecuteCS() override {};
	void Draw(const bool isDrawDepth = false) override;

public:
	void SetParent(const Transform& parent);
	void SetCollider(const SphereCollider collider);

public:
	Vec3 GetPos();
	SphereCollider GetCollider();
};

