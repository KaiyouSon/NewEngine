#include "Weed.h"

Weed::Weed() :
	mGrass(std::make_unique<Grass>())
{
	mGrass->pos = Vec3(0, 0, 10);
	mGrass->scale = 1.5f;
	mGrass->texture = TextureManager::GetTexture("Weed");
	mGrass->GenerateGrassToSquare(Vec2(2, 2), 32);
	mGrass->SetBillboardType(BillboardType::YAxisBillboard);
}

void Weed::Init()
{
}

void Weed::Update()
{
	mGrass->Update();
}

void Weed::DrawModel()
{
	mGrass->Draw();
}
