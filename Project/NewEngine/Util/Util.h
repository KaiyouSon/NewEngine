#pragma once
#include "Color.h"
#include "FrameRate.h"
#include "Random.h"
#include "Singleton.h"
#include "Transform.h"
#include "Timer.h"
#include "Easing.h"
#include "BezierCurve.h"
#include "Enum.h"
#include "Rect.h"
#include <functional>

static const int maxBoneIndices = 4;

// ��r���đ傫������Ԃ�
template<typename T>
T Max(const T a, const T b)
{
	return a >= b ? a : b;
}

// ��r���ď���������Ԃ�
template<typename T>
T Min(const T a, const T b)
{
	return a <= b ? a : b;
}

// ������Ԃ��i -1, 0, 1 �j
uint32_t Sign(const float a);
Vec2 Sign(const Vec2 a);
Vec3 Sign(const Vec3 a);

// �l��Min��Max�̊Ԃɐ�������֐�
template<typename T>
T Clamp(const T value, const T min = 0, const T max = 1)
{
	if (value < min)
	{
		return min;
	}
	if (value > max)
	{
		return max;
	}
	return value;
}

// �l����������֐�
template<typename T>
T Convergence(const T value, const T speed, const T origin = 0)
{
	float temp = value;
	if (value == origin)
	{
		return origin;
	}
	else if (value > origin)
	{
		temp -= fabs(speed);
		return Max(temp, origin);
	}
	else if (value < origin)
	{
		temp += fabs(speed);
		return Min(temp, origin);
	}

	return -1;
}

// �l�𒴂�����߂�
template<typename T>
T Restore(const T value, const T limit, const T origin = 0)
{
	if (value >= limit)
	{
		return origin + value - limit;
	}
	return value;
}

// �������擾
uint32_t GetDight(const uint32_t value);

// ���ݎ��Ԃ�Ԃ��֐�
unsigned long GetNowTime(const TimeUnit timeUnit = TimeUnit::MilliSecond);

// ���[���h���W���X�N���[�����W�ɕϊ�����
Vec2 WorldToScreen(const Vec3 worldPos);

// �O�p�`�̖@�������߂�
Vec3 GetTriangleNormal(const Vec3 p0, const Vec3 p1, const Vec3 p2);

// Debug�r���h�̂ݎ��s����
void ProcessAtDebugBulid(std::function<void()> lambdaFunc);

// Release�r���h�g�̎��s����
void ProcessAtReleaseBulid(std::function<void()> lambdaFunc);

// �o�̓E�B���h�E�ɏo�͂���
void OutputDebugLog(const char* fmt...);