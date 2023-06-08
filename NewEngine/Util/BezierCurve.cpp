#include "BezierCurve.h"
#include "Easing.h"
using namespace std;

BezierCurve::BezierCurve() :
	timer_(0), timeRate_(0), isEnd_(false), startIndex_(1)
{
	ease_.SetEaseTimer(0);
	ease_.SetPowNum(1);
}

BezierCurve::BezierCurve(const uint32_t ease_Timer) :
	timer_(0), timeRate_(0), isEnd_(false), startIndex_(1)
{
	ease_.SetEaseTimer(ease_Timer);
	ease_.SetPowNum(1);
}

BezierCurve::BezierCurve(const uint32_t ease_Timer, const float powNum) :
	timer_(0), timeRate_(0), isEnd_(false), startIndex_(1)
{
	ease_.SetEaseTimer(ease_Timer);
	ease_.SetPowNum(powNum);
}

void BezierCurve::Reset()
{
	timer_ = 0;
	timeRate_ = 0;
	ease_.Reset();
	isEnd_ = false;
}

void BezierCurve::Update()
{
	if (points_.size() < 2) return;

	ease_.Update(true);
	if (ease_.GetisEnd() == true)
	{
		isEnd_ = true;
	}
}

Vec3 BezierCurve::InterPolation(const BezierType type)
{
	if (RecursiveLerp(points_, type).size() < 2) return { 0,0,0 };

	// “ñ‰ñ
	switch (type)
	{
	case BezierType::Lerp:
		return ease_.Lerp(RecursiveLerp(points_, type)[0], RecursiveLerp(points_, type)[1]);
		break;
	case BezierType::EaseIn:
		return ease_.In(RecursiveLerp(points_, type)[0], RecursiveLerp(points_, type)[1]);
		break;
	case BezierType::EaseOut:
		return ease_.Out(RecursiveLerp(points_, type)[0], RecursiveLerp(points_, type)[1]);
		break;
	default:
		return Vec3::zero;
		break;
	}
}

vector<Vec3> BezierCurve::RecursiveLerp(const vector<Vec3>& points_, const BezierType type)
{
	vector<Vec3> tempPoints;

	if (points_.size() < 3)
	{
		return points_;
	}
	else
	{
		switch (type)
		{
		case BezierType::Lerp:
			for (size_t i = 0; i < points_.size() - 1; i++)
			{
				tempPoints.push_back(ease_.Lerp(points_[i], points_[i + 1]));
			}
			return RecursiveLerp(tempPoints, type);
			break;

		case BezierType::EaseIn:
			for (size_t i = 0; i < points_.size() - 1; i++)
			{
				tempPoints.push_back(ease_.In(points_[i], points_[i + 1]));
			}
			return RecursiveLerp(tempPoints, type);
			break;

		case BezierType::EaseOut:
			for (size_t i = 0; i < points_.size() - 1; i++)
			{
				tempPoints.push_back(ease_.Out(points_[i], points_[i + 1]));
			}
			return RecursiveLerp(tempPoints, type);
			break;

		default:
			return vector<Vec3>{};
			break;
		}
	}
}