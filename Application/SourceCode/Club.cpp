#include "Club.h"
#include "MalletMotion.h"

Club::Club()
{
	weapon = std::make_unique<Object3D>();
	weapon->SetModel(ModelManager::GetModel("Club"));

	// ’Æ‚Ìƒ‚[ƒVƒ‡ƒ“
	motion = std::make_unique<MalletMotion>();

	localPos_ = Vec3(0.f, -1.5f, 0.f);
	localRot_ = Radian(Vec3(90.f, 0.f, 0.f));
}

void Club::Init()
{
	motion->Init();
}

void Club::Update(Transform* parent)
{
	weapon->Update(parent);

	collider.startPos;
}

void Club::DrawModel()
{
	weapon->Draw();
}
