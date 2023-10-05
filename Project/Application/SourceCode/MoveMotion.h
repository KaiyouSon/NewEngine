#pragma once
#include "NewEngine.h"

class PlayerBody;

class MoveMotion
{
private:
	bool mIsPlay;
	bool mIsInit;
	uint32_t mStep;
	std::vector<Vec3> mCurRots;
	std::vector<Vec3> mStartRots;
	std::vector<Vec3> mEndRots;
	Easing mEase;
	float mCurSpeed;
	float mEndSpeed;

	uint32_t mCount;

private:
	void CalcCurrentRot(PlayerBody* human);
	void ReverceRots();

	void Step0Init();
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

