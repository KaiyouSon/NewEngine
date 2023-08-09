#pragma once
#include "NewEngine.h"

class RespawnPoint
{
private:
	std::unique_ptr<Object3D> mRipple;
	std::unique_ptr<Object3D> mRhombus;

	Timer angle;

public:
	static void CreateGraphicsPipeline();
	RespawnPoint();
	void Init();
	void Update();
	void DrawModel();
};

