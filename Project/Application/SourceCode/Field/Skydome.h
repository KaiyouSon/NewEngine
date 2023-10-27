#pragma once
#include "NewEngine.h"
#include "Cloud.h"

// 天球のクラス
class Skydome
{
private:
	std::unique_ptr<Object3D> mSkydome;
	std::vector<std::unique_ptr<Cloud>> mClouds;

public:
	Skydome();
	void Init();
	void Update();
	void Draw();
};
