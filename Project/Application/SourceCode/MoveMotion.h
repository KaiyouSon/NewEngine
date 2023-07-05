#pragma once
#include "NewEngine.h"

class HumanoidBody;

class MoveMotion
{
private:
	bool isPlay_;
	bool isInit_;
	uint32_t step_;
	std::vector<Vec3> curRots_;
	std::vector<Vec3> startRots_;
	std::vector<Vec3> endRots_;
	Easing ease_;
	float curSpeed_;
	float endSpeed_;

	uint32_t count_;
private:

	void CalcCurrentRot(HumanoidBody* human);
	void ReverceRots();

	void Step0Init(HumanoidBody* human);
	void Step0Update(HumanoidBody* human);
	void Step1Init(HumanoidBody* human);
	void Step1Update(HumanoidBody* human);
	void Step2Init(HumanoidBody* human);
	void Step2Update(HumanoidBody* human);
	void Step3Init(HumanoidBody* human);
	void Step3Update(HumanoidBody* human);



public:
	MoveMotion();
	void Init(HumanoidBody* human);
	void JoggingMotion(HumanoidBody* human);
	void RunMotion(HumanoidBody* human);

	void JoggingInit(HumanoidBody* human);
	void RunInit(HumanoidBody* human);

public:
	bool GetisPlay();
};

