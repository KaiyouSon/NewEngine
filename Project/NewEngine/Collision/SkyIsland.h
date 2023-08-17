#pragma once
#include "NewEngine.h"

class SkyIsland
{
private:
	Transform mParent;
	std::unique_ptr<Object3D> mSkyIsland;

public:
	SkyIsland();
	void Init();
	void Update();
	void DrawModel();

public:
	void SetParent(const Transform parent);
};

