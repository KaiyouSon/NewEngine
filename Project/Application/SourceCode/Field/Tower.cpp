#include "Tower.h"

Tower::Tower() :
	mTower(std::make_unique<Object3D>())
{
	mTower->SetModel(ModelManager::GetModel("Tower"));
}

void Tower::Init()
{
}

void Tower::Update()
{
	mTower->Update();
}

void Tower::Draw()
{
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
