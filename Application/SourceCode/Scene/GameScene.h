#pragma once
#include "NewEngine.h"
#include "RenderTexture.h"

class GameScene : public IScene
{
private:
	bool isCG3;
	bool isAL4;

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
	// CG3関連
	Object3D skyDomeObj;
	Object3D groundObj;
	SilhouetteObj silhouetteObj;
	OutLineObj outlineObj;
	Object3D obj;
	Object3D obj2;
	Object3D obj3;

	void DirectionalLightDrawGui();
	void PointLightDrawGui();
	void SpotLightDrawGui();
	void CircleDrawGui();
	void FogDrawGui();

	void CG3Init();
	void CG3Update();
	void CG3DrawModel();
	void CG3DrawGui();
	void CG3DrawModelGui();

private:
	// クォータニオン関連
	void QuaternionDrawGui();

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

