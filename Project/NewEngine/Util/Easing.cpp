#include "Easing.h"
#include "Util.h"
#include <math.h>

Easing::Easing() :
	mTimer(0), mTimeRate(0),
	mLimitTimer(0), mPowNum(1), mIsEnd(false)
{
}
Easing::Easing(const int32_t easeTimer) :
	mTimer(0), mTimeRate(0),
	mLimitTimer(easeTimer), mPowNum(1), mIsEnd(false)
{
}
Easing::Easing(const int32_t easeTimer, const float powNum) :
	mTimer(0), mTimeRate(0),
	mLimitTimer(easeTimer), mPowNum(powNum), mIsEnd(false)
{
}

void Easing::Reset()
{
	mTimer = 0;
	mTimeRate = 0;
	mIsEnd = false;
}

void Easing::Update()
{
	mTimer++;
	if (mTimeRate >= 1)
	{
		mIsEnd = true;
	}

	mTimer = Min<int32_t>(mTimer, mLimitTimer);
	mTimeRate = Min<float>((float)mTimer / mLimitTimer, 1);
}

// イージング終わり
bool Easing::GetisEnd()
{
	return mIsEnd;
}
// タイマー取得
int32_t Easing::GetTimer()
{
	return mTimer;
}
// タイムレートを取得
float Easing::GetTimeRate()
{
	return mTimeRate;
}
// 補間タイムを取得
uint32_t Easing::GetEaseTimer()
{
	return mLimitTimer;
}

float Easing::Interpolation(const float startPos, const float endPos)
{
	switch (mEaseType)
	{
	case EaseType::Lerp:
		return Lerp(startPos, endPos);

	case EaseType::In:
		return In(startPos, endPos);

	case EaseType::Out:
		return Out(startPos, endPos);

	case EaseType::InOut:
		return InOut(startPos, endPos);

	default:
		return 0;
	}
}

Vec2 Easing::Interpolation(const Vec2 startPos, const Vec2 endPos)
{
	switch (mEaseType)
	{
	case EaseType::Lerp:
		return Lerp(startPos, endPos);

	case EaseType::In:
		return In(startPos, endPos);

	case EaseType::Out:
		return Out(startPos, endPos);

	case EaseType::InOut:
		return InOut(startPos, endPos);

	default:
		return 0;
	}
}

Vec3 Easing::Interpolation(const Vec3 startPos, const Vec3 endPos)
{
	switch (mEaseType)
	{
	case EaseType::Lerp:
		return Lerp(startPos, endPos);

	case EaseType::In:
		return In(startPos, endPos);

	case EaseType::Out:
		return Out(startPos, endPos);

	case EaseType::InOut:
		return InOut(startPos, endPos);

	default:
		return 0;
	}
}
// ラープ
float Easing::Lerp(const float startPos, const float endPos)
{
	float dis = endPos - startPos;
	return dis * mTimeRate + startPos;
}
Vec2 Easing::Lerp(const Vec2 startPos, const Vec2 endPos)
{
	Vec2 dis = endPos - startPos;
	return dis * mTimeRate + startPos;
}
Vec3 Easing::Lerp(const Vec3 startPos, const Vec3 endPos)
{
	Vec3 dis = endPos - startPos;
	return dis * mTimeRate + startPos;
}

// イーズイン
float Easing::In(const float startPos, const float endPos)
{
	float dis = endPos - startPos;
	return dis * powf(mTimeRate, mPowNum) + startPos;
}
Vec2 Easing::In(const Vec2 startPos, const Vec2 endPos)
{
	Vec2 dis = endPos - startPos;
	return dis * powf(mTimeRate, mPowNum) + startPos;
}
Vec3 Easing::In(const Vec3 startPos, const Vec3 endPos)
{
	Vec3 dis = endPos - startPos;
	return dis * powf(mTimeRate, mPowNum) + startPos;
}

// イーズアウト
float Easing::Out(const float startPos, const float endPos)
{
	float dis = endPos - startPos;
	if ((int32_t)mPowNum % 2 == 1)
	{
		return dis * (powf(mTimeRate - 1, mPowNum) + 1) + startPos;
	}
	return dis * -1 * (powf(mTimeRate - 1, mPowNum) - 1) + startPos;
}
Vec2 Easing::Out(const Vec2 startPos, const Vec2 endPos)
{
	Vec2 dis = endPos - startPos;
	if ((int32_t)mPowNum % 2 == 1)
	{
		return dis * (powf(mTimeRate - 1, mPowNum) + 1) + startPos;
	}
	return dis * -1 * (powf(mTimeRate - 1, mPowNum) - 1) + startPos;
}
Vec3 Easing::Out(const Vec3 startPos, const Vec3 endPos)
{
	Vec3 dis = endPos - startPos;
	if ((int32_t)mPowNum % 2 == 1)
	{
		return dis * (powf(mTimeRate - 1, mPowNum) + 1) + startPos;
	}
	return dis * -1 * (powf(mTimeRate - 1, mPowNum) - 1) + startPos;
}

// インアウト
float Easing::InOut(const float startPos, const float endPos)
{
	float dis = (endPos - startPos);

	if (mTimeRate < 0.5f)
	{
		return dis / 2.f * powf(mTimeRate * 2.f, mPowNum) + startPos;
	}
	return -dis / 2.f * (powf((1.f - (mTimeRate - 0.5f) * 2.f), mPowNum) - 2.f) + startPos;

}
Vec2 Easing::InOut(const Vec2 startPos, const Vec2 endPos)
{
	Vec2 dis = (endPos - startPos) / 2;
	if (mTimeRate < 0.5f)
	{
		return In(startPos, endPos);
	}
	return Out(startPos, endPos);
}
Vec3 Easing::InOut(const Vec3 startPos, const Vec3 endPos)
{
	Vec3 dis = (endPos - startPos);

	if (mTimeRate < 0.5f)
	{
		return dis / 2.f * powf(mTimeRate * 2.f, mPowNum) + startPos;
	}
	return -dis / 2.f * (powf((1.f - (mTimeRate - 0.5f) * 2.f), mPowNum) - 2.f) + startPos;
}

// インバック
float Easing::InBack(const float startPos, const float endPos)
{
	const float back1 = 1.70154f;

	float dis = endPos - startPos;
	return dis * powf(mTimeRate, mPowNum) * ((back1 + 1.0f) * mTimeRate - back1) + startPos;
}
Vec2 Easing::InBack(const Vec2 startPos, const Vec2 endPos)
{
	const float back1 = 1.70154f;

	Vec2 dis = endPos - startPos;
	return dis * powf(mTimeRate, mPowNum) * ((back1 + 1.0f) * mTimeRate - back1) + startPos;
}
Vec3 Easing::InBack(const Vec3 startPos, const Vec3 endPos)
{
	const float back1 = 1.70154f;

	Vec3 dis = endPos - startPos;
	return dis * powf(mTimeRate, mPowNum) * ((back1 + 1.0f) * mTimeRate - back1) + startPos;
}