#include "BezierCurve.h"
#include "Easing.h"
using namespace std;

BezierCurve::BezierCurve() :
	timer(0), timeRate(0), isEnd(false), startIndex(1)
{
	ease.SetEaseTimer(0);
	ease.SetPowNum(1);
}

BezierCurve::BezierCurve(const int& easeTimer) :
	timer(0), timeRate(0), isEnd(false), startIndex(1)
{
	ease.SetEaseTimer(easeTimer);
	ease.SetPowNum(1);
}

BezierCurve::BezierCurve(const int& easeTimer, const float& powNum) :
	timer(0), timeRate(0), isEnd(false), startIndex(1)
{
	ease.SetEaseTimer(easeTimer);
	ease.SetPowNum(powNum);
}

void BezierCurve::Reset()
{
	timer = 0;
	timeRate = 0;
	ease.Reset();
	isEnd = false;
}

void BezierCurve::Update()
{
	if (points.size() < 2) return;

	ease.Update();
	if (ease.GetisEnd() == true)
	{
		isEnd = true;
	}
}

Vec3 BezierCurve::InterPolation(const BezierCurveType& type)
{
	if (RecursiveLerp(points, type).size() < 2) return { 0,0,0 };

	// “ñ‰ñ
	switch (type)
	{
	case Lerp:
		return ease.Lerp(RecursiveLerp(points, type)[0], RecursiveLerp(points, type)[1]);
		break;
	case EaseIn:
		return ease.In(RecursiveLerp(points, type)[0], RecursiveLerp(points, type)[1]);
		break;
	case EaseOut:
		return ease.Out(RecursiveLerp(points, type)[0], RecursiveLerp(points, type)[1]);
		break;
	default:
		return Vec3::zero;
		break;
	}
}

vector<Vec3> BezierCurve::RecursiveLerp(const vector<Vec3>& points, const BezierCurveType& type)
{
	vector<Vec3> tempPoints;

	if (points.size() < 3)
	{
		return points;
	}
	else
	{
		switch (type)
		{
		case Lerp:
			for (size_t i = 0; i < points.size() - 1; i++)
			{
				tempPoints.push_back(ease.Lerp(points[i], points[i + 1]));
			}
			return RecursiveLerp(tempPoints, type);
			break;

		case EaseIn:
			for (size_t i = 0; i < points.size() - 1; i++)
			{
				tempPoints.push_back(ease.In(points[i], points[i + 1]));
			}
			return RecursiveLerp(tempPoints, type);
			break;

		case EaseOut:
			for (size_t i = 0; i < points.size() - 1; i++)
			{
				tempPoints.push_back(ease.Out(points[i], points[i + 1]));
			}
			return RecursiveLerp(tempPoints, type);
			break;

		default:
			return vector<Vec3>{};
			break;
		}
	}
}