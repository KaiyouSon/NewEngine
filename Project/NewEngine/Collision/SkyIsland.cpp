#include "SkyIsland.h"

SkyIsland::SkyIsland() :
	mSkyIsland(std::make_unique<Object3D>())
{
}

void SkyIsland::Init()
{
	mSkyIsland->SetisShadow(true, false);
	mSkyIsland->isLighting = true;
}

void SkyIsland::Update()
{
	mSkyIsland->Update(&mParent);
}

void SkyIsland::DrawModel()
{
	mSkyIsland->Draw();
}

void SkyIsland::SetParent(const Transform parent)
{
	mParent = parent;
	mParent.Update();
}

void SkyIsland::SetModel(Model* model)
{
	mSkyIsland->SetModel(model);
}

void SkyIsland::SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline)
{
	mSkyIsland->SetGraphicsPipeline(graphicsPipeline);
}
