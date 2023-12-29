#pragma once
#include "NewEngine.h"
#include "IFieldObject.h"

// 空島のクラス
class SkyIsland : public IFieldObject
{
private:
	Transform mParent;
	std::unique_ptr<Object3D> mSkyIsland;

public:
	SkyIsland();
	void Init() override;
	void Update() override;
	void ExecuteCS() override {};
	void Draw(const bool isDrawDepth = false) override;

public:
	void SetParent(const Transform parent);
	void SetModel(Model* model);
	void SetGraphicsPipeline(GraphicsPipeline* graphicsPipeline);
};
