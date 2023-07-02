#include "Club.h"

Club::Club()
{
	weapon = std::make_unique<Object3D>();
	weapon->SetModel(ModelManager::GetModel("Club"));

	localPos_ = Vec3(0.f,-1.5f,0.f);
	localRot_ = Radian(Vec3(90.f, 0.f, 0.f));
}

void Club::Init()
{
}

void Club::Update(Transform* parent)
{
	weapon->Update(parent);
}

void Club::DrawModel()
{
	weapon->Draw();
}
