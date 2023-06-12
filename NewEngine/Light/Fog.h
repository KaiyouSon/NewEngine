#pragma once
#include "ConstantBuffer.h"
#include <memory>

class Fog
{
private:
	std::unique_ptr<ConstantBuffer<ConstantBufferData::CFog>> constantBufferFog_;

public:
	bool isActive;
	float nearDis;
	float farDis;
	float pad;
	Color color;

public:
	Fog();
	void Update();
	void Draw();
};

