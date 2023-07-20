#include "Field.h"

Field::Field() :
	ground_(std::make_unique<Object3D>()),
	sphere_(std::make_unique<Object3D>()),
	messageSign_(std::make_unique<MessageSign>()),
	skydome_(std::make_unique<Skydome>())
{
	ground_->SetModel(ModelManager::GetModel("Ground"));
	sphere_->SetModel(ModelManager::GetModel("Sphere"));

	Init();
}

void Field::Init()
{
	ground_->scale = 1000.f;
	ground_->tiling = 100;

	sphere_->pos = { -4,4,0 };
	sphere_->scale = 4.f;
	messageSign_->Init();
	skydome_->Init();

}

void Field::Update()
{
	sphere_->rot.y += Radian(1);
	sphere_->Update();

	ground_->Update();
	skydome_->Update();
	messageSign_->Update();
}

void Field::DrawModel()
{
	sphere_->Draw();
	ground_->Draw();
	skydome_->DrawModel();
	messageSign_->DrawModel();
}

void Field::DrawFrontSprite()
{
}

void Field::DrawDebugGui()
{
	skydome_->DrawDebugGui();
}

MessageSign* Field::GetMessageSign()
{
	return messageSign_.get();
}