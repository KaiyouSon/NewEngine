#pragma once
#include "Color.h"
#include "FrameRate.h"
#include "Random.h"
#include "Singleton.h"
#include "Transform.h"
#include "SceneManager.h"
#include "Timer.h"
#include "Easing.h"
#include "BezierCurve.h"
#include <memory>
#include <vector>
#include <string>
#include <list>

static const int maxBoneIndices = 4;

enum class BlendMode
{
	Alpha,
	Add,
	Sub,
	Inv,
};

enum class TimeUnit
{
	MilliSecond,
	Second,
	Minute
};

// ��r���đ傫������Ԃ�
float Max(const float& a, const float& b);

// ��r���ď���������Ԃ�
float Min(const float& a, const float& b);

// ������Ԃ��i -1, 0, 1 �j
int Sign(const float& a);
Vec2 Sign(const Vec2& a);
Vec3 Sign(const Vec3& a);

// �l��Min��Max�̊Ԃɐ�������֐�
float Clamp(const float& value, const float& min = 0, const float& max = 1);

// �l����������֐�
float Convergence(const float& value, const float& speed, const float& origin = 0);

// �l�𒴂�����߂�
float Restore(const float& value, const float& limit, const float& origin = 0);

// �������擾
int GetDight(const int& value);

// ���ݎ��Ԃ�Ԃ��֐�
unsigned long GetNowTime(const TimeUnit& timeUnit = TimeUnit::MilliSecond);

// ���[���h���W���X�N���[�����W�ɕϊ�����
Vec2 WorldToScreen(const Vec3& worldPos);

// �O�p�`�̖@�������߂�
Vec3 GetTriangleNormal(const Vec3& p0, const Vec3& p1, const Vec3& p2);