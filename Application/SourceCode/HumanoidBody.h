#pragma once
#include "NewEngine.h"

enum class PartID
{
	Transform,

	Head,	// ��
	Body,	// ��

	RightArm,	// �E�r
	RightHand,	// �E��
	LeftArm,	// ���r
	LeftHand,	// ����

	RightThigh,	 // �E������
	RightLeg,	 // �E��
	LeftThigh,	 // ��������
	LeftLeg,	 // ����
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

