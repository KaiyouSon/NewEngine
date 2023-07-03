#pragma once
#include "NewEngine.h"

class HumanoidBody;

class MoveMotion
{
private:
	enum class MoveType
	{
		Jogging,
		Run
	};

private:
	bool isInit_;
	bool isEnd_;
	uint32_t step_;
	std::vector<Vec3> curRots_;
	std::vector<Vec3> startRots_;
	std::vector<Vec3> endRots_;
	Easing ease_;
	MoveType moveType_;
	MoveType prevMoveType_;

	uint32_t count_;
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
	void Step3Init(HumanoidBody* human);
	void Step3Update(HumanoidBody* human);


	void JoggingInit();
	void RunInit();

public:
	MoveMotion();
	void Init(HumanoidBody* human);
	void JoggingMotion(HumanoidBody* human);
	void RunMotion(HumanoidBody* human);

public:
	bool GetisEnd();
};

