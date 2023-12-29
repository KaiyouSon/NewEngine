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

void SkyIsland::Draw(const bool isDrawDepth)
{
	if (isDrawDepth == false)
	{
		SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Object3D"));
	}
	else
	{
		SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Object3DWriteNone"));
	}
	mSkyIsland->Draw("Object3D");
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
