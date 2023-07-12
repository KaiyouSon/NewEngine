#pragma once
#include "MessegeSign.h"

class Field
{
private:
	std::unique_ptr<Object3D> ground_;
	std::unique_ptr<MessegeSign> messegeSign_;

private:
	// デバッグ用
	std::unique_ptr<Object3D> sphere_;

public:
	Field();
	void Init();
	void Update();
	void DrawModel();
	void DrawFrontSprite();
};

