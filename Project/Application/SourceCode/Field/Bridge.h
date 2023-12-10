#pragma once
#include "NewEngine.h"

class Bridge
{
private:
	std::unique_ptr<Object3D> mBridge;

public:
	Bridge();
	void Init();
	void Update();
	void Draw();

public:
	void SetTransform(const Transform& transform);
	void SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline);
};