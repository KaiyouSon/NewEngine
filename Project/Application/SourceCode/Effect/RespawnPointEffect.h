#pragma once
#include "NewEngine.h"

class RespawnPointEffect
{
private:
	std::unique_ptr<GPUEmitter> mEmitter;
	Timer mTimer;
	Vec3 mStartPos;

private:
	void GenerateUpdate();

public:
	RespawnPointEffect();
	void Init();
	void Generate(const Vec3 pos);
	void Update();
	void DrawModel();
};
