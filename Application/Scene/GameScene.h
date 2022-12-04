#pragma once
#include "IScene.h"
#include "Util.h"
#include "Object3D.h"
#include "Sprite.h"

class GameScene : public IScene
{
private:
	Object3D obj[10];
	Sprite spr;

public:
	void Init() override;
	void Update() override;
	void DrawBackSprite() override;
	void DrawModel() override;
	void DrawFrontSprite() override;
};

