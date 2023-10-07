#include "Cloud.h"

Cloud::Cloud() :
	mCloud(std::make_unique<Object3D>())
{
	mCloud->SetModel(ModelManager::GetModel("Cloud"));
	mCloud->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Cloud"));
	mCloud->color = Color(0xc4c178);

	Init();
}

void Cloud::Init()
{
	mCloud->pos.y = 180.f;
	mCloud->scale = 100.f;
	mCloud->rot.x = Radian(180);
}

void Cloud::Update()
{
	mCloud->offset.x += 0.0005f;
	mCloud->Update();
}

void Cloud::DrawModel()
{
	mCloud->Draw();
}

void Cloud::SetPos(const Vec3 pos)
{
	mCloud->pos = pos;
}
