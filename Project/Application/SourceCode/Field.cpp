#include "Field.h"
#include "FieldDataManager.h"

Field::Field() :
	mSphere(std::make_unique<Object3D>()),
	mSkydome(std::make_unique<Skydome>()),
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

	if (mFieldData)
	{
		// ����
		for (uint32_t i = 0; i < mFieldData->coffins.size(); i++)
		{
			mFieldData->coffins[i]->Init();
		}

		// ����
		for (uint32_t i = 0; i < mFieldData->skyIslands.size(); i++)
		{
			mFieldData->skyIslands[i]->Init();
		}

		// ��
		for (uint32_t i = 0; i < mFieldData->trees.size(); i++)
		{
			mFieldData->trees[i]->Init();
		}

		// �G��
		for (uint32_t i = 0; i < mFieldData->weeds.size(); i++)
		{
			mFieldData->weeds[i]->Init();
		}

		// ��
		for (uint32_t i = 0; i < mFieldData->walls.size(); i++)
		{
			mFieldData->walls[i]->Init();
		}

		// ����
		for (uint32_t i = 0; i < mFieldData->gates.size(); i++)
		{
			mFieldData->gates[i]->Init();
		}

		// ���X�|�[���n�_
		for (uint32_t i = 0; i < mFieldData->respawnPoints.size(); i++)
		{
			mFieldData->respawnPoints[i]->Init();
		}

	}
}

void Field::Update()
{
	mSphere->rot.y += Radian(1);
	mSphere->Update();
	//mSphere->pos = { 0,8,0 };

	mSkydome->Update();
	for (uint32_t i = 0; i < mMessageSigns.size(); i++)
	{
		mMessageSigns[i]->Update();
	}

	if (mFieldData)
	{
		// ����
		for (uint32_t i = 0; i < mFieldData->coffins.size(); i++)
		{
			mFieldData->coffins[i]->Update();
		}

		// ����
		for (uint32_t i = 0; i < mFieldData->skyIslands.size(); i++)
		{
			mFieldData->skyIslands[i]->Update();
		}

		// ��
		for (uint32_t i = 0; i < mFieldData->trees.size(); i++)
		{
			mFieldData->trees[i]->Update();
		}

		// �G��
		for (uint32_t i = 0; i < mFieldData->weeds.size(); i++)
		{
			mFieldData->weeds[i]->Update();
		}

		// ��
		for (uint32_t i = 0; i < mFieldData->walls.size(); i++)
		{
			mFieldData->walls[i]->Update();
		}

		// ����
		for (uint32_t i = 0; i < mFieldData->gates.size(); i++)
		{
			mFieldData->gates[i]->Update();
		}

		// ���X�|�[���n�_
		for (uint32_t i = 0; i < mFieldData->respawnPoints.size(); i++)
		{
			mFieldData->respawnPoints[i]->Update();
		}
	}
}

void Field::RenderTextureSetting()
{
	mSkydome->RenderTextureSetting();
}

void Field::DrawModel()
{
	//mSphere->Draw();
	mSkydome->Draw();
	for (uint32_t i = 0; i < mMessageSigns.size(); i++)
	{
		mMessageSigns[i]->DrawModel();
	}

	if (mFieldData)
	{
		// ����
		for (uint32_t i = 0; i < mFieldData->coffins.size(); i++)
		{
			mFieldData->coffins[i]->DrawModel();
		}

		// ����
		for (uint32_t i = 0; i < mFieldData->skyIslands.size(); i++)
		{
			mFieldData->skyIslands[i]->DrawModel();
		}

		// ��
		for (uint32_t i = 0; i < mFieldData->trees.size(); i++)
		{
			mFieldData->trees[i]->DrawModel();
		}

		// �G��
		for (uint32_t i = 0; i < mFieldData->weeds.size(); i++)
		{
			mFieldData->weeds[i]->DrawModel();
		}

		// ��
		for (uint32_t i = 0; i < mFieldData->walls.size(); i++)
		{
			mFieldData->walls[i]->DrawModel();
		}

		// ����
		for (uint32_t i = 0; i < mFieldData->gates.size(); i++)
		{
			mFieldData->gates[i]->DrawModel();
		}

		// ���X�|�[���n�_
		for (uint32_t i = 0; i < mFieldData->respawnPoints.size(); i++)
		{
			mFieldData->respawnPoints[i]->DrawModel();
		}
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

FieldData* Field::GetFieldData()
{
	return mFieldData;
}

void Field::SetSpherePos(const Vec3 pos)
{
	mSphere->pos = pos;
}
