#pragma once
#include "Grass.h"

// 草をひとまとめにしたクラス
class Weed
{
private:
	static uint32_t sDensity;

private:
	std::unique_ptr<Grass> mGrass;

public:
	Weed();
	void Init();
	void Update();
	void DrawModel();

public:
	void SetPos(const Vec3 pos);
	void SetGenerateSize(const Vec2 size);
	void SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline);

public:
	void GenerateToSquare(const Vec2 size);
	void GenerateToSphere(const float radius);
};
