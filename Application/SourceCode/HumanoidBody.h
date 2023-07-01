#pragma once
#include "NewEngine.h"

enum class PartID
{
	Transform,

	Head,	// 頭
	Body,	// 体

	RightArm,	// 右腕
	RightHand,	// 右手
	LeftArm,	// 左腕
	LeftHand,	// 左手

	RightThigh,	 // 右太もも
	RightLeg,	 // 右足
	LeftThigh,	 // 左太もも
	LeftLeg,	 // 左足
};

class HumanoidBody
{
private:
	std::array<std::unique_ptr<Object3D>, 11> parts_;

private:
	float rotSpped;
	bool isReverce;
	Easing joggingEase;

public:
	HumanoidBody();
	void Init();
	void Update();
	void DrawModel();
	void DrawDebugGui();

public:
	void IdleMotion();
	void JoggingMotion();
};

