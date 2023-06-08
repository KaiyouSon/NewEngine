#include "Easing.h"
#include "Util.h"
#include <math.h>

Easing::Easing() :
	timer_(0), timeRate_(0),
	limitTimer_(0), powNum_(1), isEnd_(false)
{
}
Easing::Easing(const int easeTimer) :
	timer_(0), timeRate_(0),
	limitTimer_(easeTimer), powNum_(1), isEnd_(false)
{
}
Easing::Easing(const int easeTimer, const float powNum) :
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

void Easing::Update(const bool isRoop)
{
	if (isEnd_ == true)
	{
		if (timer_ >= limitTimer_)
		{
			Reset();
		}
	}

	timer_++;
	if (timeRate_ >= 1)
	{
		isEnd_ = true;
	}

	timer_ = Min<int>(timer_, limitTimer_);
	timeRate_ = Min<float>((float)timer_ / limitTimer_, 1);
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
	if ((int)powNum_ % 2 == 1)
	{
		return dis * (powf(timeRate_ - 1, powNum_) + 1) + startPos;
	}
	else if ((int)powNum_ % 2 == 0)
	{
		return dis * -1 * (powf(timeRate_ - 1, powNum_) - 1) + startPos;
	}

	return -1;
}
Vec2 Easing::Out(const Vec2 startPos, const Vec2 endPos)
{
	Vec2 dis = endPos - startPos;
	if ((int)powNum_ % 2 == 1)
	{
		return dis * (powf(timeRate_ - 1, powNum_) + 1) + startPos;
	}
	else if ((int)powNum_ % 2 == 0)
	{
		return dis * -1 * (powf(timeRate_ - 1, powNum_) - 1) + startPos;
	}

	return -1;
}
Vec3 Easing::Out(const Vec3 startPos, const Vec3 endPos)
{
	Vec3 dis = endPos - startPos;
	if ((int)powNum_ % 2 == 1)
	{
		return dis * (powf(timeRate_ - 1, powNum_) + 1) + startPos;
	}
	else if ((int)powNum_ % 2 == 0)
	{
		return dis * -1 * (powf(timeRate_ - 1, powNum_) - 1) + startPos;
	}

	return -1;
}

// インアウト
float Easing::InOut(const float startPos, const float endPos)
{
	float dis1 = (endPos - startPos) / 2;
	float dis2 = (endPos - dis1);

	if (timeRate_ <= 0.5f)
	{
		return dis1 * powf(timeRate_, powNum_) + startPos;
	}
	else
	{
		if ((int)powNum_ % 2 == 1)
		{
			return dis2 * (powf(timeRate_ - 1, powNum_) + 1) + startPos;
		}
		else if ((int)powNum_ % 2 == 0)
		{
			return dis2 * -1 * (powf(timeRate_ - 1, powNum_) - 1) + startPos;
		}
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