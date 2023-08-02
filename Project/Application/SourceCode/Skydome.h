#pragma once
#include "NewEngine.h"
#include "Cloud.h"

class Skydome
{
private:
	std::unique_ptr<Object3D> mSkydome;
	std::vector<std::unique_ptr<Cloud>> mClouds;

public:
	Skydome();
	void Init();
	void Update();
	void DrawModel();
	void DrawDebugGui();
};

