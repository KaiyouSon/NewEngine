#include "Tower.h"

Tower::Tower() :
	mTower(std::make_unique<Object3D>())
{
	mTower->SetModel(ModelManager::GetModel("Tower"));
	mType = FieldObjectType::Tower;
}

void Tower::Init()
{
}

void Tower::Update()
{
	mTower->Update();
}

void Tower::Draw(const bool isDrawDepth)
{
	isDrawDepth;
	mTower->Draw();
}

void Tower::SetTransform(const Transform& transform)
{
	mTower->pos = transform.pos;
	mTower->scale = transform.scale;
	mTower->rot = transform.rot;
}

void Tower::SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline)
{
	mTower->SetGraphicsPipeline(graphicsPipeline);
}
