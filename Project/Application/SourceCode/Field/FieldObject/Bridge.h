#pragma once
#include "NewEngine.h"
#include "IFieldObject.h"

class Bridge : public IFieldObject
{
private:
	std::unique_ptr<Object3D> mBridge;

public:
	Bridge();
	void Init() override;
	void Update() override;
	void ExecuteCS() override {};
	void Draw(const bool isDrawDepth = false) override;

public:
	void SetTransform(const Transform& transform);
	void SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline);
};