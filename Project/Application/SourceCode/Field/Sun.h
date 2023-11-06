#pragma once
#include "NewEngine.h"

class Sun
{
private:
	std::unique_ptr<ParticleObject> mSun;

public:
	Sun();
	void Init();
	void Update();
	void Draw();

public:
	void SetTransform(const Transform& transform);
};

