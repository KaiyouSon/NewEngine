#pragma once
#include "NewEngine.h"

class MessegeSign
{
private:
	std::unique_ptr<Object3D> messegeSign_;

private:
	SphereCollider collider_;

private:
	void ColliderUpdate();

public:
	MessegeSign();
	void Init();
	void Update();
	void DrawModel();

public:
	SphereCollider GetCollider();
};