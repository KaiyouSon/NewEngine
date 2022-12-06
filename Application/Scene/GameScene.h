#pragma once
#include "IScene.h"
#include "NewEngine.h"

class GameScene : public IScene
{
private:
	Object3D obj[10];
	Sprite spr;
	Model m1;
	Model m2;

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

