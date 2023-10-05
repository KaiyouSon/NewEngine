#pragma once
#include "NewEngine.h"

// �ŋߎg���ĂȂ����߃��t�@�N�^�����O�͌��
class OutLineObj
{
private:
	std::unique_ptr<ConstantBuffer<ConstantBufferData::CTransform3D>> mConstantBufferTransform;
	std::unique_ptr<ConstantBuffer<ConstantBufferData::CColor>> mConstantBufferColor;
	GraphicsPipeline* mGraphicsPipeline;
	Transform mTransform;
	Transform* mParent;

public:
	Object3D* obj;
	Color color;

public:
	OutLineObj();
	OutLineObj(const OutLineObj& other) = delete;

	void Update(Transform* parent = nullptr);
	void Draw();
};

