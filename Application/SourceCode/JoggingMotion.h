#pragma once
#include "NewEngine.h"

class HumanoidBody;

class JoggingMotion
{
private:
	bool isReverce_;
	uint32_t step;
	std::vector<Vec3> startRots_;
	std::vector<Vec3> endRots_;
	Easing ease_;

private:
	void ReverceRots();

	void RotsInit(HumanoidBody* human);
public:
	JoggingMotion();
	void Init(HumanoidBody* human);
	void Update(HumanoidBody* human);
};

