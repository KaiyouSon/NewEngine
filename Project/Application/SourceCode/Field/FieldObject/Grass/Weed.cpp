#include "Weed.h"

uint32_t Weed::sDensity = 6;

Weed::Weed() :
	mGrass(std::make_unique<Grass>())
{
	mGrass->pos = Vec3(0, 0, 10);
	mGrass->scale = 2.f;
	mGrass->color = Color(0xb8b137);

	mGrass->texture = TextureManager::GetTexture("Weed");
}

void Weed::Init()
{
	mGrass->SetBillboardType(BillboardType::YAxisBillboard);
}

void Weed::Update()
{
	mGrass->Update();
}

void Weed::Draw(const bool isDrawDepth)
{
	if (isDrawDepth == false)
	{
		mGrass->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("Grass"));
	}
	else
	{
		mGrass->SetGraphicsPipeline(PipelineManager::GetGraphicsPipeline("GrassWriteNone"));
	}

	mGrass->Draw();
}

void Weed::SetPos(const Vec3 pos)
{
	mGrass->pos = pos;
}

void Weed::GenerateToSquare(const Vec2 size)
{
	mGrass->GenerateGrassToSquare(size, sDensity);
}

void Weed::GenerateToSphere(const float radius)
{
	mGrass->GenerateGrassToSphere(radius, sDensity / 4);
}
