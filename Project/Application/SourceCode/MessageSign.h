#pragma once
#include "NewEngine.h"

class MessageSign
{
private:
	std::unique_ptr<Object3D> mMessageSign;
	Texture* mMessageTex;

private:
	SphereCollider mCollider;

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
	void SetRot(const Vec3 rot);

public:
	Vec3 GetPos();
	Texture* GetMessageTexture();
	SphereCollider GetCollider();
};