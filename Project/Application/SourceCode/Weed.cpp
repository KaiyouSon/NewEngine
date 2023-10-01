#include "Weed.h"

Weed::Weed() :
	mGrass(std::make_unique<Grass>())
{
	mGrass->pos = Vec3(0, 0, 10);
	mGrass->scale = 2.f;
	mGrass->color = Color(0xb8b137);
	mGrass->SetBillboardType(BillboardType::YAxisBillboard);

	mGenerateSize = 1;
}

void Weed::Init()
{
	float area = mGenerateSize.x * mGenerateSize.y;
	mGrass->GenerateGrassToSquare(mGenerateSize, (uint32_t)(area * 4));
	mGrass->texture = TextureManager::GetTexture("Weed");
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

void Weed::SetGenerateSize(const Vec2 size)
{
	mGenerateSize = size;
}

void Weed::SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline)
{
	mGrass->SetGraphicsPipeline(graphicsPipeline);
}
