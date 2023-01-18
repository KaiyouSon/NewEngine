#pragma once
#include "IScene.h"
#include "NewEngine.h"
#include "SilhouetteObj.h"

class GameScene : public IScene
{
private:
	Object3D skyDomeObj;
	Object3D groundObj;

	SilhouetteObj silhouetteObj;
	Object3D obj;
	Object3D obj2;
	Sprite spr;
	Quaternion q;

	void DebugGui();

private:
	// 当たり判定関連
	Object3D sphereObj;
	Object3D planeObj;
	Object3D triangleObj;
	Object3D rayObj;
	Object3D capsuleObj1;
	Object3D capsuleObj2;
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

