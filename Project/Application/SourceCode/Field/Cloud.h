#pragma once
#include "NewEngine.h"

// 雲のクラス
class Cloud
{
private:
	std::unique_ptr<Object3D> mCloud;

public:
	Cloud();
	void Init();
	void Update();
	void DrawModel();

public:
	void SetPos(const Vec3 pos);
};
