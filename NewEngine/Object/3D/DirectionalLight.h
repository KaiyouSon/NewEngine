#pragma once
#include "ConstantBuffer.h"
#include "Util.h"

class DirectionalLight
{
private:
	ConstantBuffer<ConstantBufferDataDirectionalLight>* constantBufferDirectionalLight;

public:
	Vec3 pos = { 1,0,0 };
	Vec3 color = { 1,1,1 };
	bool isActive = false;
	bool isDirty = false;
	//static DirectionalLight current;

public:
	DirectionalLight();
	~DirectionalLight();

	void Init();
	void Update();
	void Draw();
};

