#include "MessegeSign.h"

MessegeSign::MessegeSign() :
	messegeSign_(std::make_unique<Object3D>())
{
	messegeSign_->SetModel(ModelManager::GetModel("MessegeSign"));

	Init();
}

void MessegeSign::Init()
{
	messegeSign_->pos = Vec3(10, 0, 5);
	messegeSign_->scale = 1.5f;
}

void MessegeSign::Update()
{
	messegeSign_->Update();
}

void MessegeSign::DrawModel()
{
	messegeSign_->Draw();
}

SphereCollider MessegeSign::GetCollider()
{
	return collider_;
}

void MessegeSign::ColliderUpdate()
{
	collider_.centerPos = messegeSign_->pos;
	collider_.radius = messegeSign_->scale.x / 2.f;
}