#pragma once
#include "NewEngine.h"

class OutLineObj
{
private:
	ConstantBuffer<ConstantBufferDataTransform3D>* constantBufferTransform;
	ConstantBuffer<ConstantBufferDataColor>* constantBufferColor;
	Transform transform;

public:
	Object3D* obj;
	Color color;

public:
	OutLineObj();
	~OutLineObj();

	void Update(const OutLineObj* parent = nullptr);
	void Draw();
};

