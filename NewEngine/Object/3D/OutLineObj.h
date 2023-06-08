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

	void Update(OutLineObj* parent = nullptr);
	void Draw();
};

