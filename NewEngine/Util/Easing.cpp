#include "Easing.h"
#include "Util.h"
#include <math.h>

Easing::Easing() :
	timer(0), timeRate(0),
	easeTimer(0), powNum(1), isEnd(false)
{
}
Easing::Easing(const int& easeTimer) :
	timer(0), timeRate(0),
	easeTimer(easeTimer), powNum(1), isEnd(false)
{
}
Easing::Easing(const int& easeTimer, const float& powNum) :
	timer(0), timeRate(0),
	easeTimer(easeTimer), powNum(powNum), isEnd(false)
{
}

void Easing::Reset()
{
	timer = 0;
	timeRate = 0;
	isEnd = false;
}

void Easing::Update()
{
	timer++;
	if (timeRate >= 1)
	{
		isEnd = true;
	}
	else
	{
		timeRate = Min((float)timer / easeTimer, 1);
	}
}

// ���[�v
float Easing::Lerp(const float& startPos, const float& endPos)
{
	float dis = endPos - startPos;	
	return dis * timeRate + startPos;
}
Vec2 Easing::Lerp(const Vec2& startPos, const Vec2& endPos)
{
	Vec2 dis = endPos - startPos;
	return dis * timeRate + startPos;
}
Vec3 Easing::Lerp(const Vec3& startPos, const Vec3& endPos)
{
	Vec3 dis = endPos - startPos;
	return dis * timeRate + startPos;
}

// �C�[�Y�C��
float Easing::In(const float& startPos, const float& endPos)
{
	float dis = endPos - startPos;
	return dis * powf(timeRate, powNum) + startPos;
}
Vec2 Easing::In(const Vec2& startPos, const Vec2& endPos)
{
	Vec2 dis = endPos - startPos;
	return dis * powf(timeRate, powNum) + startPos;
}
Vec3 Easing::In(const Vec3& startPos, const Vec3& endPos)
{
	Vec3 dis = endPos - startPos;
	return dis * powf(timeRate, powNum) + startPos;
}

// �C�[�Y�A�E�g
float Easing::Out(const float& startPos, const float& endPos)
{
	float dis = endPos - startPos;
	if ((int)powNum % 2 == 1)
	{
		return dis * (powf(timeRate - 1, powNum) + 1) + startPos;
	}
	else if ((int)powNum % 2 == 0)
	{
		return dis * -1 * (powf(timeRate - 1, powNum) - 1) + startPos;
	}

	return -1;
}
Vec2 Easing::Out(const Vec2& startPos, const Vec2& endPos)
{
	Vec2 dis = endPos - startPos;
	if ((int)powNum % 2 == 1)
	{
		return dis * (powf(timeRate - 1, powNum) + 1) + startPos;
	}
	else if ((int)powNum % 2 == 0)
	{
		return dis * -1 * (powf(timeRate - 1, powNum) - 1) + startPos;
	}

	return -1;
}
Vec3 Easing::Out(const Vec3& startPos, const Vec3& endPos)
{
	Vec3 dis = endPos - startPos;
	if ((int)powNum % 2 == 1)
	{
		return dis * (powf(timeRate - 1, powNum) + 1) + startPos;
	}
	else if ((int)powNum % 2 == 0)
	{
		return dis * -1 * (powf(timeRate - 1, powNum) - 1) + startPos;
	}

	return -1;
}

// �C���A�E�g
float Easing::InOut(const float& startPos, const float& endPos)
{
	float dis1 = (endPos - startPos) / 2;
	float dis2 = (endPos - dis1);

	if (timeRate <= 0.5)
	{
		return dis1 * powf(timeRate, powNum) + startPos;
	}
	else
	{
		if ((int)powNum % 2 == 1)
		{
			return dis2 * (powf(timeRate - 1, powNum) + 1) + startPos;
		}
		else if ((int)powNum % 2 == 0)
		{
			return dis2 * -1 * (powf(timeRate - 1, powNum) - 1) + startPos;
		}
	}

	return -1;
}
Vec2 Easing::InOut(const Vec2& startPos, const Vec2& endPos)
{
	Vec2 dis = (endPos - startPos) / 2;
	if (timeRate < 0.5)
	{
		return In(startPos, endPos);
	}
	else
	{
		return Out(startPos, endPos);
	}

	return -1;
}
Vec3 Easing::InOut(const Vec3& startPos, const Vec3& endPos)
{
	if (timeRate < 0.5)
	{
		return In(startPos, endPos);
	}
	else
	{
		return Out(startPos, endPos);
	}

	return -1;
}

// �C���o�b�N
float Easing::InBack(const float& startPos, const float& endPos)
{
	const float back1 = 1.70154f;

	float dis = endPos - startPos;
	return dis * powf(timeRate, powNum) * ((back1 + 1.0f) * timeRate - back1) + startPos;
}
Vec2 Easing::InBack(const Vec2& startPos, const Vec2& endPos)
{
	const float back1 = 1.70154f;

	Vec2 dis = endPos - startPos;
	return dis * powf(timeRate, powNum) * ((back1 + 1.0f) * timeRate - back1) + startPos;
}
Vec3 Easing::InBack(const Vec3& startPos, const Vec3& endPos)
{
	const float back1 = 1.70154f;

	Vec3 dis = endPos - startPos;
	return dis * powf(timeRate, powNum) * ((back1 + 1.0f) * timeRate - back1) + startPos;
}