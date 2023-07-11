#pragma once
#include "NewEngine.h"

class PlayerBody;

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

	void CalcCurrentRot(PlayerBody* human);
	void ReverceRots();

	void Step0Init(PlayerBody* human);
	void Step0Update(PlayerBody* human);
	void Step1Init(PlayerBody* human);
	void Step1Update(PlayerBody* human);
	void Step2Init(PlayerBody* human);
	void Step2Update(PlayerBody* human);
	void Step3Init(PlayerBody* human);
	void Step3Update(PlayerBody* human);



public:
	MoveMotion();
	void Init(PlayerBody* human);
	void JoggingMotion(PlayerBody* human);
	void RunMotion(PlayerBody* human);

	void JoggingInit(PlayerBody* human);
	void RunInit(PlayerBody* human);

public:
	bool GetisPlay();
};

