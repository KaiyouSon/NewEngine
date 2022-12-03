#pragma once
#include "Sprite.h"
#include "Object3D.h"

class TestScene
{
private:
	Object3D obj;
	Sprite spr;

public:
	void Load();
	void Init();
	void Update();
	void DrawFrontSprite();
	void DrawBackSprite();
	void DrawModel();
};