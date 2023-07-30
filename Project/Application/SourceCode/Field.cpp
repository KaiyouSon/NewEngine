#include "Field.h"

Field::Field() :
	ground_(std::make_unique<Object3D>()),
	sphere_(std::make_unique<Object3D>()),
	skydome_(std::make_unique<Skydome>())
{
	ground_->SetModel(ModelManager::GetModel("Ground"));
	sphere_->SetModel(ModelManager::GetModel("Sphere"));

	for (uint32_t i = 0; i < messageSigns_.size(); i++)
	{
		messageSigns_[i] = std::make_unique<MessageSign>();
		messageSigns_[i]->SetRot(Vec3(0, Radian(90), 0));

		if (i < 3)
		{
			messageSigns_[i]->SetPos(Vec3(-20 + (float)(i * 20), 0, 50));
		}
		else
		{
			messageSigns_[i]->SetPos(Vec3(0, 0, 75 + (float)(i - 3) * 25));
		}
	}

	messageSigns_[0]->SetMessageTexture(TextureManager::GetTexture("TutorialStr2"));
	messageSigns_[1]->SetMessageTexture(TextureManager::GetTexture("TutorialStr1"));
	messageSigns_[2]->SetMessageTexture(TextureManager::GetTexture("TutorialStr3"));
	messageSigns_[3]->SetMessageTexture(TextureManager::GetTexture("TutorialStr4"));
	messageSigns_[4]->SetMessageTexture(TextureManager::GetTexture("TutorialStr5"));

	Init();
}

void Field::Init()
{
	ground_->scale = 1000.f;
	ground_->tiling = 100;

	sphere_->pos = { 0,0,0 };
	sphere_->scale = 4.f;

	for (uint32_t i = 0; i < messageSigns_.size(); i++)
	{
		messageSigns_[i]->Init();
	}
	skydome_->Init();
}

void Field::Update()
{
	sphere_->rot.y += Radian(1);
	sphere_->Update();

	ground_->Update();
	skydome_->Update();
	for (uint32_t i = 0; i < messageSigns_.size(); i++)
	{
		messageSigns_[i]->Update();
	}
}

void Field::DrawModel()
{
	//sphere_->Draw();
	ground_->Draw();
	skydome_->DrawModel();
	for (uint32_t i = 0; i < messageSigns_.size(); i++)
	{
		messageSigns_[i]->DrawModel();
	}
}

void Field::DrawFrontSprite()
{
}

void Field::DrawDebugGui()
{
	skydome_->DrawDebugGui();
}

std::array<std::unique_ptr<MessageSign>, 5>* Field::GetMessageSigns()
{
	return &messageSigns_;
}