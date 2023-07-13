#pragma once
#include "NewEngine.h"

class Skydome
{
private:
	std::unique_ptr<Object3D> skydome_;

public:
	Skydome();
	void Init();
	void Update();
	void DrawModel();
};

