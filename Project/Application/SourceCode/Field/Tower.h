#pragma once
#include "NewEngine.h"

class Tower
{
private:
	std::unique_ptr<Object3D> mTower;

public:
	Tower();
	void Init();
	void Update();
	void Draw();

public:
	void SetTransform(const Transform& transform);
	void SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline);

};