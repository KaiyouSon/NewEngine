#include "MessegeSign.h"

MessegeSign::MessegeSign() :
	messegeSign_(std::make_unique<Object3D>())
{
}

void MessegeSign::Init()
{
	messegeSign_->SetModel(ModelManager::GetModel("MessegeSign"));
	messegeSign_->pos = Vec3(10, 0, 5);
	messegeSign_->scale = 1.5f;
}

void MessegeSign::Update()
{
	messegeSign_->Update();
}

void MessegeSign::DrawModel()
{
	messegeSign_->Draw();
}
