#pragma once
#include "ConstantBuffer.h"
#include "Util.h"

class DirectionalLight
{
private:
	ConstantBuffer<ConstantBufferDataDirectionalLight>* constantBufferDirectionalLight;

public:
	Vec3 pos;
	Color color;
	bool isActive;

public:
	DirectionalLight();
	~DirectionalLight();

	void Init();
	void Update();
	void Draw();
};

