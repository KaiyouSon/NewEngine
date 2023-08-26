#include "Skydome.h"

Skydome::Skydome() :
	mSkydome(std::make_unique<Object3D>()),
	mPostEffect(std::make_unique<PostEffect>())
{
	mSkydome->SetModel(ModelManager::GetModel("Skydome"));
	mSkydome->color = Color(0xc4c178);
	mSkydome->isLighting = false;

	for (uint32_t i = 0; i < 50; i++)
	{
		mClouds.emplace_back(std::move(std::make_unique<Cloud>()));

		const float range = 500.f;
		const Vec3 randomPos =
		{
			Random::RangeF(-range, range),
			150.f,
			Random::RangeF(-range, range),
		};
		mClouds.back()->SetPos(randomPos);
	}

	mRenderTexture = TextureManager::GetRenderTexture("Skydome");
	mPostEffect->AddRenderTexture(mRenderTexture);
}

void Skydome::Init()
{
	mSkydome->scale = 800.f;
}

void Skydome::Update()
{
	mSkydome->Update();
	for (uint32_t i = 0; i < mClouds.size(); i++)
	{
		mClouds[i]->Update();
	}
}

void Skydome::Draw()
{
	mSkydome->Draw();
	for (uint32_t i = 0; i < mClouds.size(); i++)
	{
		mClouds[i]->DrawModel();
	}
}

void Skydome::DrawDebugGui()
{
	Gui::BeginWindow("Skydome");
	Gui::DrawColorEdit("Skydome Color", mSkydome->color);
	Gui::EndWindow();
}
