#pragma once
#include "Grass.h"
#include "IFieldObject.h"

// 草をひとまとめにしたクラス
class Weed : public IFieldObject
{
private:
	static uint32_t sDensity;

private:
	std::unique_ptr<Grass> mGrass;

public:
	Weed();
	void Init() override;
	void Update() override;
	void ExecuteCS() override {};
	void Draw(const bool isDrawDepth = false) override;

public:
	void SetPos(const Vec3 pos);

public:
	void GenerateToSquare(const Vec2 size);
	void GenerateToSphere(const float radius);
};
