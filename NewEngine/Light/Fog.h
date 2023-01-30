#pragma once
#include "ConstantBuffer.h"

class Fog
{
private:
	ConstantBuffer<ConstantBufferDataFog>* constantBufferFog;

public:
	bool isActive;
	float nearDis;
	float farDis;
	float pad;
	Color color;

public:
	Fog();
	~Fog();
	void Update();
	void Draw();
};

