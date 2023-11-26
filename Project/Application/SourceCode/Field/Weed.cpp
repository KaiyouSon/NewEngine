#include "Weed.h"

uint32_t Weed::sDensity = 8;

Weed::Weed() :
	mGrass(std::make_unique<Grass>())
{
	mGrass->pos = Vec3(0, 0, 10);
	mGrass->scale = 2.f;
	mGrass->color = Color(0xb8b137);
	mGrass->SetBillboardType(BillboardType::YAxisBillboard);

	mGrass->texture = TextureManager::GetTexture("Weed");
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

void Weed::SetPos(const Vec3 pos)
{
	mGrass->pos = pos;
}

void Weed::SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline)
{
	mGrass->SetGraphicsPipeline(graphicsPipeline);
}

void Weed::GenerateToSquare(const Vec2 size)
{
	mGrass->GenerateGrassToSquare(size, sDensity);
}

void Weed::GenerateToSphere(const float radius)
{
	mGrass->GenerateGrassToSphere(radius, sDensity / 4);
}
