#pragma once
#include "NewEngine.h"

class Grass
{
private:
	std::vector<VertexBufferData::VGrass> mVertices;
	std::unique_ptr<VertexBuffer<VertexBufferData::VGrass>> mVertexBuffer;
	std::vector<Timer> mTimers;
	Transform mTransform;
	Transform* mParent;
	Material mMaterial;
	GraphicsPipeline* mGraphicsPipeline;
	Billboard mBillboard;

public:
	Vec3 pos;
	Vec3 scale;
	Vec3 rot;
	Color color;
	Texture* texture;

private:
	void MaterialInit();
	void MaterialTransfer();
	void MaterialDrawCommands();

public:
	static void CreateGraphicsPipeline();

	Grass();
	void GenerateGrassToSquare(const Vec2 size, const uint32_t maxNum);
	void Update(Transform* parent = nullptr);
	void Draw();

public:
	void SetBillboardType(const BillboardType billboardType);
	void SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline);
};

