#pragma once
#include "NewEngine.h"

class OutLineObj
{
private:
	std::unique_ptr<ConstantBuffer<ConstantBufferData::CTransform3D>> constantBufferTransform_;
	std::unique_ptr<ConstantBuffer<ConstantBufferData::CColor>> constantBufferColor_;
	Transform transform_;
	Transform* parent_;

public:
	Object3D* obj;
	Color color;

public:
	OutLineObj();

	void Update(Transform* parent = nullptr);
	void Draw();
};

