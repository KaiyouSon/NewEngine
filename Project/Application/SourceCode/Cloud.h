#pragma once
#include "NewEngine.h"

class Cloud
{
private:
	std::unique_ptr<Object3D> mCloud;

public:
	static void CreateGraphicsPipeline();

public:
	Cloud();
	void Init();
	void Update();
	void DrawModel();

public:
	void SetPos(const Vec3 pos);
};

