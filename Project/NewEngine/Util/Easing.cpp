#include "Easing.h"
#include "Util.h"
#include <math.h>

Easing::Easing() :
	timer_(0), timeRate_(0),
	limitTimer_(0), powNum_(1), isEnd_(false)
{
}
Easing::Easing(const int32_t easeTimer) :
	timer_(0), timeRate_(0),
	limitTimer_(easeTimer), powNum_(1), isEnd_(false)
{
}
Easing::Easing(const int32_t easeTimer, const float powNum) :
	timer_(0), timeRate_(0),
	limitTimer_(easeTimer), powNum_(powNum), isEnd_(false)
{
}

void Easing::Reset()
{
	timer_ = 0;
	timeRate_ = 0;
	isEnd_ = false;
}

void Easing::Update()
{
	timer_++;
	if (timeRate_ >= 1)
	{
		isEnd_ = true;
	}

	timer_ = Min<int32_t>(timer_, limitTimer_);
	timeRate_ = Min<float>((float)timer_ / limitTimer_, 1);
}

float Easing::Interpolation(const float startPos, const float endPos)
{
	switch (easeType_)
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

	return 0;
}

Vec2 Easing::Interpolation(const Vec2 startPos, const Vec2 endPos)
{
	switch (easeType_)
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

	return 0;
}

Vec3 Easing::Interpolation(const Vec3 startPos, const Vec3 endPos)
{
	switch (easeType_)
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

	return 0;
}

// ラープ
float Easing::Lerp(const float startPos, const float endPos)
{
	float dis = endPos - startPos;
	return dis * timeRate_ + startPos;
}
Vec2 Easing::Lerp(const Vec2 startPos, const Vec2 endPos)
{
	Vec2 dis = endPos - startPos;
	return dis * timeRate_ + startPos;
}
Vec3 Easing::Lerp(const Vec3 startPos, const Vec3 endPos)
{
	Vec3 dis = endPos - startPos;
	return dis * timeRate_ + startPos;
}

// イーズイン
float Easing::In(const float startPos, const float endPos)
{
	float dis = endPos - startPos;
	return dis * powf(timeRate_, powNum_) + startPos;
}
Vec2 Easing::In(const Vec2 startPos, const Vec2 endPos)
{
	Vec2 dis = endPos - startPos;
	return dis * powf(timeRate_, powNum_) + startPos;
}
Vec3 Easing::In(const Vec3 startPos, const Vec3 endPos)
{
	Vec3 dis = endPos - startPos;
	return dis * powf(timeRate_, powNum_) + startPos;
}

// イーズアウト
float Easing::Out(const float startPos, const float endPos)
{
	float dis = endPos - startPos;
	if ((int32_t)powNum_ % 2 == 1)
	{
		return dis * (powf(timeRate_ - 1, powNum_) + 1) + startPos;
	}
	else if ((int32_t)powNum_ % 2 == 0)
	{
		return dis * -1 * (powf(timeRate_ - 1, powNum_) - 1) + startPos;
	}

	return -1;
}
Vec2 Easing::Out(const Vec2 startPos, const Vec2 endPos)
{
	Vec2 dis = endPos - startPos;
	if ((int32_t)powNum_ % 2 == 1)
	{
		return dis * (powf(timeRate_ - 1, powNum_) + 1) + startPos;
	}
	else if ((int32_t)powNum_ % 2 == 0)
	{
		return dis * -1 * (powf(timeRate_ - 1, powNum_) - 1) + startPos;
	}

	return -1;
}
Vec3 Easing::Out(const Vec3 startPos, const Vec3 endPos)
{
	Vec3 dis = endPos - startPos;
	if ((int32_t)powNum_ % 2 == 1)
	{
		return dis * (powf(timeRate_ - 1, powNum_) + 1) + startPos;
	}
	else if ((int32_t)powNum_ % 2 == 0)
	{
		return dis * -1 * (powf(timeRate_ - 1, powNum_) - 1) + startPos;
	}

	return -1;
}

// インアウト
float Easing::InOut(const float startPos, const float endPos)
{
	float dis = (endPos - startPos);

	if (timeRate_ < 0.5f)
	{
		return dis / 2.f * powf(timeRate_ * 2.f, powNum_) + startPos;
	}
	else
	{
		return -dis / 2.f * (powf((1.f - (timeRate_ - 0.5f) * 2.f), powNum_) - 2.f) + startPos;
	}

	return -1;
}
Vec2 Easing::InOut(const Vec2 startPos, const Vec2 endPos)
{
	Vec2 dis = (endPos - startPos) / 2;
	if (timeRate_ < 0.5f)
	{
		return In(startPos, endPos);
	}
	else
	{
		return Out(startPos, endPos);
	}

	return -1;
}
Vec3 Easing::InOut(const Vec3 startPos, const Vec3 endPos)
{
	Vec3 dis = (endPos - startPos);

	if (timeRate_ < 0.5f)
	{
		return dis / 2.f * powf(timeRate_ * 2.f, powNum_) + startPos;
	}
	else
	{
		return -dis / 2.f * (powf((1.f - (timeRate_ - 0.5f) * 2.f), powNum_) - 2.f) + startPos;
	}

	return -1;
}

// インバック
float Easing::InBack(const float startPos, const float endPos)
{
	const float back1 = 1.70154f;

	float dis = endPos - startPos;
	return dis * powf(timeRate_, powNum_) * ((back1 + 1.0f) * timeRate_ - back1) + startPos;
}
Vec2 Easing::InBack(const Vec2 startPos, const Vec2 endPos)
{
	const float back1 = 1.70154f;

	Vec2 dis = endPos - startPos;
	return dis * powf(timeRate_, powNum_) * ((back1 + 1.0f) * timeRate_ - back1) + startPos;
}
Vec3 Easing::InBack(const Vec3 startPos, const Vec3 endPos)
{
	const float back1 = 1.70154f;

	Vec3 dis = endPos - startPos;
	return dis * powf(timeRate_, powNum_) * ((back1 + 1.0f) * timeRate_ - back1) + startPos;
}