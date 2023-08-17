#include "Field.h"
#include "FieldDataManager.h"

Field::Field() :
	mSphere(std::make_unique<Object3D>()),
	mSkydome(std::make_unique<Skydome>()),
	mRespawnPoint(std::make_unique<RespawnPoint>()),
	mFieldData(nullptr)
{
	mSphere->SetModel(ModelManager::GetModel("Sphere"));
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

	mTrees.emplace_back(std::make_unique<Tree>());

	Init();

	mFieldData = FieldDataManager::GetFieldData("SkyIsland");
}

void Field::Init()
{
	mSphere->pos = { 0,0,0 };
	mSphere->scale = 2.0f;

	for (uint32_t i = 0; i < mMessageSigns.size(); i++)
	{
		mMessageSigns[i]->Init();
	}
	mSkydome->Init();
	mRespawnPoint->Init();

	for (uint32_t i = 0; i < mTrees.size(); i++)
	{
		mTrees[i]->Init();
	}

	if (mFieldData)
	{
		// Š»‰±
		for (uint32_t i = 0; i < mFieldData->coffins.size(); i++)
		{
			mFieldData->coffins[i]->Init();
		}


		// •‚“‡
		mFieldData->skyIslands.push_back(std::make_unique<SkyIsland>());
		for (uint32_t i = 0; i < mFieldData->skyIslands.size(); i++)
		{
			mFieldData->skyIslands[i]->Init();
		}
	}

}

void Field::Update()
{
	mSphere->rot.y += Radian(1);
	mSphere->Update();

	//mSphere->pos = { 0,8,0 };

	mSkydome->Update();
	mRespawnPoint->Update();
	for (uint32_t i = 0; i < mMessageSigns.size(); i++)
	{
		mMessageSigns[i]->Update();
	}

	for (uint32_t i = 0; i < mTrees.size(); i++)
	{
		mTrees[i]->Update();
	}

	if (mFieldData)
	{
		// Š»‰±
		for (uint32_t i = 0; i < mFieldData->coffins.size(); i++)
		{
			mFieldData->coffins[i]->Update();
		}

		// •‚“‡
		for (uint32_t i = 0; i < mFieldData->skyIslands.size(); i++)
		{
			mFieldData->skyIslands[i]->Update();
		}
	}
}

void Field::DrawModel()
{
	mSphere->Draw();
	mSkydome->DrawModel();
	for (uint32_t i = 0; i < mMessageSigns.size(); i++)
	{
		mMessageSigns[i]->DrawModel();
	}

	for (uint32_t i = 0; i < mTrees.size(); i++)
	{
		mTrees[i]->DrawModel();
	}

	if (mFieldData)
	{
		// Š»‰±
		for (uint32_t i = 0; i < mFieldData->coffins.size(); i++)
		{
			mFieldData->coffins[i]->DrawModel();
		}

		// •‚“‡
		for (uint32_t i = 0; i < mFieldData->skyIslands.size(); i++)
		{
			mFieldData->skyIslands[i]->DrawModel();
		}
	}

	mRespawnPoint->DrawModel();

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

RespawnPoint* Field::GetRespawnPoint()
{
	return mRespawnPoint.get();
}

FieldData* Field::GetFieldData()
{
	return mFieldData;
}

void Field::SetSpherePos(const Vec3 pos)
{
	mSphere->pos = pos;
}
