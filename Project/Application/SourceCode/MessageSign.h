#pragma once
#include "NewEngine.h"

class MessageSign
{
private:
	std::unique_ptr<Object3D> messageSign_;
	Texture* messageTex_;

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
	void SetMessageTexture(Texture* texture);
	void SetPos(const Vec3 pos);

public:
	Texture* GetMessageTexture();
	SphereCollider GetCollider();
};