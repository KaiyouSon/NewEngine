#include "MessageSign.h"

MessageSign::MessageSign() :
	mMessageSign(std::make_unique<Object3D>())
{
	mMessageSign->SetModel(ModelManager::GetModel("MessageSign"));

	Init();
}

void MessageSign::Init()
{
	mMessageSign->scale = 1.5f;
}

void MessageSign::Update()
{
	ColliderUpdate();

	mMessageSign->Update();
}

void MessageSign::DrawModel()
{
	mMessageSign->Draw();
}

void MessageSign::ColliderUpdate()
{
	mCollider.centerPos = mMessageSign->pos;
	mCollider.radius = mMessageSign->scale.x;
}

Vec3 MessageSign::GetPos()
{
	return mMessageSign->pos;
}

Texture* MessageSign::GetMessageTexture()
{
	return mMessageTex;
}

SphereCollider MessageSign::GetCollider()
{
	return mCollider;
}

void MessageSign::SetMessageTexture(Texture* texture)
{
	mMessageTex = texture;
}

void MessageSign::SetPos(const Vec3 pos)
{
	mMessageSign->pos = pos;
}

void MessageSign::SetRot(const Vec3 rot)
{
	mMessageSign->rot = rot;
}

void MessageSign::SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline)
{
	mMessageSign->SetGraphicsPipeline(graphicsPipeline);
}

