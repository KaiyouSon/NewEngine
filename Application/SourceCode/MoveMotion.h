#pragma once
#include "NewEngine.h"

class HumanoidBody;

class MoveMotion
{
private:
	bool isInit_;
	bool isEnd_;
	uint32_t step_;
	std::vector<Vec3> curRots_;
	std::vector<Vec3> startRots_;
	std::vector<Vec3> endRots_;
	Easing ease_;

private:
	void RotsInit(HumanoidBody* human);
	void CalcCurrentRot(HumanoidBody* human);
	void ReverceRots();

	void Step0Init(HumanoidBody* human);
	void Step0Update(HumanoidBody* human);
	void Step1Init(HumanoidBody* human);
	void Step1Update(HumanoidBody* human);
	void Step2Init(HumanoidBody* human);
	void Step2Update(HumanoidBody* human);

public:
	MoveMotion();
	void Init(HumanoidBody* human);
	void JoggingMotion(HumanoidBody* human);

public:
	bool GetisEnd();
};

