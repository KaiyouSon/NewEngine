#pragma once
#include "NewEngine.h"

// シャドウマップに描画する用のクラス
class ShadowObj
{
private:
	Transform mTransform;
	Transform* mParent;
	Model* mModel;
	Material mMaterial;
	GraphicsPipeline* mGraphicsPipeline;
	Camera* mCamera;

public:
	Vec3 pos;
	Vec3 scale;
	Vec3 rot;

private:
	void MaterialInit();
	void MaterialTransfer();
	void MaterialDrawCommands();

public:
	ShadowObj();
	void Update(Transform* parent = nullptr);
	void Draw();

public: //セッター

	// モデル
	void SetModel(Model* model);

	// カメラ
	void SetCamera(Camera* camera = nullptr);

	// 親
	void SetParent(Transform* parent);

};

