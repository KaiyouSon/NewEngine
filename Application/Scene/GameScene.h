#pragma once
#include "IScene.h"
#include "NewEngine.h"

class GameScene : public IScene
{
private:
	Object3D obj;
	Object3D obj2;
	Sprite spr;

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

