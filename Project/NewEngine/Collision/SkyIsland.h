#pragma once
#include "NewEngine.h"

// 空島のクラス
class SkyIsland
{
private:
	Transform mParent;
	std::unique_ptr<Object3D> mSkyIsland;

public:
	SkyIsland();
	void Init();
	void Update();
	void DrawModel();

public:
	void SetParent(const Transform parent);
	void SetModel(Model* model);
	void SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline);
};
