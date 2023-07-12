#pragma once
#include "NewEngine.h"

class MessageSign
{
private:
	std::unique_ptr<Object3D> messageSign_;

private:
	SphereCollider collider_;

private:
	void ColliderUpdate();

public:
	MessageSign();
	void Init();
	void Update();
	void DrawModel();

public:
	SphereCollider GetCollider();
};