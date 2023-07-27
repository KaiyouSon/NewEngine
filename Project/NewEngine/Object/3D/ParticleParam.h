#pragma once
#include "MathUtil.h"
#include "Easing.h"
#include "Color.h"

namespace ParticleParameter
{
	struct PParam0
	{
		Vec3 curPos;
		Vec2 curScale;
		float curRot;
		Color curColor;
	};

	struct PParam1
	{
		Vec3 moveVec;
		Vec3 moveAccel;

		Vec3 startPos;
		Vec3 endPos;

		Vec2 startScale;
		Vec2 endScale;

		float startRot;
		float endRot;

		Color startColor;
		Color endColor;

		Easing ease;
		PParam1();
	};
}