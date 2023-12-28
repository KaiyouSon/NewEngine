#include "Bridge.h"

Bridge::Bridge() :
	mBridge(std::make_unique<Object3D>())
{
	mBridge->SetModel(ModelManager::GetModel("Bridge"));
	mType = FieldObjectType::Bridge;
}

void Bridge::Init()
{
	mBridge->rot.y += Radian(90);
}

void Bridge::Update()
{
	mBridge->Update();
}

void Bridge::Draw(const bool isDrawDepth)
{
	isDrawDepth;
	mBridge->Draw();
}

void Bridge::SetTransform(const Transform& transform)
{
	mBridge->pos = transform.pos;
	mBridge->scale = transform.scale;
	mBridge->rot = transform.rot;
}

void Bridge::SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline)
{
	mBridge->SetGraphicsPipeline(graphicsPipeline);
}
