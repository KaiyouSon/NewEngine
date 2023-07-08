#pragma once
#include "NewEngine.h"
#include "MotionManager.h"

class HumanoidBody;

class IMotion
{
protected:
	bool isInit_;
	bool isPlay_;
	bool isCanChangeMotion_;

	uint32_t step_;
	Easing ease_;

	std::vector<Vec3> curRots_;
	std::vector<Vec3> endRots_;
	Motion* motion_;

protected:
	void BaseInit(HumanoidBody* human);
	void BasePrevUpdate(HumanoidBody* human);
	void BasePostUpdate(HumanoidBody* human);

public:
	virtual void Init(HumanoidBody* human) = 0;
	virtual void Update(HumanoidBody* human) = 0;

public:
	bool GetisPlay();
	bool GetisCanChangeMotion();
};

