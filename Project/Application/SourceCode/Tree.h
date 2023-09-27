#pragma once
#include "NewEngine.h"

class Tree
{
private:
	Transform mParent;
	std::unique_ptr<Object3D> mTree;
	std::unique_ptr<Object3D> mBranch;
	SphereCollider mCollider;

public:
	static void CreateGraphicsPipeline();

	Tree();
	void Init();
	void Update();
	void DrawModel();

public:
	void SetParent(const Transform& parent);
	void SetCollider(const SphereCollider collider);
	void SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline);

public:
	Vec3 GetPos();
	SphereCollider GetCollider();
};

