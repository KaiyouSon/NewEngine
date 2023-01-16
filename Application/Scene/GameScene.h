#pragma once
#include "IScene.h"
#include "NewEngine.h"

class GameScene : public IScene
{
private:
	Object3D skyDomeObj;
	Object3D groundObj;

	Object3D obj;
	Object3D obj2;
	Sprite spr;
	Quaternion q;




private:
	// 当たり判定関連
	Object3D sphereObj;
	Object3D planeObj;
	Object3D triangleObj;
	Object3D rayObj;
	int currentCollision;

	void CollisionInit();
	void CollisionUpdate();
	void CollisionDrawModel();
	void CollisionDrawGui();

private:
	// ライト関連
	void DirectionalLightDrawGui();
	void PointLightDrawGui();
	void SpotLightDrawGui();

public:
	GameScene();
	~GameScene();
	void Init() override;
	void Update() override;
	void DrawBackSprite() override;
	void DrawModel() override;
	void DrawFrontSprite() override;
	void DrawDebugGui() override;
};

