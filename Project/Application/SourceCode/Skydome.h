#pragma once
#include "NewEngine.h"
#include "Cloud.h"

class Skydome
{
private:
	std::unique_ptr<Object3D> skydome_;
	std::vector<std::unique_ptr<Cloud>> clouds_;

public:
	Skydome();
	void Init();
	void Update();
	void DrawModel();
};

