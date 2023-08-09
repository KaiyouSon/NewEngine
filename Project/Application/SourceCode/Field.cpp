#include "Field.h"

Field::Field() :
	mGround(std::make_unique<Object3D>()),
	mSphere(std::make_unique<Object3D>()),
	mSkydome(std::make_unique<Skydome>())
{
	mGround->SetModel(ModelManager::GetModel("Ground"));
	mSphere->SetModel(ModelManager::GetModel("Sphere"));

	mGround->SetisShadow(true, false);
	//mSphere->SetisShadow(true);

	for (uint32_t i = 0; i < mMessageSigns.size(); i++)
	{
		mMessageSigns[i] = std::make_unique<MessageSign>();
		mMessageSigns[i]->SetRot(Vec3(0, Radian(90), 0));

		if (i < 3)
		{
			mMessageSigns[i]->SetPos(Vec3(-20 + (float)(i * 20), 0, 50));
		}
		else
		{
			mMessageSigns[i]->SetPos(Vec3(0, 0, 75 + (float)(i - 3) * 25));
		}
	}

	mMessageSigns[0]->SetMessageTexture(TextureManager::GetTexture("TutorialStr2"));
	mMessageSigns[1]->SetMessageTexture(TextureManager::GetTexture("TutorialStr1"));
	mMessageSigns[2]->SetMessageTexture(TextureManager::GetTexture("TutorialStr3"));
	mMessageSigns[3]->SetMessageTexture(TextureManager::GetTexture("TutorialStr4"));
	mMessageSigns[4]->SetMessageTexture(TextureManager::GetTexture("TutorialStr5"));

	Init();
}

void Field::Init()
{
	mGround->scale = 1000.f;
	mGround->tiling = 100;

	mSphere->pos = { 0,0,0 };
	mSphere->scale = 4.f;

	for (uint32_t i = 0; i < mMessageSigns.size(); i++)
	{
		mMessageSigns[i]->Init();
	}
	mSkydome->Init();
}

void Field::Update()
{
	mSphere->rot.y += Radian(1);
	mSphere->Update();

	//mSphere->pos = { 0,8,0 };

	mGround->Update();
	mSkydome->Update();
	for (uint32_t i = 0; i < mMessageSigns.size(); i++)
	{
		mMessageSigns[i]->Update();
	}
}

void Field::DrawModel()
{
	//mSphere->Draw();
	mGround->Draw();
	mSkydome->DrawModel();
	for (uint32_t i = 0; i < mMessageSigns.size(); i++)
	{
		mMessageSigns[i]->DrawModel();
	}
}

void Field::DrawFrontSprite()
{
}

void Field::DrawDebugGui()
{
	mSkydome->DrawDebugGui();
}

std::array<std::unique_ptr<MessageSign>, 5>* Field::GetMessageSigns()
{
	return &mMessageSigns;
}