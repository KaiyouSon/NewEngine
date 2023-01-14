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

