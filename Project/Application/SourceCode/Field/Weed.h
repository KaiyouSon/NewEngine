#pragma once
#include "Grass.h"

class Weed
{
private:
	std::unique_ptr<Grass> mGrass;
	Vec2 mGenerateSize;

public:
	Weed();
	void Init();
	void Update();
	void DrawModel();

public:
	void SetPos(const Vec3 pos);
	void SetGenerateSize(const Vec2 size);
	void SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline);
};

