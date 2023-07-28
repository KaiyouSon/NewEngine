#include "MessageSign.h"

MessageSign::MessageSign() :
	messageSign_(std::make_unique<Object3D>())
{
	messageSign_->SetModel(ModelManager::GetModel("MessageSign"));

	Init();
}

void MessageSign::Init()
{
	messageSign_->scale = 1.5f;
}

void MessageSign::Update()
{
	ColliderUpdate();

	messageSign_->Update();
}

void MessageSign::DrawModel()
{
	messageSign_->Draw();
}

Texture* MessageSign::GetMessageTexture()
{
	return messageTex_;
}

SphereCollider MessageSign::GetCollider()
{
	return collider_;
}

void MessageSign::SetMessageTexture(Texture* texture)
{
	messageTex_ = texture;
}

void MessageSign::SetPos(const Vec3 pos)
{
	messageSign_->pos = pos;
}

void MessageSign::ColliderUpdate()
{
	collider_.centerPos = messageSign_->pos;
	collider_.radius = messageSign_->scale.x;
}