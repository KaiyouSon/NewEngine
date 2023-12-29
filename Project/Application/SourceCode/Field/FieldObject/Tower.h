#pragma once
#include "NewEngine.h"
#include "IFieldObject.h"

class Tower : public IFieldObject
{
private:
	std::unique_ptr<Object3D> mTower;

public:
	Tower();
	void Init() override;
	void Update() override;
	void ExecuteCS() override {};
	void Draw(const bool isDrawDepth = false) override;

public:
	void SetTransform(const Transform& transform);
	void SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline);

};